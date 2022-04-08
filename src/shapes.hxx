#pragma once
#include "board.hxx"

using Block = ge211::Rect<int>;
using Position = ge211::Posn<int>;


class Square
{
public:
    Square(Position posn, bool stag);
    bool valid_space();
    bool sq_hits_bottom();
    Position position();
    bool stagnant();
private:
    bool stagnant_;
    Position posn_;
};

using Square_set = std::vector<Square>;

class Shape
{
public:
    Shape(int group, int orientation, Position center, Square_set squares);

    Shape make_L_shape(Position center, int orientation);
    Shape make_T_shape(Position center, int orientation);
    Shape make_line_shape(Position center, int orientation);
    Shape make_square_shape(Position center, int orientation);
    Shape make_shape(Position center, int orientation, int group);

    Shape rotate();
    Shape fall();
    Shape move_right();
    Shape move_left();

    bool hits_side();
    bool hits_bottom();
    bool hits_stag(std::vector<Position> stagnant_squares);
    Square_set squares();
    Position center();
    int orientation();
    void change_center(Position pos);

private:
    Square_set squares_;
    Position center_;
    int group_;
    int orientation_;
};

