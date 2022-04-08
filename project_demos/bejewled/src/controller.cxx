#include "controller.hxx"

static const double steps_per_second = 10.0;

///
/// Constructors
///

Controller::Controller(Model& model)
        : model_(model),
          view_(model_,
                mixer(),
                default_window_dimensions)
{ }


///
/// Protected member functions
///

void
Controller::on_frame(double dt)
{
    animation_seconds_ += dt;

    while (can_animate_()) {
        if (!model_.step()) {
            animating_ = false;
            break;
        }
    }

    view_.set_animating(animating_);
    view_.resume_music_if_ended();
}

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites, selection_);
}

void
Controller::on_mouse_up(ge211::Mouse_button, ge211::Posn<int> screen_pos)
{
    if (animating_) {
        return;
    }

    const auto& board = model_.board();

    auto clicked = view_.screen_to_board(screen_pos);
    auto selected = selection_;

    selection_ = {-1, -1};

    if (!board.contains(clicked) || board[clicked].is_empty()) {
        return;
    }

    if (!board.contains(selected)) {
        selection_ = clicked;
        return;
    }

    if (board[selected].is_empty()) {
        return;
    }

    switch (model_.swap_tiles(clicked, selected)) {
    case Model::Reaction::error:
        view_.play_invalid_effect();
        break;

    case Model::Reaction::swap:
        start_animation_();
        break;

    case Model::Reaction::noop:
        break;
    }
}

void
Controller::on_start()
{
    start_animation_();
}

void
Controller::start_animation_()
{
    animating_ = true;
    animation_seconds_ = 0;
    animation_step_ = 0;
}

bool
Controller::can_animate_()
{
    if (!animating_) {
        return false;
    }

    bool result = animation_step_ < steps_per_second * animation_seconds_;

    if (result) {
        ++animation_step_;
    }

    return result;
}

void
Controller::on_key(ge211::Key key)
{
    if (key.code() == 'q') { quit(); }
}
