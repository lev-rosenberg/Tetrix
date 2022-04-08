#include "tile.hxx"

static const Normal_action normal_action;

Tile::Tile()
        : status_(Tile::Status::empty),
          group_(0),
          action_(nullptr)
{ }

Tile::Tile(
        Action_list const& actions,
        ge211::Random_source<int>& random_group,
        ge211::Random_source<int>& random_action,
        ge211::Random_source<bool>& random_action_probability)
        : status_(Tile::Status::full),
          group_(random_group.next())
{
    if (random_action_probability.next()) {
        auto choice = random_action.next();
        action_ = actions[choice];
    } else {
        action_ = &normal_action;
    }
}

Board::Position_set
Tile::apply_action(Board::Position pos, const Board& board) const
{
    if (action_) {
        return action_->apply(pos, board);
    } else {
        return {};
    }
}

const std::string&
Tile::action_symbol() const
{
    if (action_) {
        return action_->symbol();
    } else {
        // static means this lives between calls:
        static const std::string empty;
        return empty;
    }
}

void
Tile::condemn()
{
    if (is_full()) {
        status_ = Status::condemned;
    }
}

void
Tile::remove()
{
    status_ = Status::empty;
}

Board::Position_set
Normal_action::apply(Position, const Board&) const
{
    return {};
}
