#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include <vector>
#include "raylib.h"
#include "level.h"

class LevelManager {
public:
    static LevelManager &get_instance()
    {
        static LevelManager instance;
        return instance;
    }

    LevelManager(const LevelManager &) = delete;
    LevelManager operator=(const LevelManager &) = delete;
    LevelManager(LevelManager &&) = delete;
    LevelManager operator=(LevelManager &&) = delete;

    [[nodiscard]] std::vector<Level> get_levels() const {
        return levels;
    }

    bool is_inside_level(int row, int column);
    bool is_colliding(Vector2 pos, char look_for);
    char& get_collider(Vector2 pos, char look_for);
    void reset_level_index();
    void load_level(int offset);
    void unload_level();
    char& get_level_cell(size_t row, size_t column);
    void set_level_cell(size_t row, size_t column, char chr);

private:
    LevelManager() = default;
    ~LevelManager() = default;

    std::vector<Level> levels;

};

#endif //LEVEL_MANAGER_H
