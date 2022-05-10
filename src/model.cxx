#include "model.hxx"

int MAX_MAP = 3;

Model::Model(int map_number)
: board(map_number)
, is_finished(false)
, player()
, map_num(map_number)
, next_map(false)
{}

void Model::move(bool mob_move)
{
    bool has_moved = false;
    ge211::Dimensions direc = direction(player.velo);
    ge211::Position   next  = player.next();
    while (!is_finished &&
           player.pos != next &&
           board.on_board(player.pos + direc) &&
           board.get_block(player.pos + direc) != Block::wall) {
        player.pos += direc;
        has_moved = true;
        if (at_end()) {
            if (map_num < MAX_MAP) {
                next_map = true;
            } else {
                is_finished = true;
            }
        }
    }
    if (!is_finished) {
        if (!has_moved && !board.on_board(player.next())) {
            ge211::Dimensions const velo = player.velo;
            ge211::Dimensions const dims = board.get_dims();
            if (velo.width && velo.height) {
                player.pos = {dims.width - player.pos.x - 1,
                              dims.height - player.pos.y - 1};
            } else if (velo.width) {
                player.pos.x = dims.width - player.pos.x - 1;
            } else if (velo.height) {
                player.pos.y = dims.height - player.pos.y - 1;
            }
        }

        if (mob_move) {
            for (int i = 0; i < board.mobs.size(); i++) {
                if (board.get_block(board.mobs[i].next()) == Block::wall) {
                    board.mobs[i].change_direc();
                    board.mobs[i].pos = board.mobs[i].next();
                } else {
                    board.mobs[i].pos = board.mobs[i].next();
                }
            }
        }
    }
}

bool Model::at_end()
{ return board.get_block(player.pos) == Block::end; }

bool Model::on_mob()
{
    for (Mob const mob : board.mobs) {
        if (mob.pos == player.pos) {
            return true;
        }
    }
    return false;
}

void Model::break_blocks()
{
    bool                           found = false;
    std::vector<ge211::Dimensions> vects = {{1,  0},
                                            {0,  1},
                                            {-1, 0},
                                            {0,  -1}};
    for (ge211::Dimensions         direc : vects) {
        if (board.get_block(player.pos + direc) == Block::wall) {
            board.break_block(player.pos + direc);
            found = true;
        }
    }
    if (found)
        breaks--;
}

ge211::Dimensions Model::direction(ge211::Dimensions raw)
{
    return {raw.width / std::max(std::abs(raw.width), 1),
            raw.height / std::max(std::abs(raw.height), 1)};
}