#ifndef PLAYER_H
#define PLAYER_H


class Player
{
public:
    Player();

    int last_x;
    int last_y;

    bool OK;

    void SetLastPos(int x, int y);
};

#endif // PLAYER_H
