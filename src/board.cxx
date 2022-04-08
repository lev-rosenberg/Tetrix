#include "board.hxx"
#include <ge211.hxx>
#include <vector>
#include <iostream>

using namespace ge211;
using Position = ge211::Posn<int>;
using Rectangle = ge211::Rect<int>;
using Position_set = std::vector<Position>;


Board::Board(Position_set stagnant_squares, int score)
        :stagnant_squares_(stagnant_squares),
        dims_({10,20}),
        score_(score)
{}

Position_set
Board::all_stagnant_squares()
{return stagnant_squares_;}

void
Board::update_stagnant_square(Position pos)
{
    stagnant_squares_.push_back(pos);
}

void
Board::add_score(int i)
{
    score_ += i;
}









