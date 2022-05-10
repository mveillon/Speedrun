#pragma once

#ifndef FINAL_PROJECT_BLOCK_HXX
#define FINAL_PROJECT_BLOCK_HXX

#endif //FINAL_PROJECT_BLOCK_HXX

#include <ge211.hxx>

enum class Block
{
    air,
    wall,
    spawn,
    end,
    unknown
};

class Square
{
public:
    explicit Square(Block b_type, ge211::Position pos);

    Block b_type;
    ge211::Position pos;
};