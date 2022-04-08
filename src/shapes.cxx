#include "board.hxx"
#include "shapes.hxx"

using namespace ge211;
using Position = ge211::Posn<int>;

////// Square Things //////

Square::Square(Position posn, bool stag)
        : stagnant_(stag),
        posn_{posn}
{ }



bool
Square::sq_hits_bottom()
{
    if (posn_.y > 19) {
        return true;
    }
    return false;
}

bool
Square::valid_space()
{
    if (posn_.x <= 0) {
        return false;
    }
    if (posn_.x > 10) {
        return false;
    }
    return true;
}


Position
Square::position()
{return posn_;}

bool
Square::stagnant()
{return stagnant_;}

////// Shape Things //////


Shape::Shape(int group, int orientation, Position center, Square_set squares)
        : squares_(squares),
        center_{center},
        group_(group),
        orientation_{orientation}

{}

Shape
Shape::make_T_shape(Position center, int orientation)
{
    if (orientation == 0) {
        Square l_side = Square({center.x-1, center.y}, false);
        Square middle = Square({center}, false);
        Square r_side = Square({center.x+1, center.y}, false);
        Square bottom = Square({center.x, center.y+1}, false);
        return Shape(0, orientation, center, {l_side, middle, r_side, bottom});
    }
    else if (orientation == 1) {
        Square top_side = Square({center.x, center.y-1}, false);
        Square middle = Square({center}, false);
        Square bottom_side = Square({center.x, center.y+1}, false);
        Square left = Square({center.x-1, center.y}, false);
        return Shape(0, orientation, center, {top_side, middle, bottom_side,
                                              left});
    }
    else if (orientation == 2) {
        Square l_side = Square({center.x-1, center.y}, false);
        Square middle = Square({center}, false);
        Square r_side = Square({center.x+1, center.y}, false);
        Square top = Square({center.x, center.y-1}, false);
        return Shape(0, orientation, center, {l_side, middle, r_side, top});
    }
    else {
        Square top_side = Square({center.x, center.y-1}, false);
        Square middle = Square({center}, false);
        Square bottom_side = Square({center.x, center.y+1}, false);
        Square right = Square({center.x+1, center.y}, false);
        return Shape(0, orientation, center, {top_side, middle, bottom_side, right});
    }
}

Shape
Shape::make_L_shape(Position center, int orientation)
{
    if (orientation == 0) {
        Square top = Square({center.x, center.y-1}, false);
        Square middle = Square({center}, false);
        Square bottom = Square({center.x, center.y+1}, false);
        Square xtra = Square({center.x+1, center.y+1}, false);
        return Shape(1, orientation, center, {top, middle, bottom, xtra});
    }
    else if (orientation == 1) {
        Square right = Square({center.x+1, center.y}, false);
        Square middle = Square({center}, false);
        Square bottom = Square({center.x-1, center.y}, false);
        Square xtra = Square({center.x-1, center.y+1}, false);
        return Shape(1, orientation, center, {right, middle, bottom, xtra});
    }
    else if (orientation == 2) {
        Square top = Square({center.x, center.y-1}, false);
        Square middle = Square({center}, false);
        Square bottom = Square({center.x, center.y+1}, false);
        Square xtra = Square({center.x-1, center.y-1}, false);
        return Shape(1, orientation, center, {top, middle, bottom, xtra});
    }
    else {
        Square left = Square({center.x-1, center.y}, false);
        Square middle = Square({center}, false);
        Square right = Square({center.x+1, center.y}, false);
        Square xtra = Square({center.x+1, center.y-1}, false);
        return Shape(1, orientation, center, {left, middle, right, xtra});
    }
}

Shape
Shape::make_line_shape(Position center, int orientation)
{
    if ((orientation == 0) or (orientation == 2)) {
        Square top = Square({center.x, center.y+1}, false);
        Square middle = Square({center}, false);
        Square bottom = Square({center.x, center.y-1}, false);
        return Shape(2, orientation, center, {top, middle, bottom});
    }
    else {
        Square left = Square({center.x+1, center.y}, false);
        Square middle = Square({center}, false);
        Square right = Square({center.x-1, center.y}, false);
        return Shape(2, orientation, center, {left, middle, right});
    }
}

Shape
Shape::make_square_shape(Position center, int orientation)
{
    Square top_left = Square({center}, false);
    Square top_right = Square({center.x+1, center.y}, false);
    Square bottom_left = Square({center.x, center.y+1}, false);
    Square bottom_right = Square({center.x+1, center.y+1}, false);
    return Shape(3, orientation, center, {top_left, top_right, bottom_left, bottom_right});
}
Shape
Shape::make_shape(Position center, int orientation, int group)
{
    if (group == 0) {
        return make_T_shape(center, orientation);
    }
    else if (group == 1) {
        return make_L_shape(center, orientation);
    }
    else if (group == 2) {
        return make_line_shape(center, orientation);
    }
    else {
        return make_square_shape(center, orientation);
    }
}

Shape
Shape::rotate()
{
    if (orientation_ == 0) {
        Shape maybe = make_shape({center_.x, center_.y}, 1, group_);
        if (not maybe.hits_side()) {
            return make_shape(center_, 1, group_);
        }
        else {
            return make_shape(center_, 0, group_);
        }
    }
    else if (orientation_ == 1) {
        Shape maybe = make_shape({center_.x, center_.y}, 2, group_);
        if (not maybe.hits_side()) {
            return make_shape(center_, 2, group_);
        }
        else {
            return make_shape(center_, 1, group_);
        }
    }
    else if (orientation_ == 2) {
        Shape maybe = make_shape({center_.x, center_.y}, 3, group_);
        if (not maybe.hits_side()) {
            return make_shape(center_, 3, group_);
        }
        else {
            return make_shape(center_, 2, group_);
        }
    }
    else {
        Shape maybe = make_shape({center_.x, center_.y}, 0, group_);
        if (not maybe.hits_side()) {
            return make_shape(center_, 0, group_);
        }
        else {
            return make_shape(center_, 3, group_);
        }
    }
}

bool
Shape::hits_side()
{
    for (Square square : squares_) {
        if (not square.valid_space()) {
            return true;
        }
    }
    return false;
}

bool
Shape::hits_bottom()
{
    for (Square square : squares_) {
        if (square.sq_hits_bottom()) {
            return true;
        }
    }
    return false;
}

bool
Shape::hits_stag(std::vector<Position> stagnant_squares)
{
    for (Square f_square : squares_) {
        for (Position st_square : stagnant_squares) {
            if ((f_square.position().y == st_square.y) and (f_square.position().x == st_square.x))
                return true;
        }
    }
    return false;
}

Shape
Shape::fall()
{
    Position Posn = {center_.x, center_.y + 1};
    return make_shape(Posn, orientation_, group_);
}

Shape
Shape::move_right()
{
    Shape maybe = make_shape({center_.x + 1, center_.y}, orientation_, group_);
    if (not maybe.hits_side()) {
        return maybe;
    }
    else {
        return make_shape(center_, orientation_, group_);
    }
}

Shape
Shape::move_left()
{
    Shape maybe = make_shape({center_.x - 1, center_.y}, orientation_, group_);
    if (not maybe.hits_side()) {
        return maybe;
    }
    else {
        return make_shape(center_, orientation_, group_);
    }
}




//// ACCESSING PRIVATE MEMBERS /////

Square_set
Shape::squares()
{return squares_;}

Position
Shape::center()
{return center_;}

int
Shape::orientation()
{return orientation_;}


/// THIS IS FOR MODEL TESTING ///
void
Shape::change_center(Position pos)
{
    center_ = pos;
}

