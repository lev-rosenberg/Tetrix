#pragma once

#include "model.hxx"

#include <algorithm>
#include <unordered_map>

// Controls how the game is shown to the user:
class View
{
public:
    ///
    /// Public member functions
    ///
    View(
            const Model& model,
            ge211::Mixer& mixer,
            ge211::Dims<int> window_dims);

    void draw(ge211::Sprite_set&, Board::Position selected);

    /// Tells the view whether animation is currently happening, so it
    /// can play a sound effect during the animation.
    void set_animating(bool);

    /// Plays invalid.ogg.
    void play_invalid_effect() const;

    /// Keeps the music going.
    void resume_music_if_ended() const;

    Model::Position screen_to_board(ge211::Posn<int>) const;

    ge211::Posn<int> board_to_screen(Model::Position) const;

private:
    ///
    /// Private helper functions
    ///

    // Draws the board.
    void draw_board_(ge211::Sprite_set&, Board::Position selected);

    // Draws the score.
    void draw_score_(ge211::Sprite_set&);

    // Loads the audio clips if possible. (Only called from the constructor.)
    void load_audio_();

    // Computes and stores sizing and spaces.
    struct Geometry
    {
        using Position = Model::Position;
        using Dimensions = Model::Dimensions;

        Geometry(Dimensions logical, Dimensions physical);

        Position screen_to_board(Position) const;

        Position board_to_screen(Position) const;

        int tile_radius() const;

        Dimensions grid_dims() const;

        Position score_position(Dimensions score_dims) const;

        int grid_size;
        Dimensions window; // physical
        Dimensions board;  // logical
        Dimensions margin; // physical
    };

    // Holds a circle sprite of the right color for each tile group:
    using Tile_table = std::vector<ge211::Circle_sprite>;

    // Holds text sprites that we can look up by their text:
    using Type_table = std::unordered_map<std::string, ge211::Text_sprite>;

    // The view can look at the model but doesn't change it.
    const Model& model_;

    // For visuals:
    Geometry geometry_;
    ge211::Font type_font_;
    ge211::Rectangle_sprite selection_sprite_;
    Tile_table tile_sprites_;
    Type_table type_sprites_;
    std::size_t previous_score_;  // currently rendered in score_sprite_
    ge211::Font score_font_;
    ge211::Text_sprite score_sprite_;

    // For audio:
    ge211::Mixer& mixer_;
    ge211::Music_track bg_music_;
    ge211::Sound_effect success_sound_;
    ge211::Sound_effect invalid_sound_;
    ge211::Sound_effect_handle success_handle_;
};
