#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

//--GLOBALS FOR LEVELS---------------------------------------------

constexpr static char SPRITESHEET_FILEPATH[] = "assets/falcon.png";

constexpr static char BACKGROUND1_FILEPATH[] = "assets/otherstars.jpg";
constexpr static char BACKGROUND2_FILEPATH[] = "assets/sumstars.jpg";
constexpr static char BACKGROUND3_FILEPATH[] = "assets/cstars1.png";
constexpr static char BACKGROUND4_FILEPATH[] = "assets/cstars2.png";

constexpr static char MSGKESSEL_FILEPATH[] = "assets/kesselrun.png";
constexpr static char MSGCLEAR_FILEPATH[] = "assets/clear.png";

constexpr static char PBULLET_FILEPATH[] = "assets/redbullet.png";
constexpr static char EBULLET_FILEPATH[] = "assets/ebullet.png";

constexpr static float BULLET_WIDTH = 37.0f / 100.0f;
constexpr static float BULLET_HEIGHT = 8.0f / 100.0f;




//=================================================================

class Utility {
public:
    // ————— METHODS ————— //
    static GLuint load_texture(const char* filepath);
    static void draw_text(ShaderProgram *program, GLuint font_texture_id, std::string text, float screen_size, float spacing, glm::vec3 position);

};
