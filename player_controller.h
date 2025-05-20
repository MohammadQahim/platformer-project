#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include"player.h"
#include "level.h"


class PlayerController
{
public:
    static PlayerController &get_instance()
    {
        static PlayerController instance;
        return instance;
    }

    PlayerController(const PlayerController &) = delete;
    PlayerController operator=(const PlayerController &) = delete;
    PlayerController(PlayerController &&) = delete;
    PlayerController operator=(PlayerController &&) = delete;

    void reset_player_stats();
    void increment_player_score();
    int  get_total_player_score() const;
    void spawn_player();
    void kill_player();
    void move_player_horizontally(float delta);
    void update_player_gravity();
    void update_player();


    Player& player1() {
        return player;
    }

    [[nodiscard]] const Player& player1() const {
        return player;
    }

    const int MAX_PLAYER_LIVES = 3;
    int player_lives = MAX_PLAYER_LIVES;


private:
    PlayerController() = default;
    ~PlayerController() = default;

    int player_level_scores[LEVEL_COUNT]{};
    Player player;

};

#endif
