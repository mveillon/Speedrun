#include "controller.hxx"

Controller::Controller(int map_number)
: model_(map_number)
, view_(map_number)
, frames_(0)
{
    int spawn_len = model_.board.spawns.size();
    int num = get_random().up_to(spawn_len);
    model_.player.pos = model_.board.spawns[num];
}

void Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set, model_, frames_);
}

ge211::Dimensions Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

void Controller::on_key_down(ge211::Key key)
{
    no_velo_ = false;
    ge211::Dimensions* velo = &model_.player.velo;
    switch (key.code()) {
    case 'w':
        velo->height = -1;
        break;
    case 'a':
        velo->width = -1;
        break;
    case 's':
        velo->height = 1;
        break;
    case 'd':
        velo->width = 1;
        break;
    case 'q':
        quit();
        break;
    case ' ':
        if (model_.breaks)
            model_.break_blocks();
        break;
    }
}

void Controller::on_key_up(ge211::Key key)
{
    ge211::Dimensions* velo = &model_.player.velo;
    if (!no_velo_) {
        switch (key.code()) {
        case 'w':
            velo->height++;
            if (velo->height == -1)
                velo->height++;
            break;
        case 'a':
            velo->width++;
            if (velo->width == -1)
                velo->width++;
            break;
        case 's':
            velo->height--;
            if (velo->height == 1)
                velo->height--;
            break;
        case 'd':
            velo->width--;
            if (velo->width == 1)
                velo->width--;
            break;
        }
    }
}

void Controller::on_frame(double dt)
{
    if (frames_ % model_.player_fpb == 0) {
        bool mob_move = frames_ % model_.mob_fpb == 0;
        model_.move(mob_move);
        if (model_.next_map) {
            model_ = Model(model_.map_num + 1);
            view_  = View(model_.map_num);
            respawn();
        } else if (model_.on_mob()) {
            respawn();
        }
    }
    if (!model_.is_finished)
        frames_++;
}

void Controller::respawn()
{
    int spawn_len = model_.board.spawns.size();
    int num       = get_random().up_to(spawn_len);
    model_.player.pos = model_.board.spawns[num];
    model_.player.velo = {0, 0};
    no_velo_ = true;
}