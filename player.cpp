#include "player.h"

Player::Player()
{
    last_x = 0;
    last_y = 0;
    OK = true;
}

void Player::SetLastPos(int x, int y)
{
    this->last_x = x;
    this->last_y = y;
}
