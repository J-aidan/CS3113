/**
* Author: Aidan Jacobs
* Assignment: Kessel Run
* Date due: 2024-12-11, 2:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Scene.h"

void Scene::keep_player_onscreen(float camx)
{

    glm::vec3 curr_pos = m_game_state.player->get_position();
    glm::vec3 curr_scale = m_game_state.player->get_scale();
    camx = -camx;
    if (curr_pos.y > -(curr_scale.y / 2))
    {
        curr_pos.y = -(curr_scale.y / 2);
    }
    if (curr_pos.y < -7.5 + (curr_scale.y / 2))
    {
        curr_pos.y = -7.5 + (curr_scale.y / 2);
    }
    if (curr_pos.x < camx - 5.0f + (curr_scale.x / 2))
    {
        curr_pos.x = camx - 5.0f + (curr_scale.x / 2);
    }
    if (curr_pos.x > camx + 5.0f - (curr_scale.x / 2))
    {
        curr_pos.x = camx + 5.0f - (curr_scale.x / 2);
    }
    m_game_state.player->set_position(curr_pos);
}