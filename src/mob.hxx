#pragma once

#ifndef SPEEDRUN_MOB_HXX
#define SPEEDRUN_MOB_HXX

#endif //SPEEDRUN_MOB_HXX

#include <ge211.hxx>

class Mob
{
public:
    //constructs a mob
    explicit Mob(ge211::Position initial_pos);

    ge211::Position   pos;
    ge211::Dimensions velo;

    //changes the mob's direction
    void change_direc()
    { velo.width *= -1; }

    //returns where the mob would be after its next move
    ge211::Position next()
    { return pos + velo; }
};

