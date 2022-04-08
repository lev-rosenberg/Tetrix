#include "view.hxx"
#include <iostream>
static int const grid_size = 30;
static ge211::Color board_color {30,30,30};
static ge211::Color falling_color {5,100,5};
static ge211::Color stagnant_color {100,5,5};
static std::string const tetris_music_filename {"Tetty_music.wav"};
using Position_set = std::vector<Position>;

View::View(Model const& model, ge211::Mixer& mixer)
        : model_(model),
          board_square({grid_size-4, grid_size-4}, board_color),
          falling_square({grid_size-4, grid_size-4}, falling_color),
          stagnant_square({grid_size-4, grid_size-4}, stagnant_color),
          mixer_(mixer),
          game_over(false)
{
    if (mixer_.is_enabled()) {
        load_audio_();
    }
}

void
View::draw(ge211::Sprite_set& set, Shape falling, Position_set stagnant, int
score)
{
    ge211::Text_sprite::Builder score_builder(sans30);
    if (not game_over) {
        score_builder.color({200, 200, 200});
    } else {
        score_builder.color({100, 5, 5});
    }
    score_builder << score;
    score_sprite.reconfigure(score_builder);
    set.add_sprite(score_sprite, {330, 50}, 3);

    for (Square square: falling.squares()) {
        set.add_sprite(falling_square, board_to_screen(square.position()), 1);
    }
    Position_set new_stagnant;
    for (int i = 0; i < model_.board().dimensions().width; i++) {
        for (int j = 0; j < model_.board().dimensions().height; j++) {
            set.add_sprite(board_square, {2 + i * grid_size, 2 + j *
                                                                 grid_size}, 0);
            if (model_.full_row(j, stagnant)) {
                new_stagnant = model_.reset_stagnant(j, stagnant);
                reset_stag_squares(new_stagnant, stagnant);
            }
        }
    }
    for (Position posn : stagnant) {
        if (posn.y == 0){
            game_over = true;
        }
        set.add_sprite(stagnant_square, board_to_screen(posn), 2);
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    return {((grid_size + 10) * model_.board().dimensions().width),
            (grid_size * model_.board().dimensions().height)};
}

Position
View::board_to_screen(Position logical)
{
    logical.x = logical.x * grid_size - grid_size + 2;
    logical.y = logical.y * grid_size + 2;
    return logical;
}

void
View::reset_stag_squares(Position_set& new_stag, Position_set& old_stag)
{
    old_stag.clear();
    for (Position posn : new_stag) {
        old_stag.push_back(posn);
    }
}

void
View::load_audio_()
{
    try {
        // To enable background music, put a file named bg_music.ogg
        // in the Resources/ directory.
        if (tet_music_.try_load(tetris_music_filename, mixer_)) {
            mixer_.play_music(tet_music_);
        }
    } catch (ge211::Environment_error const& exn) {
        ge211::internal::logging::warn(exn.what())
                << "To enable background music, put a file named\n"
                << tetris_music_filename << " in the Resources/ directory.";
    }
}

void
View::resume_music_if_ended() const
{
    if (mixer_.get_music_state() == ge211::Mixer::State::paused) {
        mixer_.resume_music();
    }
}

bool
View::game_over_()
{return game_over;}