#include "Scene.h"

class LevelC : public Scene {
public:
    int ENEMY_COUNT = 10;
    int BACKGROUND_TILES = 7;

    ~LevelC();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
    void reset_level() override;
};

