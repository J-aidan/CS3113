/**
* Author: Aidan Jacobs
* Assignment: Kessel Run
* Date due: 2024-12-11, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 100
#define LEVEL_HEIGHT 8

constexpr char ENEMY_FILEPATH[]       = "assets/tie.png",
           FONTSHEET_FILEPATH[] = "assets/font1.png";

GLuint m_font_texture_id;

unsigned int LEVELA_DATA[] =
{
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  3,  0,  0,  0,  2,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

bool m_level_clear = false;

LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete [] m_game_state.backgrounds;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::initialise()
{
    m_game_state.m_level_fail = false;

    m_font_texture_id = Utility::load_texture(FONTSHEET_FILEPATH);

    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/asteroid_tiles.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 8, 3);
    
    // Code from main.cpp's initialise()
    /**
     Ship's Stuff
     */

    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    m_game_state.player = new Entity(player_texture_id, 5.0f, 2.94f, .59f, PLAYER); // Simpler constructor
    m_game_state.player->set_entity_type(PLAYER);

    m_game_state.player->set_scale(glm::vec3(2.94f, .59f, 0.0f));

    m_game_state.player->set_position(glm::vec3(-5.0f, -2.5f, 0.0f));


    // ����� ENEMIES ����� //
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] = Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY);
        m_game_state.enemies[i].set_entity_type(ENEMY);
        m_game_state.enemies[i].set_ai_state(IDLE);
        m_game_state.enemies[i].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
        m_game_state.enemies[i].set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    m_game_state.enemies[0].set_ai_type(FLYER);
    m_game_state.enemies[1].set_ai_type(FLYER);
    m_game_state.enemies[2].set_ai_type(FLYER);

    m_game_state.enemies[0].set_jumping_power(4.0f);
    m_game_state.enemies[1].set_jumping_power(4.0f);

    m_game_state.enemies[0].set_init_position(glm::vec3(40.0f, -2.5f, 0.0f));
    m_game_state.enemies[1].set_init_position(glm::vec3(60.0f, -4.5f, 0.0f));
    m_game_state.enemies[2].set_init_position(glm::vec3(80.0f, -3.0f, 0.0f));

    m_game_state.enemies[0].set_position(glm::vec3(40.0f, -2.5f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(60.0f, -4.5f, 0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(80.0f, -3.0f, 0.0f));


    /**
    BG
    */
    GLuint background_texture_id = Utility::load_texture(BACKGROUND4_FILEPATH);
    m_game_state.backgrounds = new Entity[BACKGROUND_TILES];
    for (int i = 0; i < BACKGROUND_TILES-1; i++)
    {
        m_game_state.backgrounds[i] = Entity(background_texture_id, 0.0f, 20.0f, 20.0f, BACKGROUND);
        m_game_state.backgrounds[i].set_scale(glm::vec3(20.0f, 20.0f, 0.0f));
        m_game_state.backgrounds[i].set_position(glm::vec3(5.0f + i * 20.0f, -4.0f, 0.0f));
    }
    background_texture_id = Utility::load_texture(MSGCLEAR_FILEPATH);
    m_game_state.backgrounds[BACKGROUND_TILES - 1] = Entity(background_texture_id, 0.0f, 20.0f, 20.0f, BACKGROUND);
    m_game_state.backgrounds[BACKGROUND_TILES - 1].set_scale(glm::vec3(6.64f, 1.0f, 0.0f));
    m_game_state.backgrounds[BACKGROUND_TILES - 1].set_position(glm::vec3(5.0f + (BACKGROUND_TILES-2)*20.0f, -4.0f, 0.0f));
;
    //1.6 to 1
    //622, 360

    /**
    BGM and SFX
    */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    m_game_state.bgm = Mix_LoadMUS("assets/title.mp3");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    //Mix_VolumeMusic(0.0f);
    m_game_state.jump_sfx = Mix_LoadWAV("assets/jump_sound.wav");
    m_game_state.pshoot_sfx = Mix_LoadWAV("assets/pblast.wav");
    m_game_state.tie_sfx = Mix_LoadWAV("assets/tie.wav");
    m_game_state.explosion_sfx = Mix_LoadWAV("assets/explosion.wav");
    
}

void LevelA::update(float delta_time)
{
    for (int i = 0; i < BACKGROUND_TILES; i++)
    {
        m_game_state.backgrounds[i].update(delta_time, m_game_state.player, nullptr, 0, m_game_state.map);
    }
    if (!(m_game_state.player->get_is_active()))
    {
        m_game_state.m_level_fail = true;
    }
  
    if(m_game_state.player->get_is_shooting())
    {
            
            m_game_state.player->set_shooting(false);

            GLuint bullet_texture_id = Utility::load_texture(PBULLET_FILEPATH);
            Entity* np = new Entity(bullet_texture_id, 6.0f, BULLET_WIDTH, BULLET_HEIGHT, PPROJECTILE); // Simpler constructor
            np->set_entity_type(PPROJECTILE);
            np->set_scale(glm::vec3(BULLET_WIDTH, BULLET_HEIGHT, 0.0f));
            np->set_position(glm::vec3(m_game_state.player->get_position().x, m_game_state.player->get_position().y + .25f, 0.0f));
            np->set_movement(glm::vec3(1.0, 0.0f, 0.0f));

            m_game_state.projectiles.push_back(np);

    }

    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);


    for (int i = 0; i < ENEMY_COUNT; i++)
        m_game_state.enemies[i].update(delta_time, m_game_state.player, nullptr, 0, m_game_state.map);
    for (int i = 0; i < m_game_state.projectiles.size(); i++)
        m_game_state.projectiles[i]->update(delta_time, m_game_state.player, nullptr, 0, m_game_state.map);
    for (int i = 0; i < m_game_state.projectiles.size(); i++)
    {
        for (int j = 0; j < ENEMY_COUNT; j++)
        {
            m_game_state.projectiles[i]->check_collision(&m_game_state.enemies[j]);
        }
    }
    if (m_game_state.player->get_position().x > 110.f) m_game_state.next_scene_id = 2;
}

void LevelA::render(ShaderProgram *program)
{
    
    for (int i = 0; i < BACKGROUND_TILES; i++)
        m_game_state.backgrounds[i].render(program);
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
        m_game_state.enemies[i].render(program);
    for (int i = 0; i < m_game_state.projectiles.size(); i++)
        m_game_state.projectiles[i]->render(program);

}

void LevelA::reset_level()
{
    m_game_state.player->activate();
    m_game_state.m_level_fail = false;

    m_game_state.player->set_position(glm::vec3(-5.0f, -3.75f, 0.0f));
    m_game_state.player->set_hp(3);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].activate();
        m_game_state.enemies[i].set_movement(glm::vec3(-1.0f, 0.0f, 0.0f));
        m_game_state.enemies[i].set_hp(3);
    }
    m_game_state.enemies[0].set_position(glm::vec3(40.0f, -2.5f, 0.0f));
    m_game_state.enemies[1].set_position(glm::vec3(60.0f, -4.5f, 0.0f));
    m_game_state.enemies[2].set_position(glm::vec3(80.0f, -3.0f, 0.0f));
}

