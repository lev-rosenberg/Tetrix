#include "model.hxx"
#include <catch.hxx>
#include <iostream>
#include <shapes.hxx>

using Position_set = std::vector<Position>;

TEST_CASE("shape falls")
{
    Model model;
    CHECK(model.falling_shape_.center() == Position(5, -2));
    model.change_center({5,17});
    CHECK(model.falling_shape_.center() == Position(5, 17));
    model.on_frame(0.5);
    CHECK(model.falling_shape_.center() == Position(5, 18));
}

TEST_CASE("shape rotates")
{
    Model model;
    model.change_center({5,5});
    CHECK(model.falling_shape_.orientation() == 0);
    model.falling_shape_ = model.falling_shape_.rotate();
    CHECK(model.falling_shape_.orientation() == 1);
    model.falling_shape_ = model.falling_shape_.rotate();
    CHECK(model.falling_shape_.orientation() == 2);
    model.falling_shape_ = model.falling_shape_.rotate();
    CHECK(model.falling_shape_.orientation() ==3);
    model.falling_shape_ = model.falling_shape_.rotate();
    CHECK(model.falling_shape_.orientation() ==0);
}

TEST_CASE("MOVING LEFT / RIGHT / ROTATE + hits side of screen")
{
    Model model;
    model.change_center({4,5});
    model.falling_shape_ = model.falling_shape_.move_left();
    CHECK(model.falling_shape_.center() == Position(3, 5));
    model.falling_shape_ = model.falling_shape_.move_left();
    CHECK(model.falling_shape_.center() == Position(2, 5));
    model.falling_shape_ = model.falling_shape_.move_left();
    CHECK(model.falling_shape_.center() == Position(2, 5));
    //it stays at 2 because the left side of the board is 1, and there is 1 square left of center in this orientation (T)
    CHECK(model.falling_shape_.orientation() == 0);
    model.falling_shape_ = model.falling_shape_.rotate();
    CHECK(model.falling_shape_.orientation() == 1);
    model.falling_shape_ = model.falling_shape_.move_left();
    CHECK(model.falling_shape_.center() == Position(2, 5));
    //its still 2 because there is still 1 square left of center in this orientation (left sideways T)
    model.falling_shape_ = model.falling_shape_.rotate();
    CHECK(model.falling_shape_.orientation() == 2);
    model.falling_shape_ = model.falling_shape_.rotate();
    CHECK(model.falling_shape_.orientation() == 3);
    model.falling_shape_ = model.falling_shape_.move_left();
    //its now 1 because there is no square left of center in this orientation (right sideways T)
    CHECK(model.falling_shape_.center() == Position(1, 5));
}


TEST_CASE("hitting bottom adds stagnant squares, and falling shape re-initializes at top")
{
    Model model;
    CHECK(model.board().all_stagnant_squares() == Position_set{});
    model.change_center({4,17});
    model.on_frame(0.25);
    CHECK(model.board().all_stagnant_squares() == Position_set{});
    model.on_frame(0.25);
    CHECK(model.board().all_stagnant_squares() == Position_set{{3,18},{4,18}, {5,18}, {4,19}});
    CHECK(model.falling_shape_.center() == Position{5,-2});
}
TEST_CASE("after filling bottom row, the row dissapears (and stagnant squares left above move down)")
{
    Model model;
    CHECK(model.board().all_stagnant_squares() == Position_set{});
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({2,18});
    model.on_frame(0.25);
    model.on_frame(0.25);
    CHECK(model.board().all_stagnant_squares() == Position_set{{3,19},{2,19}, {1,19}});
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({5,18});
    model.on_frame(0.25);
    model.on_frame(0.25);
    CHECK(model.board().all_stagnant_squares() == Position_set{{3,19},{2,19}, {1,19},{6,19},{5,19}, {4,19}});
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({8,17});
    model.on_frame(0.25);
    model.on_frame(0.25);
    model.on_frame(0.25);
    CHECK(model.board().all_stagnant_squares() == Position_set{{3,19},{2,19}, {1,19},{6,19},{5,19}, {4,19},{9,19},{8,19}, {7,19}});
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 0);
    model.change_center({10,17});
    model.on_frame(0.25);
    model.on_frame(0.25);
    //these last stagnant squares are from the top 2 squares of a vertical line_shape that filled in the row
    CHECK(model.board().all_stagnant_squares() == Position_set{{10,19}, {10,18}});
}

TEST_CASE("hits stagnant squares turns falling shape stagnant")
{
    Model model;
    CHECK(model.board().all_stagnant_squares() == Position_set{});
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({2,18});
    model.on_frame(0.25);
    model.on_frame(0.25);
    CHECK(model.board().all_stagnant_squares() == Position_set{{3,19},{2,19}, {1,19}});
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({2,17});
    model.on_frame(0.25);
    model.on_frame(0.25);
    CHECK(model.board().all_stagnant_squares() == Position_set{{3,19},{2,19}, {1,19},{3,18},{2,18}, {1,18}});
}

TEST_CASE("score adds 25 after row dissapears plus another 1 with each second")
{
    Model model;
    int score = model.board().score();
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({2,18});
    model.on_frame(0.25);
    model.on_frame(0.25);
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({5,18});
    model.on_frame(0.25);
    model.on_frame(0.25);
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 1);
    model.change_center({8,17});
    model.on_frame(0.25);
    model.on_frame(0.25);
    model.on_frame(0.25);
    model.falling_shape_ = model.falling_shape_.make_line_shape({5,-2}, 0);
    model.change_center({10,17});
    model.on_frame(0.25);
    model.on_frame(0.25);
    //+ another 2 because of the 2.25 seconds passing
    CHECK(model.board().score() == score + 25 + 2);
}

