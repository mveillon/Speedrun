#include "board.hxx"
#include <catch.hxx>

struct Board_access
{
    Board& b;

    Square get_square(int ind)
    { return b.squares_[ind]; }

    ge211::Dimensions get_dims()
    { return b.dims_; }

    int square_len()
    { return b.squares_.size(); }

    Block get_block(ge211::Position pos)
    { return b.get_block(pos); }

    int spawn_len()
    { return b.spawns.size(); }
};

TEST_CASE("Example")
{
    CHECK(1 + 1 == 2);
}

TEST_CASE("Constructor")
{
    Board board{1};
    struct Board_access b{board};

    CHECK(b.get_square(0).b_type == Block::air);
    CHECK(b.get_square(0).pos == ge211::Position(0, 0));
    CHECK(b.get_dims().width == 101);
    CHECK(b.get_dims().height == 101);
    CHECK(b.square_len() == 101 * 101);
    Square last = b.get_square(b.get_dims().width * b.get_dims().height - 1);
    CHECK(last.b_type == Block::air);
    CHECK(last.pos.x == 100);
    CHECK(last.pos.y == 100);
    CHECK(b.get_block({1, 92}) == Block::wall);
    CHECK(b.get_block({2, 92}) == Block::wall);
    CHECK(b.spawn_len() == 1);
}

TEST_CASE("Inequality")
{
    CHECK(ge211::Position(1, 0) < ge211::Position(0, 1));
    CHECK_FALSE(ge211::Position(50, 100) < ge211::Position(100, 50));
    CHECK(ge211::Position(71, 5) > ge211::Position(10000, 4));
    CHECK_FALSE(ge211::Position(0, 0) > ge211::Position(1, 1));
    CHECK(ge211::Position(1, 2) == ge211::Position(1, 2));
    CHECK_FALSE(ge211::Position(2, 1) == ge211::Position(1, 2));
    CHECK(ge211::Position(3, 4) != ge211::Position(6, 5));
    CHECK_FALSE(ge211::Position(10, 50) != ge211::Position(10, 50));
    CHECK_FALSE(ge211::Position(2, 1) > ge211::Position(2, 1));
    CHECK_FALSE(ge211::Position(3, 2) < ge211::Position(3, 2));

    Board board{1};
    struct Board_access b{board};

    CHECK(b.get_square(0).pos < b.get_square(1).pos);
    CHECK(b.get_square(0).pos < b.get_square(b.get_dims().width).pos);
    CHECK(b.get_square(b.get_dims().width).pos.y);
}

TEST_CASE("On board")
{
    Board b{1};

    CHECK(b.on_board(ge211::Position(0, 0)));
    CHECK(b.on_board(ge211::Position(100, 100)));
    CHECK_FALSE(b.on_board(ge211::Position(-1, 0)));
    CHECK_FALSE(b.on_board(ge211::Position(100, -1)));
    CHECK_FALSE(b.on_board(ge211::Position(101, 50)));

}

TEST_CASE("get_block")
{
    Board b{1};

    CHECK(b.get_block({49, 49}) == Block::air);
    CHECK(b.get_block({0, 0}) == Block::air);
    CHECK(b.get_block({100, 100}) == Block::air);
    CHECK(b.get_block({99, 100}) == Block::wall);
    CHECK(b.get_block({34, 77}) == Block::wall);
}