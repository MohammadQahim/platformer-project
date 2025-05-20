#include "level_manager.h"
#include "level.h"
#include "globals.h"
#include "player_controller.h"
#include "enemies_controller.h"

#include <fstream>  //  for file reading
#include <sstream>  //  for parsing
#include <string>
#include <stdexcept>

bool LevelManager::is_inside_level(int row, int column) {
    if (row < 0 || row >= current_level.get_rows()) return false;
    if (column < 0 || column >= current_level.get_columns()) return false;
    return true;
}

bool LevelManager::is_colliding(Vector2 pos, char look_for) {
    Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

char& LevelManager::get_collider(Vector2 pos, char look_for) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

    for (int row = pos.y - 1; row < pos.y + 1; ++row) {
        for (int column = pos.x - 1; column < pos.x + 1; ++column) {
            // Check if the cell is out-of-bounds
            if (!is_inside_level(row, column)) continue;
            if (get_level_cell(row, column) == look_for) {
                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return get_level_cell(row, column);
                }
            }
        }
    }

    // If failed, get an approximation
    return get_level_cell(pos.x, pos.y);;
}

void LevelManager::reset_level_index() {
    level_index = 0;
}

void LevelManager::load_level(int offset) {
    level_index += offset;

    try {
        std::ifstream file("data/levels.rll");
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open levels.rll file.");
        }

        int level_count = 0;
        std::string line;
        std::string level_line;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == ';') continue;
            if (level_count == level_index) {
                level_line = line;
                break;
            }
            level_count++;
        }

        file.close();

        if (level_line.empty()) {
            throw std::runtime_error("Level line not found for current index.");
        }

        std::vector<std::vector<char>> level_rows;
        std::vector<char> current_row;
        size_t max_columns = 0;
        size_t pos = 0;

        while (pos < level_line.size()) {
            if (level_line[pos] == '|') {
                level_rows.push_back(current_row);
                if (current_row.size() > max_columns) max_columns = current_row.size();
                current_row.clear();
                pos++;
                continue;
            }

            if (isdigit(level_line[pos])) {
                int run_length = 0;
                while (pos < level_line.size() && isdigit(level_line[pos])) {
                    run_length = run_length * 10 + (level_line[pos] - '0');
                    pos++;
                }
                if (pos >= level_line.size()) throw std::runtime_error("Malformed RLE encoding in level data.");
                char tile = level_line[pos++];
                for (int i = 0; i < run_length; ++i) {
                    current_row.push_back(tile);
                }
            } else {
                current_row.push_back(level_line[pos++]);
            }
        }

        if (!current_row.empty()) {
            level_rows.push_back(current_row);
            if (current_row.size() > max_columns) max_columns = current_row.size();
        }

        if (level_rows.empty()) {
            throw std::runtime_error("Parsed level is empty.");
        }

        if (current_level_data) {
            delete[] current_level_data;
        }

        size_t total_rows = level_rows.size();
        current_level_data = new char[total_rows * max_columns];

        for (size_t r = 0; r < total_rows; r++) {
            for (size_t c = 0; c < max_columns; c++) {
                current_level_data[r * max_columns + c] = (c < level_rows[r].size()) ? level_rows[r][c] : '-';
            }
        }

        current_level.set_rows(total_rows);
        current_level.set_columns(max_columns);
        current_level.set_data(current_level_data);

        PlayerController::get_instance().spawn_player();
        EnemiesController::get_instance().spawn_enemies();
        derive_graphics_metrics_from_loaded_level();
        timer = MAX_LEVEL_TIME;
    }
    catch (const std::exception& e) {
        TraceLog(LOG_ERROR, TextFormat("Level loading failed: %s", e.what()));
        game_state = VICTORY_STATE;
        create_victory_menu_background();
        level_index = 0;
    }
}

void LevelManager::unload_level() {
    delete[] current_level_data;
}

// Getters and setters
char& LevelManager::get_level_cell(size_t row, size_t column) {
    return current_level.get_data()[row * current_level.get_columns() + column];
}

void LevelManager::set_level_cell(size_t row, size_t column, char chr) {
    get_level_cell(row, column) = chr;
}


