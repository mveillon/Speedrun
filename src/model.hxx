#pragma once

#include "player.hxx"
#include "board.hxx"
#include <ge211.hxx>

class Model
{
public:
    //constructs a Model
    //does not put the player at spawn - controller must do that to have access
    //to ge211::Random
    explicit Model(int map_number);

    Board  board;
    Player player;
    bool is_finished;
    bool next_map;
    int map_num;
    //frames per block - higher number = slower movement
    int player_fpb = 3;
    int mob_fpb = 6;
    int start_breaks = 2;
    int breaks = start_breaks;

    //moves the player, if allowed
    void move(bool mob_move);

    //checks if the player is on the same square as a mob
    bool on_mob();

    //breaks all blocks the player is touching
    void break_blocks();
private:
    //checks if the player is at the end
    bool at_end();

    //returns the vector with each component of length 1
    ge211::Dimensions direction(ge211::Dimensions raw);
};