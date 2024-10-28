/**
* Author: Aidan Jacobs
* Assignment: Lunar Lander
* Date due: 2024-10-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define LOG(argument) std::cout << argument << '\n'
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define PLATFORM_COUNT 13
#define SPIKEPLATFORM_COUNT 7
#define FILLPLATFORM_COUNT 3
#define SAFEPLATFORM_COUNT 2


#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
#include <vector>
#include <cstdlib>
#include "Entity.h"

// ––––– STRUCTS AND ENUMS ––––– //
struct GameState
{
    Entity* player;
    Entity* platforms;
};

// ––––– CONSTANTS ––––– //
constexpr int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

constexpr float BG_RED     = 0.1922f,
            BG_BLUE    = 0.549f,
            BG_GREEN   = 0.9059f,
            BG_OPACITY = 1.0f;

constexpr int FONTBANK_SIZE = 16;

constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr float MILLISECONDS_IN_SECOND = 1000.0;
constexpr char SPRITESHEET_FILEPATH[] = "assets/Charizard.png",
PLATFORM1_FILEPATH[] = "assets/plat1.png",
PLATFORM2_FILEPATH[] = "assets/plat2.png",
PLATFORM3_FILEPATH[] = "assets/plat3.png",
PLATFORM4_FILEPATH[] = "assets/plat4.png",
FONTSHEET_FILEPATH[] = "assets/font1.png";


constexpr int NUMBER_OF_TEXTURES = 1;
constexpr GLint LEVEL_OF_DETAIL  = 0;
constexpr GLint TEXTURE_BORDER   = 0;

void draw_text(ShaderProgram* shader_program, GLuint font_texture_id, std::string text,
    float font_size, float spacing, glm::vec3 position);

// ––––– GLOBAL VARIABLES ––––– //
GameState g_state;

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

int g_stamina = 15000;

GLuint g_font_texture_id;


// ––––– GENERAL FUNCTIONS ––––– //
void draw_text(ShaderProgram* shader_program, GLuint font_texture_id, std::string text,
    float font_size, float spacing, glm::vec3 position)
{
    // Scale the size of the fontbank in the UV-plane
    // We will use this for spacing and positioning
    float width = 1.0f / FONTBANK_SIZE;
    float height = 1.0f / FONTBANK_SIZE;

    // Instead of having a single pair of arrays, we'll have a series of pairs—one for
    // each character. Don't forget to include <vector>!
    std::vector<float> vertices;
    std::vector<float> texture_coordinates;

    // For every character...
    for (int i = 0; i < text.size(); i++) {
        // 1. Get their index in the spritesheet, as well as their offset (i.e. their
        //    position relative to the whole sentence)
        int spritesheet_index = (int)text[i];  // ascii value of character
        float offset = (font_size + spacing) * i;

        // 2. Using the spritesheet index, we can calculate our U- and V-coordinates
        float u_coordinate = (float)(spritesheet_index % FONTBANK_SIZE) / FONTBANK_SIZE;
        float v_coordinate = (float)(spritesheet_index / FONTBANK_SIZE) / FONTBANK_SIZE;

        // 3. Inset the current pair in both vectors
        vertices.insert(vertices.end(), {
            offset + (-0.5f * font_size), 0.5f * font_size,
            offset + (-0.5f * font_size), -0.5f * font_size,
            offset + (0.5f * font_size), 0.5f * font_size,
            offset + (0.5f * font_size), -0.5f * font_size,
            offset + (0.5f * font_size), 0.5f * font_size,
            offset + (-0.5f * font_size), -0.5f * font_size,
            });

        texture_coordinates.insert(texture_coordinates.end(), {
            u_coordinate, v_coordinate,
            u_coordinate, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate + width, v_coordinate + height,
            u_coordinate + width, v_coordinate,
            u_coordinate, v_coordinate + height,
            });
    }

    // 4. And render all of them using the pairs
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(model_matrix, position);

    shader_program->set_model_matrix(model_matrix);
    glUseProgram(shader_program->get_program_id());

    glVertexAttribPointer(shader_program->get_position_attribute(), 2, GL_FLOAT, false, 0,
        vertices.data());
    glEnableVertexAttribArray(shader_program->get_position_attribute());

    glVertexAttribPointer(shader_program->get_tex_coordinate_attribute(), 2, GL_FLOAT,
        false, 0, texture_coordinates.data());
    glEnableVertexAttribArray(shader_program->get_tex_coordinate_attribute());

    glBindTexture(GL_TEXTURE_2D, font_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(shader_program->get_position_attribute());
    glDisableVertexAttribArray(shader_program->get_tex_coordinate_attribute());
}

GLuint load_texture(const char* filepath)
{
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }

    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(image);

    return textureID;
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow( "I choose you!",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    // ––––– VIDEO ––––– //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_program.set_projection_matrix(g_projection_matrix);
    g_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);


    // ––––– PLATFORMS ––––– //
    GLuint platform1_texture_id = load_texture(PLATFORM1_FILEPATH);
    GLuint platform2_texture_id = load_texture(PLATFORM2_FILEPATH);
    GLuint platform3_texture_id = load_texture(PLATFORM3_FILEPATH);
    GLuint platform4_texture_id = load_texture(PLATFORM4_FILEPATH);


    g_state.platforms = new Entity[PLATFORM_COUNT];

    //Initialize and set types of every platform entity

    //Spike group
    for (int i = 0; i < SPIKEPLATFORM_COUNT; i++)
    {
        if (i % 2 == 0)
        {
            g_state.platforms[i].set_texture_id(platform3_texture_id);

        }
        else
        {
            g_state.platforms[i].set_texture_id(platform4_texture_id);
        } 
        g_state.platforms[i].set_position(glm::vec3(-3.5f + i, -3.25f, 0.0f));
        g_state.platforms[i].set_width(1.0f);
        g_state.platforms[i].set_height(1.0f);
        g_state.platforms[i].set_entity_type(UNSAFEPLATFORM);
        g_state.platforms[i].update(0.0f, NULL, NULL, 0);
    }
    //terrain platforms
    for (int i = 0; i < FILLPLATFORM_COUNT; i++)
    {
        if (i == 0)
        {
            g_state.platforms[SPIKEPLATFORM_COUNT+i].set_position(glm::vec3(3.5f, -3.25f, 0.0f));
        }
        else if (i == 1)
        {
            g_state.platforms[SPIKEPLATFORM_COUNT+i].set_position(glm::vec3(4.5f, -3.25f, 0.0f));
        }
        else
        {
            g_state.platforms[SPIKEPLATFORM_COUNT + i].set_position(glm::vec3(4.5f, -2.25f, 0.0f));
        }
        g_state.platforms[SPIKEPLATFORM_COUNT + i].set_texture_id(platform2_texture_id);
        g_state.platforms[SPIKEPLATFORM_COUNT + i].set_width(1.0f);
        g_state.platforms[SPIKEPLATFORM_COUNT + i].set_height(1.0f);
        g_state.platforms[SPIKEPLATFORM_COUNT + i].set_entity_type(UNSAFEPLATFORM);
        g_state.platforms[SPIKEPLATFORM_COUNT + i].update(0.0f, NULL, NULL, 0);
    }
    //platforms for landing
    for (int i = 0; i < SAFEPLATFORM_COUNT; i++)
    {
        if (i == 0)
        {
            g_state.platforms[PLATFORM_COUNT - i - 1].set_position(glm::vec3(-4.5f, -3.25f, 0.0f));
        }

        else
        {
            g_state.platforms[PLATFORM_COUNT - i - 1].set_position(glm::vec3(3.5f, -2.25f, 0.0f));
        }
        g_state.platforms[PLATFORM_COUNT - i - 1].set_texture_id(platform1_texture_id);
        g_state.platforms[PLATFORM_COUNT - i - 1].set_width(1.0f);
        g_state.platforms[PLATFORM_COUNT - i - 1].set_height(1.0f);
        g_state.platforms[PLATFORM_COUNT - i - 1].set_entity_type(SAFEPLATFORM);
        g_state.platforms[PLATFORM_COUNT - i - 1].update(0.0f, NULL, NULL, 0);
    }
    //extra spike at right edge
    g_state.platforms[PLATFORM_COUNT - SAFEPLATFORM_COUNT - 1].set_position(glm::vec3(4.5f, -1.25f, 0.0f));
    g_state.platforms[PLATFORM_COUNT - SAFEPLATFORM_COUNT - 1].set_texture_id(platform3_texture_id);
    g_state.platforms[PLATFORM_COUNT - SAFEPLATFORM_COUNT - 1].set_width(1.0f);
    g_state.platforms[PLATFORM_COUNT - SAFEPLATFORM_COUNT - 1].set_height(1.0f);
    g_state.platforms[PLATFORM_COUNT - SAFEPLATFORM_COUNT - 1].set_entity_type(UNSAFEPLATFORM);
    g_state.platforms[PLATFORM_COUNT - SAFEPLATFORM_COUNT - 1].update(0.0f, NULL, NULL, 0);




    // ––––– PLAYER (Charizard) ––––– //
    GLuint player_texture_id = load_texture(SPRITESHEET_FILEPATH);
 

    g_state.player = new Entity(
        player_texture_id, // texture_id
        5.0f,       // speed
        .5f,       // width
        .5f,      // height
        PLAYER
    );
    
    g_state.player->set_acceleration(glm::vec3(0.0f, -.05f, 0.0f));
    g_state.player->set_scale(glm::vec3(.6367f, .5f, 0.0f));

    g_font_texture_id = load_texture(FONTSHEET_FILEPATH);

    // ––––– GENERAL ––––– //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{

    g_state.player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        // Quit the game with a keystroke
                        g_game_is_running = false;
                        break;

                    default:
                        break;
                }

            default:
                break;
        }
    }

    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_LEFT])
    {
        if (g_stamina > 0)
        {
            g_state.player->move_left();
            g_stamina -= .00000001;;
        }
    }
    else if (key_state[SDL_SCANCODE_RIGHT])
    {
        if (g_stamina > 0)
        {
            g_state.player->move_right();
            g_stamina -= .00000001;
        }
    }
    else if (key_state[SDL_SCANCODE_UP])
    {
        if (g_stamina > 0)
        {
            g_state.player->move_up();
            g_stamina -= .00000001;
        }
    }
}

void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (g_state.player->get_position().x < -5.0 || g_state.player->get_position().x > 5.0 || g_state.player->get_position().y < -3.75)
    {
        g_state.player->set_flying(false);
    }

    if (!(g_state.player->get_flying()))
    {
        return;
    }

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP)
    {
        g_state.player->update(FIXED_TIMESTEP, NULL, g_state.platforms, PLATFORM_COUNT);
        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    g_state.player->render(&g_program);

    for (int i = 0; i < PLATFORM_COUNT; i++) g_state.platforms[i].render(&g_program); 

    //Stamina (Fuel) UI
    draw_text(&g_program, g_font_texture_id, "Stamina: ", .4f, 0.025f,
        glm::vec3(-4.0f, 3.0f, 0.0f));
    draw_text(&g_program, g_font_texture_id, std::to_string(g_stamina), .4f, 0.025f,
        glm::vec3(0.0f, 3.0f, 0.0f));

    if (!(g_state.player->get_flying()))
    {
        if (g_state.player->get_mission_success())
        {
            //Success message
            draw_text(&g_program, g_font_texture_id, "Mission Accomplished!", .4f, 0.025f,
                glm::vec3(-4.0f, 2.0f, 0.0f));
            draw_text(&g_program, g_font_texture_id, "Charizard Cooked", .4f, 0.025f,
                glm::vec3(-4.0f, 1.0f, 0.0f));
        }
        else
        {
            //Failure message
            draw_text(&g_program, g_font_texture_id, "Mission Failure", 0.4f, 0.025f,
                glm::vec3(-4.0f, 2.0f, 0.0f));
            draw_text(&g_program, g_font_texture_id, "Charizard Got Cooked", 0.4f, 0.025f,
                glm::vec3(-4.0f, 1.0f, 0.0f));
        }
    }

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    delete [] g_state.platforms;
    delete g_state.player;
}

// ––––– GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
