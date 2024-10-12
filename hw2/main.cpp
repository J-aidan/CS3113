#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define LOG(argument) std::cout << argument << '\n'

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "cmath"
#include <ctime>
#include <random>

enum AppStatus { RUNNING, TERMINATED };

constexpr float WINDOW_SIZE_MULT = 1.0f;

constexpr int WINDOW_WIDTH = 640 * WINDOW_SIZE_MULT,
WINDOW_HEIGHT = 480 * WINDOW_SIZE_MULT;

constexpr float BG_RED = 0.1f,
BG_GREEN = 0.1f,
BG_BLUE = 0.1,
BG_OPACITY = 1.0f;

constexpr int FONTBANK_SIZE = 16;

constexpr int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

constexpr char CHARLES_SPRITE_FILEPATH[] = "charles.png",
MAX_SPRITE_FILEPATH[] = "max.png",
LANDO_SPRITE_FILEPATH[] = "lando.png",
FONTSHEET_FILEPATH[] = "font1.png",
BACKGROUND_FILEPATH[] = "track.jpg";

//storing vars for initialization of projection matrix for later use, to
//reduce use of "magic" numbers
constexpr float LIM_TOP = 3.75f;
constexpr float LIM_RIGHT = 5.0f;

constexpr glm::vec3 INIT_SCALE_CHARLES = glm::vec3(0.5f, 0.5f, 0.0f),
INIT_SCALE_MAX = glm::vec3(1.0f, 2.468f, 0.0f),
INIT_SCALE_LANDO = glm::vec3(1.0f, 2.468f, 0.0f),
INIT_POS_CHARLES = glm::vec3(0.0f, 0.0f, 0.0f),
INIT_POS_MAX = glm::vec3(-4.0f, 0.0f, 0.0f),
INIT_POS_LANDO = glm::vec3(4.0f, 0.0f, 0.0f);

SDL_Window* g_display_window;

AppStatus g_app_status = RUNNING;
ShaderProgram g_shader_program = ShaderProgram();
glm::mat4 g_view_matrix, g_projection_matrix, g_charles_matrix, g_charles2_matrix, g_charles3_matrix,
g_max_matrix, g_lando_matrix;

float g_previous_ticks = 0.0f;

GLuint g_charles_texture_id;
GLuint g_max_texture_id;
GLuint g_lando_texture_id;
GLuint g_font_texture_id;
GLuint g_background_texture_id;

glm::vec3 g_max_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_max_movement = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 g_lando_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_lando_movement = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 g_charles_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_charles_movement = glm::vec3(-1.0f, 0.0f, 0.0f);

glm::vec3 g_charles2_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_charles2_movement = glm::vec3(-1.0f, 0.0f, 0.0f);

glm::vec3 g_charles3_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_charles3_movement = glm::vec3(-1.0f, 0.0f, 0.0f);

float g_car_speed = 3.5f;

bool g_multiplayerEnabled = true;

bool g_cpu_up = true;

bool g_game_running = true;

bool g_max_is_winner = false;

int g_balls_in_play = 1;

//Uniform Distribution used for redirection of balls at random upon collision
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> distr(-2.0f, 2.0f);

void initialise();
void process_input();
void update();
void render();
void shutdown();

constexpr GLint NUMBER_OF_TEXTURES = 1;  // to be generated, that is
constexpr GLint LEVEL_OF_DETAIL = 0;  // base image level; Level n is the nth mipmap reduction image
constexpr GLint TEXTURE_BORDER = 0;  // this value MUST be zero

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
    // STEP 1: Loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        assert(false);
    }

    // STEP 2: Generating and binding a texture ID to our image
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // STEP 3: Setting our texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // STEP 4: Releasing our file from memory and returning our texture id
    stbi_image_free(image);

    return textureID;
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("DUDUDUDU, MAX VERSTAPPEN",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);


    if (g_display_window == nullptr)
    {
        shutdown();
    }
#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_charles_matrix = glm::mat4(1.0f);
    g_charles2_matrix = glm::mat4(1.0f);
    g_charles3_matrix = glm::mat4(1.0f);
    g_max_matrix = glm::mat4(1.0f);
    g_lando_matrix = glm::mat4(1.0f);
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-LIM_RIGHT, LIM_RIGHT, -LIM_TOP, LIM_TOP, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    g_charles_texture_id = load_texture(CHARLES_SPRITE_FILEPATH);
    g_max_texture_id = load_texture(MAX_SPRITE_FILEPATH);
    g_lando_texture_id = load_texture(LANDO_SPRITE_FILEPATH);
    g_font_texture_id = load_texture(FONTSHEET_FILEPATH);
    g_background_texture_id = load_texture(BACKGROUND_FILEPATH);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_max_movement = glm::vec3(0.0f);
    g_lando_movement = glm::vec3(0.0f);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_app_status = TERMINATED;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {

            case SDLK_q:
                // Quit the game with a keystroke
                g_app_status = TERMINATED;
                break;

            case SDLK_t:
                //Switch on and off multiplayer
                g_multiplayerEnabled = !g_multiplayerEnabled;
                break;

            case SDLK_1:
                //Set number of balls 
                g_balls_in_play = 1;
                break;
            case SDLK_2:
                //Set number of balls 
                g_balls_in_play = 2;
                break;
            case SDLK_3:
                //Set number of balls 
                g_balls_in_play = 3;
                break;

            default:
                break;
            }

        default:
            break;
        }
    }


    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_W] && g_max_position.y <
        (LIM_TOP - (INIT_SCALE_MAX.y / 2.0f)))
    {
        g_max_movement.y = 1.0f;
    }
    else if (key_state[SDL_SCANCODE_S] && g_max_position.y >
        -(LIM_TOP - (INIT_SCALE_MAX.y / 2.0f)))
    {
        g_max_movement.y = -1.0f;
    }

    if (key_state[SDL_SCANCODE_UP] && g_multiplayerEnabled && g_lando_position.y <
        (LIM_TOP - (INIT_SCALE_LANDO.y / 2.0f)))
    {
        g_lando_movement.y = 1.0f;
    }
    else if (key_state[SDL_SCANCODE_DOWN] && g_multiplayerEnabled && g_lando_position.y >
        -(LIM_TOP - (INIT_SCALE_LANDO.y / 2.0f)))
    {
        g_lando_movement.y = -1.0f;
    }

    // This makes sure that the player can't "cheat" their way into moving
    // faster
    if (glm::length(g_max_movement) > 1.0f)
    {
        g_max_movement = glm::normalize(g_max_movement);
    }
    if (glm::length(g_lando_movement) > 1.0f)
    {
        g_lando_movement = glm::normalize(g_lando_movement);
    }
}

void update()
{
    // --- DELTA TIME CALCULATIONS --- //
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - g_previous_ticks; // the delta time is the difference from the last frame
    g_previous_ticks = ticks;

    if(!g_game_running)
    {
        return;
    }

    if (g_charles_position.x <= -LIM_RIGHT)
    {
        //Lando Win
        g_game_running = false;
    }
    if (g_charles_position.x >= LIM_RIGHT)
    {
        //Max Win
        g_max_is_winner = true;
        g_game_running = false;
    }
    if (g_balls_in_play >= 2)
    {
        if (g_charles2_position.x <= -LIM_RIGHT)
        {
            //Lando Win
            g_game_running = false;
        }
        if (g_charles2_position.x >= LIM_RIGHT)
        {
            //Max Win
            g_max_is_winner = true;
            g_game_running = false;
        }
        if (g_balls_in_play == 3)
        {
            if (g_charles3_position.x <= -LIM_RIGHT)
            {
                //Lando Win
                g_game_running = false;
            }
            if (g_charles3_position.x >= LIM_RIGHT)
            {
                //Max Win
                g_max_is_winner = true;
                g_game_running = false;
            }
        }
    }

    if(g_game_running)
    {
        // --- ACCUMULATOR LOGIC --- //
        g_max_position += g_max_movement * g_car_speed * delta_time;

        //CPU movement logic if in single player mode
        if (!g_multiplayerEnabled)
        {
            if (g_cpu_up)
            {
                g_lando_movement.y = 1.0f;
            }
            else
            {
                g_lando_movement.y = -1.0f;
            }
            //flip direction of CPU movement if wall reached
            if (g_lando_position.y >= (LIM_TOP - (INIT_SCALE_MAX.y / 2.0f)))
            {
                g_cpu_up = false;
            }
            if (g_lando_position.y <= -(LIM_TOP - (INIT_SCALE_MAX.y / 2.0f)))
            {
                g_cpu_up = true;
            }
        }

        g_lando_position += g_lando_movement * g_car_speed * delta_time;

        //ball movement - collision with top and bottom
        if (g_charles_position.y >= (LIM_TOP - (INIT_SCALE_CHARLES.y / 2.0f)))
        {
            g_charles_movement.y = -fabs(g_charles_movement.y);
        }
        if (g_charles_position.y <= -(LIM_TOP - (INIT_SCALE_CHARLES.y / 2.0f)))
        {
            g_charles_movement.y = fabs(g_charles_movement.y);
        }
        g_charles_position += g_charles_movement * g_car_speed * delta_time;

        if (g_balls_in_play >= 2)
        {
            if (g_charles2_position.y >= (LIM_TOP - (INIT_SCALE_CHARLES.y / 2.0f)))
            {
                g_charles2_movement.y = -fabs(g_charles2_movement.y);
            }
            if (g_charles2_position.y <= -(LIM_TOP - (INIT_SCALE_CHARLES.y / 2.0f)))
            {
                g_charles2_movement.y = fabs(g_charles2_movement.y);
            }
            g_charles2_position += g_charles2_movement * g_car_speed * delta_time;
            if (g_balls_in_play == 3)
            {
                if (g_charles3_position.y >= (LIM_TOP - (INIT_SCALE_CHARLES.y / 2.0f)))
                {
                    g_charles3_movement.y = -fabs(g_charles3_movement.y);
                }
                if (g_charles3_position.y <= -(LIM_TOP - (INIT_SCALE_CHARLES.y / 2.0f)))
                {
                    g_charles3_movement.y = fabs(g_charles3_movement.y);
                }
                g_charles3_position += g_charles3_movement * g_car_speed * delta_time;
            }
        }
        // --- TRANSLATION --- //
        g_max_matrix = glm::mat4(1.0f);
        g_max_matrix = glm::translate(g_max_matrix, INIT_POS_MAX);
        g_max_matrix = glm::translate(g_max_matrix, g_max_position);

        g_lando_matrix = glm::mat4(1.0f);
        g_lando_matrix = glm::translate(g_lando_matrix, INIT_POS_LANDO);
        g_lando_matrix = glm::translate(g_lando_matrix, g_lando_position);

        g_charles_matrix = glm::mat4(1.0f);
        g_charles_matrix = glm::translate(g_charles_matrix, INIT_POS_CHARLES);
        g_charles_matrix = glm::translate(g_charles_matrix, g_charles_position);

        if (g_balls_in_play >= 2)
        {
            g_charles2_matrix = glm::mat4(1.0f);
            g_charles2_matrix = glm::translate(g_charles2_matrix, INIT_POS_CHARLES);
            g_charles2_matrix = glm::translate(g_charles2_matrix, g_charles2_position);
            if (g_balls_in_play == 3)
            {
                g_charles3_matrix = glm::mat4(1.0f);
                g_charles3_matrix = glm::translate(g_charles3_matrix, INIT_POS_CHARLES);
                g_charles3_matrix = glm::translate(g_charles3_matrix, g_charles3_position);
            }
        }


        // --- SCALING --- //
        g_max_matrix = glm::scale(g_max_matrix, INIT_SCALE_MAX);
        g_lando_matrix = glm::scale(g_lando_matrix, INIT_SCALE_LANDO);
        g_charles_matrix = glm::scale(g_charles_matrix, INIT_SCALE_CHARLES);
        if (g_balls_in_play >= 2)
        {
            g_charles2_matrix = glm::scale(g_charles2_matrix, INIT_SCALE_CHARLES);
            if (g_balls_in_play == 3)
            {
                g_charles3_matrix = glm::scale(g_charles3_matrix, INIT_SCALE_CHARLES);
            }
        }

        // --- COLLISION LOGIC --- //
        float max_x_distance = fabs(INIT_POS_MAX.x - g_charles_position.x) -
            ((INIT_SCALE_CHARLES.x + INIT_SCALE_MAX.x) / 2.0f);

        float max_y_distance = fabs(g_max_position.y - g_charles_position.y) -
            ((INIT_SCALE_CHARLES.y + INIT_SCALE_MAX.y) / 2.0f);

        float lando_x_distance = fabs(INIT_POS_LANDO.x - g_charles_position.x) -
            ((INIT_SCALE_CHARLES.x + INIT_SCALE_LANDO.x) / 2.0f);

        float lando_y_distance = fabs(g_lando_position.y - g_charles_position.y) -
            ((INIT_SCALE_CHARLES.y + INIT_SCALE_LANDO.y) / 2.0f);


        if (max_x_distance < 0.0f && max_y_distance < 0.0f)
        {
            //collision with Max
            g_charles_movement.x = 1.0f;
            //randomize y component based on declared distribution
            g_charles_movement.y = distr(gen);
            g_charles_movement = glm::normalize(g_charles_movement);
        }
        if (lando_x_distance < 0.0f && lando_y_distance < 0.0f)
        {
            //collision with Lando
            g_charles_movement.x = -1.0f;
            //randomize y component based on declared distribution
            g_charles_movement.y = distr(gen);
            g_charles_movement = glm::normalize(g_charles_movement);
        }

        if (g_balls_in_play >= 2)
        {
            float max_x_distance2 = fabs(INIT_POS_MAX.x - g_charles2_position.x) -
                ((INIT_SCALE_CHARLES.x + INIT_SCALE_MAX.x) / 2.0f);

            float max_y_distance2 = fabs(g_max_position.y - g_charles2_position.y) -
                ((INIT_SCALE_CHARLES.y + INIT_SCALE_MAX.y) / 2.0f);

            float lando_x_distance2 = fabs(INIT_POS_LANDO.x - g_charles2_position.x) -
                ((INIT_SCALE_CHARLES.x + INIT_SCALE_LANDO.x) / 2.0f);

            float lando_y_distance2 = fabs(g_lando_position.y - g_charles2_position.y) -
                ((INIT_SCALE_CHARLES.y + INIT_SCALE_LANDO.y) / 2.0f);


            if (max_x_distance2 < 0.0f && max_y_distance2 < 0.0f)
            {
                //collision with Max
                g_charles2_movement.x = 1.0f;
                //randomize y component based on declared distribution
                g_charles2_movement.y = distr(gen);
                g_charles2_movement = glm::normalize(g_charles2_movement);
            }
            if (lando_x_distance2 < 0.0f && lando_y_distance2 < 0.0f)
            {
                //collision with Lando
                g_charles2_movement.x = -1.0f;
                //randomize y component based on declared distribution
                g_charles2_movement.y = distr(gen);
                g_charles2_movement = glm::normalize(g_charles2_movement);
            }
            if (g_balls_in_play == 3)
            {
                float max_x_distance3 = fabs(INIT_POS_MAX.x - g_charles3_position.x) -
                    ((INIT_SCALE_CHARLES.x + INIT_SCALE_MAX.x) / 2.0f);

                float max_y_distance3 = fabs(g_max_position.y - g_charles3_position.y) -
                    ((INIT_SCALE_CHARLES.y + INIT_SCALE_MAX.y) / 2.0f);

                float lando_x_distance3 = fabs(INIT_POS_LANDO.x - g_charles3_position.x) -
                    ((INIT_SCALE_CHARLES.x + INIT_SCALE_LANDO.x) / 2.0f);

                float lando_y_distance3 = fabs(g_lando_position.y - g_charles3_position.y) -
                    ((INIT_SCALE_CHARLES.y + INIT_SCALE_LANDO.y) / 2.0f);


                if (max_x_distance3 < 0.0f && max_y_distance3 < 0.0f)
                {
                    //collision with Max
                    g_charles3_movement.x = 1.0f;
                    //randomize y component based on declared distribution
                    g_charles3_movement.y = distr(gen);
                    g_charles3_movement = glm::normalize(g_charles3_movement);
                }
                if (lando_x_distance3 < 0.0f && lando_y_distance3 < 0.0f)
                {
                    //collision with Lando
                    g_charles3_movement.x = -1.0f;
                    //randomize y component based on declared distribution
                    g_charles3_movement.y = distr(gen);
                    g_charles3_movement = glm::normalize(g_charles3_movement);
                }
            }
        }
    }
}


void draw_object(glm::mat4& object_model_matrix, GLuint& object_texture_id)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so we use 6 instead of 3
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
    };

    // Textures
    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
    };

    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    //Bind Texture
    if(g_game_running)
    {
        // Bind texture
        draw_object(g_max_matrix, g_max_texture_id);
        draw_object(g_lando_matrix, g_lando_texture_id);
        draw_object(g_charles_matrix, g_charles_texture_id);
        if(g_balls_in_play >= 2)
        {
            draw_object(g_charles2_matrix, g_charles_texture_id);
            if(g_balls_in_play == 3)
            {
                draw_object(g_charles3_matrix, g_charles_texture_id);
            }
        }
    }
    else
    {
        if (g_max_is_winner)
        {
            draw_text(&g_shader_program, g_font_texture_id, "Max Verstappen", 0.4f, 0.05f,
                glm::vec3(-2.5f, 2.0f, 0.0f));
            draw_text(&g_shader_program, g_font_texture_id, "(P1) Wins!", 0.4f, 0.05f,
                glm::vec3(-2.0f, 1.0f, 0.0f));
        }
        else
        {
            draw_text(&g_shader_program, g_font_texture_id, "Lando Norris", 0.4f, 0.05f,
                glm::vec3(-2.5f, 2.0f, 0.0f));
            draw_text(&g_shader_program, g_font_texture_id, "(P2) Wins!", 0.4f, 0.05f,
                glm::vec3(-2.0f, 1.0f, 0.0f));
        }
    }

    // We disable two attribute arrays now
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() { SDL_Quit(); }


int main(int argc, char* argv[])
{
    initialise();

    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}