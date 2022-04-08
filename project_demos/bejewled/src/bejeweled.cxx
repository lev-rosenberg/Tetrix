#include "controller.hxx"
#include "actions.hxx"

#include <iostream>
#include <stdexcept>

static const Model::Dimensions board_dimensions{8, 10};

static const int n_groups = 6;

static const Horizontal_lazer_action hlazer_action;

int main() try
{
    Tile::Action_list actions{&hlazer_action};
    Model model(board_dimensions, actions, n_groups);

    Controller(model).run();
}

catch (std::runtime_error const& e) {
    std::cerr << e.what() << "\n";
    return 1;
}
