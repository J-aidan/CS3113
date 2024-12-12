/**
* Author: Aidan Jacobs
* Assignment: Kessel Run
* Date due: 2024-12-11, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "Menu.h"
#include "Effects.h"

// ––––– CONSTANTS ––––– //
constexpr int WINDOW_WIDTH  = 640*2,
          WINDOW_HEIGHT = 480*2;

constexpr float BG_RED     = 0.0f,
            BG_BLUE    = 0.0f,
            BG_GREEN   = 0.0f,
            BG_OPACITY = 1.0f;

constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/greyed_textured.glsl";

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

float SCROLL_SPEED = 2.0f;

enum AppStatus { RUNNING, TERMINATED };

//Controller Input
SDL_Joystick* g_player_one_controller;

int joystick_lim = 8000;



// ––––– GLOBAL VARIABLES ––––– //

GLuint g_font_texture_id;

Scene  *g_current_scene;
Menu* g_menu;
LevelA *g_levelA;
LevelB* g_levelB;
LevelC* g_levelC;

Scene   *g_levels[4];

int lives = 3;

bool g_game_won = false;
bool g_game_done = false;
int g_game_over = 1;

SDL_Window* g_display_window;


ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

float g_camx_accum = -5.0f;

bool g_is_colliding_bottom = false;

AppStatus g_app_status = RUNNING;

void switch_to_scene(Scene *scene);
void initialise();
void process_input();
void update();
void render();
void shutdown();

// ––––– GENERAL FUNCTIONS ––––– //
void switch_to_scene(Scene *scene)
{    
    g_current_scene = scene;
    g_current_scene->initialise(); // DON'T FORGET THIS STEP!
    g_camx_accum = -5.0f;
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

    // Open the 1st controller found. Returns null on error
    g_player_one_controller = SDL_JoystickOpen(0);

    g_display_window = SDL_CreateWindow("Space Time!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_font_texture_id = Utility::load_texture("assets/font1.png");


    g_menu = new Menu();
    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_levelC = new LevelC();
    
    g_levels[0] = g_menu;
    g_levels[1] = g_levelA;
    g_levels[2] = g_levelB;
    g_levels[3] = g_levelC;

    
    //Start on menu
    switch_to_scene(g_levels[0]);

    //g_shader_program.set_game_over(g_game_over);
    
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_current_scene->get_state().player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_app_status = TERMINATED;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_app_status = TERMINATED;
                break;
            case SDLK_e:
                if (g_current_scene == g_levels[1] || g_current_scene == g_levels[2] || g_current_scene == g_levels[3])
                {
                    Mix_PlayChannel(-1, g_current_scene->get_state().pshoot_sfx, 0);
                    g_current_scene->get_state().player->shoot();
                }
            case SDLK_RETURN:
                //start first level
                if (g_current_scene == g_levels[0])
                {
                    g_levels[0]->game_started = true;
                }
                break;

            case SDLK_SPACE:
                // Jump
                if (g_current_scene->get_state().player->get_collided_bottom())
                {
                    //Mix_PlayChannel(-1, g_current_scene->get_state().jump_sfx, 0);
                    //g_current_scene->get_state().player->jump();

                }
                break;

            default:
                break;
            }

        case SDL_JOYBUTTONDOWN:
            switch (event.jbutton.button) {
            case 0: //A
                //start first level
                if (g_current_scene == g_levels[0])
                {
                    g_levels[0]->game_started = true;
                }
                break;
            case 2: //X
                if (g_current_scene == g_levels[1] || g_current_scene == g_levels[2] || g_current_scene == g_levels[3])
                {
                    Mix_PlayChannel(-1, g_current_scene->get_state().pshoot_sfx, 0);
                    g_current_scene->get_state().player->shoot();
                }

            default:
                break;
            }

        default:
            break;
        }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    const int left_stick_x = SDL_JoystickGetAxis(g_player_one_controller, 0);
    const int left_stick_y = SDL_JoystickGetAxis(g_player_one_controller, 1);

    const int lt_press = SDL_JoystickGetAxis(g_player_one_controller, 2);
    const int rt_press = SDL_JoystickGetAxis(g_player_one_controller, 5);



    if (key_state[SDL_SCANCODE_A] || left_stick_x < -joystick_lim)
    {
        g_current_scene->get_state().player->move_left();
    }
    if (key_state[SDL_SCANCODE_D] || left_stick_x > joystick_lim)
    {
        g_current_scene->get_state().player->move_right();
    }
    if (key_state[SDL_SCANCODE_W] || left_stick_y < -joystick_lim)
    {
        g_current_scene->get_state().player->move_up();
    }
    if (key_state[SDL_SCANCODE_S] || left_stick_y > joystick_lim)
    {
        g_current_scene->get_state().player->move_down();
    }
    if (key_state[SDL_SCANCODE_E] || rt_press > joystick_lim)
    {
        //g_current_scene->get_state().player->shoot();
    }
         
    if (glm::length( g_current_scene->get_state().player->get_movement()) > 1.0f)
        g_current_scene->get_state().player->normalise_movement();
    
}

void update()
{
        float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
        float delta_time = ticks - g_previous_ticks;
        g_previous_ticks = ticks;

        delta_time += g_accumulator;

        if (delta_time < FIXED_TIMESTEP)
        {
            g_accumulator = delta_time;
            return;
        }

        while (delta_time >= FIXED_TIMESTEP) {
            g_current_scene->update(FIXED_TIMESTEP);
            if (g_current_scene != g_levels[0])
            {
                g_current_scene->keep_player_onscreen(g_camx_accum);
            }

            //Camera update
            g_view_matrix = glm::mat4(1.0f);
            if (g_current_scene == g_levels[0])
            {
                g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5.0, 3.75, 0));
            }
            else
            {
                g_camx_accum -= SCROLL_SPEED * delta_time;
                g_view_matrix = glm::translate(g_view_matrix, glm::vec3(g_camx_accum, 3.75, 0));
            }


            g_is_colliding_bottom = g_current_scene->get_state().player->get_collided_bottom();

            delta_time -= FIXED_TIMESTEP;
        }

        g_accumulator = delta_time;

        //check for loss of life, if so, restart level
        if (g_current_scene->get_state().m_level_fail && lives > 0)
        {
            Mix_PlayChannel(-1, g_current_scene->get_state().explosion_sfx, 0);
            g_current_scene->reset_level();
            g_camx_accum = -5.0f;
            lives--;
           // glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
        }
        if(lives == 0)
        {
            g_game_done = true;
            //glClearColor(0.0f, 0.0f, 0.0f, BG_OPACITY);
            GLint game_over_location = glGetUniformLocation(g_shader_program.get_program_id(), "game_over");
            glUniform1i(game_over_location, 2);
            //g_shader_program.set_game_over(g_game_over);
            SCROLL_SPEED = 0;
        }
        //check for win
        if (g_current_scene == g_levels[3] && g_current_scene->get_state().player->get_position().x > 110.0f)
        {
            g_game_won = true;
            g_game_done = true;
            //glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
        }
    
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
       
    glClear(GL_COLOR_BUFFER_BIT);
       
    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //
    if (g_game_done && g_game_won)
    {
        //win
        g_current_scene->render(&g_shader_program);
        Utility::draw_text(&g_shader_program, g_font_texture_id, "You Win!", .4f, 0.0125f,
            glm::vec3(g_current_scene->get_state().player->get_position().x-2.0f, -4.0f, 0.0f));
    }
    else if(g_game_done && !g_game_won)
    {
        //loss
        g_current_scene->render(&g_shader_program);
        Utility::draw_text(&g_shader_program, g_font_texture_id, "You Lose", .4f, 0.125f,
            glm::vec3(g_current_scene->get_state().player->get_position().x - 2.0f, -3.0f, 0.0f));
    }
    else
    {

        g_current_scene->render(&g_shader_program);
    }

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_JoystickClose(g_player_one_controller);
    SDL_Quit();
    
    delete g_levelA;
    delete g_levelB;
    delete g_levelC;
}

// ––––– DRIVER GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();
    
    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        
        if (g_current_scene->get_state().next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->get_state().next_scene_id]);
        
        render();
    }
    
    shutdown();
    return 0;
}
