#pragma once

#ifndef FINAL_PROJECT_VIEW_HXX
#define FINAL_PROJECT_VIEW_HXX

#endif //FINAL_PROJECT_VIEW_HXX

#include "model.hxx"
#include <ge211.hxx>
#include <vector>
#include <string>

class View
{
public:
    //constructs a View
    //every time you add a map, you have to change this function
    //to include the new wall color (s) and the new map_dims
    explicit View(int map_number);

    //adds all the sprites that need to be rendered to the Controller's
    //sprite set
    void draw(ge211::Sprite_set& set,
              Model const& model,
              int frames);

    //returns the inital title
    std::string initial_window_title() const;

    //returns the initial dimensions for the viewing window
    ge211::Dimensions initial_window_dimensions() const;

    //converts a position on the board to a position within
    //the viewing window and from there to a position on screen
    ge211::Position board_to_screen(ge211::Position board, ge211::Position
    player);

    ge211::Dimensions map_dims;
private:
    ge211::Rectangle_sprite break_sprite, wall_sprite1, wall_sprite2,
                            spawn_sprite, end_sprite, air_sprite, mob_sprite;
    ge211::Text_sprite game_over, time;

    //player_sprite0: standing still/alternating in run cycle
    //player_sprite1: moving right
    //player_sprite2: moving left
    ge211::Image_sprite player_sprite0, player_sprite1, player_sprite2;

    //returns all positions that are in the viewing window
    std::vector<ge211::Position> in_view(ge211::Position player_pos);

    //checks if a position is in view
    bool is_in_view(ge211::Position board_pos, ge211::Position player_pos);
};