#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

/**
    Notice that the game's state is now part of the Scene class, not the main file.
*/
struct GameState
{
    // ————— GAME OBJECTS ————— //
    Map *map;
    Entity *player;
    Entity *enemies;

    std::vector<Entity*> projectiles;
    std::vector<Entity*> eprojectiles;

    // ————— BACKGROUND ————— //
    Entity* backgrounds;
    
    // ————— AUDIO ————— //
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;
    Mix_Chunk *pshoot_sfx;
    Mix_Chunk *tie_sfx;
    Mix_Chunk *explosion_sfx;
    
    // ————— POINTERS TO OTHER SCENES ————— //
    int next_scene_id;

    //Booleann for keeping track of attempts
    bool m_level_fail = false;
};

class Scene {
protected:
    GameState m_game_state;
    
public:
    // ————— ATTRIBUTES ————— //
    int m_number_of_enemies = 3;
    bool game_started = false;
    
    // ————— METHODS ————— //
    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    virtual void reset_level() = 0;
    void keep_player_onscreen(float);

    void addProjectile(Entity* np) {m_game_state.projectiles.push_back(np); }
    
    // ————— GETTERS ————— //
    GameState const get_state() const { return m_game_state;             }
    int const get_number_of_enemies() const { return m_number_of_enemies; }
};
