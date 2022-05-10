#pragma once

#ifndef FINAL_PROJECT_PLAYER_HXX
#define FINAL_PROJECT_PLAYER_HXX

#endif //FINAL_PROJECT_PLAYER_HXX

#include <ge211.hxx>

class Player
{
public:

    //constructs a Player
    explicit Player();

    ge211::Position pos;
    ge211::Dimensions velo;

    //computes where the player would move next
    ge211::Position next() const;
};