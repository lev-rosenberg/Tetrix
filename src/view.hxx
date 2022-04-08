#pragma once

#include "model.hxx"
#include <ge211.hxx>

using Position_set = std::vector<Position>;

class View
{
public:
    using Dimensions = ge211::Dims<int>;
    using Position = ge211::Posn<int>;
    using Rectangle = ge211::Rect<int>;

    explicit View(const Model& model, ge211::Mixer& mixer);

    void draw(ge211::Sprite_set& set, Shape falling, Position_set stagnant,
              int score);
    Position board_to_screen(Position logical);
    View::Dimensions initial_window_dimensions() const;
    void load_audio_();
    void resume_music_if_ended() const;
    bool game_over_();
    void reset_stag_squares(Position_set& new_stag, Position_set& old_stag);
private:
    Model model_;
    ge211::Rectangle_sprite board_square;
    ge211::Rectangle_sprite falling_square;
    ge211::Rectangle_sprite stagnant_square;
    ge211::Font sans30{"sans.ttf", 30};
    ge211::Text_sprite score_sprite;
    ge211::Mixer& mixer_;
    ge211::Music_track tet_music_;
    bool game_over;
};
