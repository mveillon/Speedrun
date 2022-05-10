#include "board.hxx"

Board::Board(int map_number)
{
    std::string path = "../map generation/map" +
            std::to_string(map_number) + ".csv";
    std::ifstream map(path);
    if (!map.is_open()) throw std::runtime_error("Could not open map");

    std::string line;
    int         x = 0;
    int         y = 0;
    while (std::getline(map, line)) {
        std::string current = "";
        x = 0;
        for (int c = 0; c < line.length(); c++) {
            bool is_end = c == line.length() - 1;
            if (is_end) {
                current += line[c];
            }
            if (line[c] == ',' || is_end) {
                Block b_type = str_to_block(current);
                if (current == "mob") {
                    mobs.push_back(Mob(ge211::Position(x, y)));
                }
                if (b_type == Block::spawn) {
                    spawns.push_back({x, y});
                }
                squares_.push_back(Square(b_type,
                                          ge211::Position(x, y)));
                current = "";
                x++;
            } else {
                current += line[c];
            }
        }
        y++;
    }
    int width = squares_.size() / y;
    dims_         = {width, y};
    map.close();
}

Block Board::get_block(ge211::Position const pos) const
{
    int ind = bsearch_helper(0,
                             dims_.width * dims_.height - 1,
                             pos);
    if (ind == -1) {
        return Block::unknown;
    }
    return squares_[ind].b_type;
}

bool Board::on_board(ge211::Position const pos) const
{
    return pos.x >= 0 &&
           pos.y >= 0 &&
           pos.x < dims_.width &&
           pos.y < dims_.height;
}

Block str_to_block(std::string const txt)
{
    Block res;
    if (txt == "air" || txt == "mob") {
        return Block::air;
    } else if (txt == "wall") {
        return Block::wall;
    } else if (txt == "spawn") {
        return Block::spawn;
    } else if (txt == "end") {
        return Block::end;
    } else {
        throw std::runtime_error("Unclassified block in map.");
    }
}

bool operator >(ge211::Position pos1, ge211::Position pos2)
{
    if (pos1.y == pos2.y) {
        return pos1.x > pos2.x;
    }
    return pos1.y > pos2.y;
}

bool operator <(ge211::Position pos1, ge211::Position pos2)
{
    return !(pos1 > pos2) && pos1 != pos2;
}

bool operator ==(ge211::Position pos1, ge211::Position pos2)
{
    return pos1.x == pos2.x && pos1.y == pos2.y;
}

bool operator !=(ge211::Position pos1, ge211::Position pos2)
{
    return !(pos1 == pos2);
}

int Board::bsearch_helper(int low, int high, ge211::Position target) const
{
    if (low > high)
        return -1;
    int mid = (low + high) / 2;
    if (squares_[mid].pos == target)
        return mid;
    if (squares_[mid].pos < target)
        return bsearch_helper(mid + 1,
                              high,
                              target);
    return bsearch_helper(low, mid - 1, target);
}

void Board::break_block(ge211::Position pos)
{
    int ind = bsearch_helper(0,
                             dims_.width * dims_.height - 1,
                             pos);
    if (ind == -1)
        throw std::runtime_error("break_block could not find the block");
    squares_[ind].b_type = Block::air;
}