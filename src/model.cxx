#include "model.hxx"
#include "shapes.hxx"
#include <iostream>

using Rectangle = Board::Rectangle;
using Position_set = Board::Position_set;

Model::Model()
        :falling_shape_(Shape(0,0,{5,-2},
                             {Square({4,-2},false),
                                      Square({5,-2},false),
                                      Square({6,-2},false),
                                      Square({5,-1},false)})),
        board_(Board({}, 0)),
        el_dt_(0),
        second(0)
{}


void
Model::make_stagnant()
{
    for (Square square : falling_shape_.squares()) {
        board_.update_stagnant_square(square.position());
    }
}
void
Model::on_frame(double dt)
{
    el_dt_ += dt;
    second += dt;
    if (second >= 1) {
        board_.add_score(1);
        second = 0;
    }
    if (el_dt_ >= 0.25) {
        el_dt_ = 0;
        Shape maybe = falling_shape_.fall();
        if (maybe.hits_bottom()) {
            make_stagnant();
            ge211::Random_source<int> group(0, 3);
            int group_val = group.next();
            ge211::Random_source<int> orientation(0, 3);
            int ori_val = orientation.next();
            falling_shape_ =
                    falling_shape_.make_shape({5, -2}, ori_val, group_val);
        }
        else if (maybe.hits_stag(board_.all_stagnant_squares())) {
            make_stagnant();
            ge211::Random_source<int> group(0, 3);
            int group_val = group.next();
            ge211::Random_source<int> orientation(0, 3);
            int ori_val = orientation.next();
            falling_shape_ =
                    falling_shape_.make_shape({5, -2}, ori_val, group_val);
        }
        else {
            falling_shape_ = maybe;
        }
    }
    for (int i = 0; i < board().dimensions().width; i++) {
        for (int j = 0; j < board().dimensions().height; j++){
            if (full_row(j, board_.stagnant_squares_)){
                board_.stagnant_squares_ = reset_stagnant(j, board_
                .stagnant_squares_);
                board_.add_score(25);
            }
        }
    }
}

Position_set
Model::reset_stagnant(int row, Board::Position_set& stagnant_squares)
{
    Board::Position_set set = {};
    for (Position pos : stagnant_squares) {
        if (pos.y != row) {
            if (pos.y < row) {
                Position npos = {pos.x, pos.y + 1};
                set.push_back(npos);
            }
            else {
                set.push_back(pos);
            }
        }
    }
    return set;
}


bool
Model::full_row(int row, Board::Position_set& stagnant_squares)
{
    int counter = 0;
    Board::Position_set set = {};
    for (Position pos : stagnant_squares) {
        if (pos.y == row) {
            counter += 1;
        }
    }
    if (counter == 10) {
        return true;
    }
    return false;
}
void
Model::change_center(Position pos) {
    falling_shape_.change_center(pos);
}