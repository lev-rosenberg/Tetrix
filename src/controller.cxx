#include "controller.hxx"
#include "shapes.hxx"
#include <iostream>
using namespace ge211;

Controller::Controller(Model& model)
        : model_(model),
        view_(model, mixer())
{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set, model_.falling_shape_, model_.board().stagnant_squares_,
               model_.board().score());
}

void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
    view_.resume_music_if_ended();
    if (view_.game_over_()){
        quit();
    }
}

void
Controller::on_key(ge211::Key key)
{
    if ((key == Key::up()) or (key == Key::down())) {
        Shape maybe = model_.falling_shape_.rotate();
        if ((!maybe.hits_side()) and (!maybe.hits_stag(model_.board().all_stagnant_squares()))) {
            model_.falling_shape_ = maybe;
        }
    }
    if (key == Key::left()) {
        Shape maybe = model_.falling_shape_.move_left();
        if (not maybe.hits_stag(model_.board().all_stagnant_squares())) {
            model_.falling_shape_ = model_.falling_shape_.move_left();
        }
    }
    if (key == Key::right()) {
        Shape maybe = model_.falling_shape_.move_right();
        if (not maybe.hits_stag(model_.board().all_stagnant_squares())) {
            model_.falling_shape_ = model_.falling_shape_.move_right();
        }
    }
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

