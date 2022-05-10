#include "view.hxx"

namespace defs {
int const               pix_len  = 25;
int const               board_z  = 0;
int const               mob_z    = 1;
int const               player_z = 2;
int const               txt_z    = 3;
int const               break_z  = 4;
ge211::Dimensions const window_dims{33, 33};
ge211::Dimensions const break_dims{pix_len * 4, pix_len * 2};
ge211::Color const      air_color{255, 255, 255};
ge211::Color const      spawn_color{100, 50, 10};
ge211::Color const      end_color{50, 100, 10};
ge211::Color const      mob_color{10, 200, 0};
ge211::Color const      break_color{0, 195, 198};
}

using namespace defs;

View::View(int map_number)
: wall_sprite1({pix_len, pix_len}, ge211::Color::black())
, wall_sprite2({pix_len, pix_len}, ge211::Color::white())
, spawn_sprite({pix_len, pix_len}, spawn_color)
, end_sprite({pix_len, pix_len}, end_color)
, air_sprite({pix_len, pix_len}, air_color)
, mob_sprite({pix_len, pix_len}, mob_color)
, break_sprite(break_dims, break_color)
, player_sprite0("player0.png")
, player_sprite1("player1.png")
, player_sprite2("player2.png")
, game_over("", ge211::Font{"gotham.ttf", 50})
, time("", ge211::Font{"gotham.ttf", 50})
{
    game_over.reconfigure(ge211::Text_sprite::Builder(
            ge211::Font{"gotham.ttf", 50})
                                  .color(ge211::Color::black()) << "You won!");

    //this is how View changes the colors and the dimensions
    //based on the map
    switch (map_number) {
    case 1:
        wall_sprite2.recolor({0, 100, 255});
        map_dims = {101, 101};
        break;
    case 2:
        wall_sprite2.recolor({132, 4, 4});
        map_dims = {101, 101};
        break;
    case 3:
        wall_sprite2.recolor({198, 149, 0});
        map_dims = {85, 93};
        break;
    }
}

void View::draw(ge211::Sprite_set& set, const Model& model, int frames)
{
    if (model.is_finished)
        set.add_sprite(game_over,
                       {window_dims.width * pix_len / 3,
                        window_dims.height * pix_len / 6},
                       txt_z);

    for (ge211::Position sq : in_view(model.player.pos)) {
        ge211::Position screen_pos = board_to_screen(sq,
                                                     model.player.pos);
        switch (model.board.get_block(sq)) {
        case Block::air:
            set.add_sprite(air_sprite, screen_pos, board_z);
            break;
        case Block::wall:
            if ((sq.x + sq.y) % 2) {
                set.add_sprite(wall_sprite1, screen_pos, board_z);
            } else {
                set.add_sprite(wall_sprite2, screen_pos, board_z);
            }
            break;
        case Block::spawn:
            set.add_sprite(spawn_sprite, screen_pos, board_z);
            break;
        case Block::end:
            set.add_sprite(end_sprite, screen_pos, board_z);
            break;
        case Block::unknown:
            throw std::runtime_error("View doesn't know what the block type "
                                     "is");
            break;
        }
    }

    for (Mob const mob : model.board.mobs) {
        if (is_in_view(mob.pos, model.player.pos)) {
            set.add_sprite(mob_sprite,
                           board_to_screen(mob.pos, model.player.pos),
                           mob_z);
        }
    }

    int             animation_len = 8;
    ge211::Position player_pos    = board_to_screen(model.player.pos,
                                                    model.player.pos);
    if (frames % (animation_len * 2) < animation_len &&
        !model.is_finished) {
        if (model.player.velo.width > 0)
            set.add_sprite(player_sprite1, player_pos, player_z);
        else if (model.player.velo.width < 0)
            set.add_sprite(player_sprite2, player_pos, player_z);
        else if (model.player.velo.height)
            set.add_sprite(player_sprite1, player_pos, player_z);
        else
            set.add_sprite(player_sprite0, player_pos, player_z);
    } else {
        set.add_sprite(player_sprite0, player_pos, player_z);
    }

    for (int x = 0; x < model.breaks; x++) {
        int real_x = (window_dims.width - 5) * pix_len -
                     x * (break_dims.width + pix_len);
        set.add_sprite(break_sprite,
                       {real_x, pix_len * 2 + pix_len / 5},
                       break_z);
    }

    int         seconds     = (frames / 60) % 60;
    int         minutes     = frames / 3600;
    int         miliseconds = (5 * frames / 3) % 100;
    std::string secs, mins, milis;
    if (seconds < 10)
        secs = "0" + std::to_string(seconds);
    else
        secs = std::to_string(seconds);
    if (minutes < 10)
        mins = "0" + std::to_string(minutes);
    else
        mins = std::to_string(minutes);
    if (miliseconds < 10)
        milis = "0" + std::to_string(miliseconds);
    else
        milis = std::to_string(miliseconds);
    time.reconfigure(ge211::Text_sprite::Builder(
            ge211::Font{"gotham.ttf", 50}).color(ge211::Color::medium_red()).
            message(mins + ":" + secs + "." + milis));
    set.add_sprite(time, {pix_len * 2, pix_len * 2}, break_z);
}

std::string View::initial_window_title() const
{
    return "Speedrun: The Game";
}

ge211::Dimensions View::initial_window_dimensions() const
{
    return {window_dims.width * pix_len,
            window_dims.height * pix_len};
}

ge211::Position View::board_to_screen(ge211::Position board, ge211::Position
player)
{
    ge211::Position top_left = {std::min(std::max(player.x -
                                window_dims.width / 2, 0),
    map_dims.width - window_dims.width),
                                std::min(std::max(player.y -
                                window_dims.height / 2, 0),
                                map_dims.height - window_dims.height)};

    return {(board.x - top_left.x) * pix_len,
            (board.y - top_left.y) * pix_len};

}

std::vector<ge211::Position> View::in_view(ge211::Position player_pos)
{
    int min_x = player_pos.x - 1 - window_dims.width / 2;
    int min_y = player_pos.y - 1 - window_dims.height / 2;
    int max_x = player_pos.x + 1 + window_dims.width / 2;
    int max_y = player_pos.y + 1 + window_dims.height / 2;
    if (min_x < 0) {
        min_x = 0;
        max_x = window_dims.width;
    } else if (max_x > map_dims.width) {
        max_x = map_dims.width;
        min_x = map_dims.width - window_dims.width;
    }
    if (min_y < 0) {
        min_y = 0;
        max_y = window_dims.height;
    } else if (max_y > map_dims.height) {
        max_y = map_dims.height;
        min_y = map_dims.height - window_dims.height;
    }

    std::vector<ge211::Position> res;
    for (int x = min_x; x < max_x; x++) {
        for (int y = min_y; y < max_y; y++) {
            res.push_back({x, y});
        }
    }
    return res;
}

bool View::is_in_view(ge211::Position board_pos, ge211::Position player_pos)
{
    int min_x = player_pos.x - 1 - window_dims.width / 2;
    int min_y = player_pos.y - 1 - window_dims.height / 2;
    int max_x = player_pos.x + 1 + window_dims.width / 2;
    int max_y = player_pos.y + 1 + window_dims.height / 2;
    if (min_x < 0) {
        min_x = 0;
        max_x = window_dims.width;
    } else if (max_x > map_dims.width) {
        max_x = map_dims.width;
        min_x = map_dims.width - window_dims.width;
    }
    if (min_y < 0) {
        min_y = 0;
        max_y = window_dims.height;
    } else if (max_y > map_dims.height) {
        max_y = map_dims.height;
        min_y = map_dims.height - window_dims.height;
    }

    return board_pos.x >= min_x &&
           board_pos.x < max_x &&
           board_pos.y >= min_y &&
           board_pos.y < max_y;
}