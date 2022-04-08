#pragma once

#include <ge211.hxx>
#include "board.hxx"
#include "shapes.hxx"

    using Dimensions = Board::Dimensions;
    using Position = Board::Position;
    using Rectangle = Board::Rectangle;
    using Position_set = Board::Position_set;

class Model
{
public:
    Model();
    void on_frame(double dt);
    Board board() const
    {return board_;}
    Shape falling_shape_;
    void make_stagnant();
    Board::Position_set reset_stagnant(int row, Board::Position_set& stag_sq);
    bool full_row(int row, Board::Position_set& stag_sq);
    void change_center(Position pos);
private:
    Board board_;
    double el_dt_;
    double second;
};
