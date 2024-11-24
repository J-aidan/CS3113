/**
* Author: Aidan Jacobs
* Assignment: Platformer
* Date due: 2024-11-23, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 80
#define LEVEL_HEIGHT 8


GLuint m_font_texture_id3;

unsigned int LevelB_DATA[] =
{
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 59,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0, 18, 18, 18, 18,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 75,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,  0,  0,  0,  0,  0, 34,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0, 75,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  0,  0,  0,  0,  0,  0, 34, 18, 18,  0,  0,  0, 34,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 75,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0,  0,  4,  5,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34,
     0,  0,  0, 18,  18, 18, 0,  0,  0,  0,  0,  0,  0, 18, 18, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18, 18, 18,  0,  0, 75,  0,  0, 17, 18, 18,  0,  0,  0,  0,  0,  0,  0,  0, 34, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18,  0,  0,  0,  0,  0, 18, 18, 18, 34,
     0,  0,  0, 34,  34, 34, 0,  0,  0,  0,  0,  0,  0, 34, 34, 34,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 18, 18, 34, 34, 34,  0,  0, 75,  0,  0, 33, 34, 34,  0,  0,  0,  0,  0,  0,  0,  0, 34, 34, 18,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 34, 18, 18, 18, 18, 18, 34, 34, 34, 34
};

bool m_levelB_done = false;
bool m_levelB_clear = false;

LevelB::~LevelB()
{
    delete[] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise()
{
    m_game_state.m_level_fail = false;

    m_font_texture_id3 = Utility::load_texture("assets/font1.png");

    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LevelB_DATA, map_texture_id, 1.0f, 16, 7);

    // Code from main.cpp's initialise()
    /**
     Pirate's Stuff
     */
     // Existing
    int player_walking_animation[2][14] =
    {
        { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26 },  // for pirate to move to the left,
        { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27 }   // for pirate to move to the right,
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);

    GLuint player_texture_id = Utility::load_texture("assets/pirate.png");

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        4.5f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        14,                         // animation frame amount
        0,                         // current animation index
        2,                         // animation column amount
        14,                         // animation row amount
        1.0f,                      // width
        1.0f,                       // height
        PLAYER
    );

    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));


    // ––––– ENEMIES ––––– //
    GLuint enemy_texture_id = Utility::load_texture("assets/captain.png");

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] = Entity(enemy_texture_id, 1.0f, .5f, 1.0f, ENEMY);
        m_game_state.enemies[i].set_entity_type(ENEMY);
        m_game_state.enemies[i].set_ai_state(IDLE);
        m_game_state.enemies[i].set_movement(glm::vec3(0.0f));
        m_game_state.enemies[i].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    }

    m_game_state.enemies[0].set_ai_type(WALKER);
    m_game_state.enemies[1].set_ai_type(JUMPER);
    m_game_state.enemies[2].set_ai_type(JUMPER);

    m_game_state.enemies[1].set_jumping_power(4.0f);
    m_game_state.enemies[2].set_jumping_power(4.0f);

    m_game_state.enemies[0].set_position(glm::vec3(60.0f, 0.0f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(71.0f, -4.0f, 0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(74.0f, 0.0f, 0.0f));

    /**
    BGM and SFX
    */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/bgm2.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    //Mix_VolumeMusic(0.0f);
    m_game_state.jump_sfx = Mix_LoadWAV("assets/jump_sound.wav");

}

void LevelB::update(float delta_time)
{
    if (!(m_game_state.player->get_is_active()))
    {
        m_game_state.m_level_fail = true;
    }
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    for (int i = 0; i < ENEMY_COUNT; i++)
        m_game_state.enemies[i].update(delta_time, m_game_state.player, nullptr, 0, m_game_state.map);
    if (m_game_state.player->get_position().y < -10.0f && m_game_state.player->get_position().x < 80.f) m_game_state.m_level_fail = true;
    if (m_game_state.player->get_position().y < -10.0f && m_game_state.player->get_position().x > 80.f) m_game_state.next_scene_id = 3;
}

void LevelB::render(ShaderProgram* program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
        m_game_state.enemies[i].render(program);
    Utility::draw_text(program, m_font_texture_id3, "Leap!", .4f, 0.0125f,
        glm::vec3(80.0f, -4.0f, 0.0f));


}

void LevelB::reset_level()
{
    m_game_state.player->activate();
    m_game_state.m_level_fail = false;

    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));


    m_game_state.enemies[0].activate();
    m_game_state.enemies[1].activate();
    m_game_state.enemies[2].activate();
    m_game_state.enemies[0].set_position(glm::vec3(60.0f, 0.0f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(71.0f, -4.0f, 0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(74.0f, 0.0f, 0.0f));

    /**
    BGM and SFX
    */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/bgm2.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    //Mix_VolumeMusic(0.0f);
    m_game_state.jump_sfx = Mix_LoadWAV("assets/jump_sound.wav");
}