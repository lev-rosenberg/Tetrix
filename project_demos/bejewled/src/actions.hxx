#pragma once

#include "tile.hxx"

class Horizontal_lazer_action : public Tile::Action
{
public:
    Horizontal_lazer_action()
            : Action("-")
    { }

protected:
    Position_set apply(Position, const Board&) const override;
};

