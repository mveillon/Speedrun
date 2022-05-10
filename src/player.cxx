#include "player.hxx"

Player::Player()
: velo{0, 0}
, pos{0, 0}
{}

ge211::Position Player::next() const
{ return pos + velo; }