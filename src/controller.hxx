#pragma once

#ifndef FINAL_PROJECT_CONTROLLER_HXX
#define FINAL_PROJECT_CONTROLLER_HXX

#endif //FINAL_PROJECT_CONTROLLER_HXX

#include "view.hxx"
#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    explicit Controller(int map_number);
protected:
    //calls view_.draw()
    void draw(ge211::Sprite_set& set) override;

    //makes the player start moving
    void on_key_down(ge211::Key key) override;

    //makes the player stop moving
    void on_key_up(ge211::Key key) override;

    //calls view_.initial_window_dimensions()
    ge211::Dimensions initial_window_dimensions() const override;

    //calls view_.initial_window_title()
    std::string initial_window_title() const override;

    //actually moves the player and increments the total frame count
    void on_frame(double dt) override;
private:
    Model model_;
    View  view_;
    bool no_velo_ = false;

    //respawns the player
    void respawn();

    long frames_;
};