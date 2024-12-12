#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 6;
    int BACKGROUND_TILES = 7;

    ~LevelB();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
    void reset_level() override;
};
