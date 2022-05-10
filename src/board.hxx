#pragma once

#ifndef FINAL_PROJECT_BOARD_HXX
#define FINAL_PROJECT_BOARD_HXX

#endif //FINAL_PROJECT_BOARD_HXX

#include "block.hxx"
#include "mob.hxx"
#include <ge211.hxx>
#include <vector>
#include <string>
#include <fstream>

class Board
{
public:

    //constructs a Board
    explicit Board(int map_number);

    //returns the block type of the given position
    //using binary search
    Block get_block(ge211::Position const pos) const;

    //checks if a position is on the board
    bool on_board(ge211::Position const pos) const;

    std::vector<ge211::Position> spawns;

    friend struct Board_access;

    //returns the board's dimensions
    ge211::Dimensions get_dims()
    { return dims_; }

    //breaks a block
    void break_block(ge211::Position pos);

    std::vector<Mob> mobs;
private:
    ge211::Dimensions dims_;
    std::vector<Square> squares_;

    //recursive helper to the binary search
    //returns -1 if the position is not found
    int bsearch_helper(int low, int high, ge211::Position target) const;
};

//compares two positions by their y-values, then their x-values
//(1, 0) > (0, 1)
bool operator <(ge211::Position pos1, ge211::Position pos2);
bool operator >(ge211::Position pos1, ge211::Position pos2);
bool operator ==(ge211::Position pos1, ge211::Position pos2);
bool operator !=(ge211::Position pos1, ge211::Position pos2);

Block str_to_block(std::string const txt);