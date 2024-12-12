/**
* Author: Aidan Jacobs
* Assignment: Kessel Run
* Date due: 2024-12-11, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "Menu.h"
#include "Utility.h"

#define LEVEL_WIDTH 80
#define LEVEL_HEIGHT 8

GLuint m_font_texture_id2;
GLuint m_background_texture_id2;

unsigned int Menu_DATA[] =
{
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

Menu::~Menu()
{
    delete[] m_game_state.enemies;
    delete    m_game_state.player;
    delete[]   m_game_state.backgrounds;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void Menu::initialise()
{

    m_game_state.next_scene_id = -1;

    m_font_texture_id2 = Utility::load_texture("assets/font1.png");

    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, Menu_DATA, map_texture_id, 1.0f, 16, 7);



    GLuint message_texture_id = Utility::load_texture(MSGKESSEL_FILEPATH);

    m_game_state.player = new Entity(message_texture_id, 0.0f, 6.12f, 3.0f, PLAYER);
    m_game_state.player->set_scale(glm::vec3(6.12f, 3.0f, 0.0f));
    m_game_state.player->set_position(glm::vec3(5.0f, -4.0f, 0.0f));


    /**
    BG
    */
    m_background_texture_id2 = Utility::load_texture(BACKGROUND3_FILEPATH);
    m_game_state.backgrounds = new Entity(m_background_texture_id2, 10.0f, 10.0f, 1.0f, PLAYER);
    m_game_state.backgrounds->set_scale(glm::vec3(10.0f, 10.0f, 0.0f));
    //622, 360
    m_game_state.backgrounds->set_position(glm::vec3(5.0f, -2.5f, 0.0f));

    /**
    BGM and SFX
    */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/title.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    //Mix_VolumeMusic(0.0f);
    m_game_state.jump_sfx = Mix_LoadWAV("assets/jump_sound.wav");

}

void Menu::update(float delta_time)
{
    for (int i = 0; i < BACKGROUND_TILES; i++)
        m_game_state.backgrounds[i].update(delta_time, m_game_state.player, nullptr, 0, m_game_state.map);
    m_game_state.player->update(delta_time, m_game_state.player, nullptr, 0, m_game_state.map);
    if(game_started) m_game_state.next_scene_id = 1;
}

void Menu::render(ShaderProgram* program)
{
    for (int i = 0; i < BACKGROUND_TILES; i++)
        m_game_state.backgrounds[i].render(program);
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    Utility::draw_text(program, m_font_texture_id2, "Press A to Start", .4f, 0.0125f,
             glm::vec3(2.0f, -6.0f, 0.0f));
}
void Menu::reset_level()
{
    m_game_state.m_level_fail = false;
}
