#pragma once

#include "board.hxx"
#include "tile.hxx"

#include <ge211.hxx>

#include <queue>

class Model
{
public:
    using Coordinate = Board::Coordinate;
    using Dimensions = Board::Dimensions;
    using Position = Board::Position;
    using Position_set = Board::Position_set;

    /// Enumeration of events that the model might want to inform the
    /// controller about.
    enum class Reaction
    {
        error,
        swap,
        noop,
    };

    Model(
            Dimensions board_dimensions,
            Tile::Action_list const& actions,
            int number_of_groups,
            int min_group_size = 4);

    const Board& board() const
    { return board_; }

    std::size_t score() const
    { return score_; }

    int number_of_groups() const
    { return number_of_groups_; }

    const Tile::Action_list& actions() const
    { return actions_; }

    // Runs the model for one step. Returns true if work was done,
    // or false if there was nothing to do.
    bool step();

    // Lets the controller tell the model about a user action.
    // The model returns whether the action was successful.
    Reaction swap_tiles(Position, Position);

private:
    bool contagion_step_();

    bool falling_step_();

    bool scavenge_step_();

    bool scavenge_position_(Position);

    void condemn_position_(Position);

    Position_set gather_group_(Position);

    Board board_;
    Tile::Action_list actions_;
    std::size_t number_of_groups_;
    std::size_t min_group_size_;
    std::queue<Position> condemned_;

    std::size_t score_ = 0;
    std::size_t score_increment_ = 0;
    bool has_swapped_ = false;

    // Random sources
    ge211::Random_source<int> random_group_;
    ge211::Random_source<int> random_action_;
    ge211::Random_source<bool> random_action_probability_;
};
