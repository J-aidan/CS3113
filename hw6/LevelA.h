#include "Scene.h"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 3;
    int BACKGROUND_TILES = 7;
    
    ~LevelA();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
    void reset_level() override;
};
