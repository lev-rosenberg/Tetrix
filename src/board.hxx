#pragma once
#include <ge211.hxx>
#include <unordered_set>
#include <vector>
class Board
{
public:
    ///
    /// MEMBER TYPES
    ///

    using Rectangle = ge211::Rect<int>;
    using Position = ge211::Posn<int>;
    using Dimensions = ge211::Dims<int>;
    using Coordinate = Rectangle::Coordinate;
    using Position_set = std::vector<Position>;


    Board(Position_set stagnant_squares, int score);
    Position_set all_stagnant_squares();
    Dimensions dimensions() const
    { return dims_; }
    int score()
    {return score_;}
    void update_stagnant_square(Position pos);
    void add_score(int i);
    Position_set stagnant_squares_;
private:
    Dimensions dims_;
    int score_;
};
