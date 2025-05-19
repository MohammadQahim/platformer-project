#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"
#include "enemies_controller.h"


class Player
{
public:
    Player(const float player_y_velocity, const Vector2 &player_pos, const bool player_on_ground,
       const bool looking_forward, const bool moving)
    : player_y_velocity(player_y_velocity),
      player_pos(player_pos),
      player_on_ground(player_on_ground),
      looking_forward(looking_forward),
      moving(moving) { }

    Player() = default;

    [[nodiscard]] float get_player_y_velocity() const {
        return player_y_velocity;
    }

    [[nodiscard]] Vector2 get_player_pos() const {
        return player_pos;
    }

    void set_player_y_velocity(const float player_y_velocity) {
        this->player_y_velocity = player_y_velocity;
    }

    void set_player_pos(const Vector2 &player_pos) {
        this->player_pos = player_pos;
    }

    void set_player_x(float x) {
        player_pos.x = x;
    }

    void set_player_y(float y) {
        player_pos.y = y;
    }

    [[nodiscard]] bool is_player_on_ground() const {
        return player_on_ground;
    }

    [[nodiscard]] bool is_looking_forward() const {
        return looking_forward;
    }

    [[nodiscard]] bool is_moving() const {
        return moving;
    }

    void set_player_on_ground(bool on_ground) {
        player_on_ground = on_ground;
    }

    void set_looking_forward(bool forward) {
        looking_forward = forward;
    }

    void set_moving(bool is_moving) {
        moving = is_moving;
    }

private:
    float player_y_velocity = 0;
    Vector2 player_pos;
    bool player_on_ground;
    bool looking_forward;
    bool moving;

};

#endif //PLAYER_H
