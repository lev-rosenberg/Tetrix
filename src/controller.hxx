#pragma once

#include "model.hxx"
#include "view.hxx"
#include "shapes.hxx"
#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    Controller(Model& model);

protected:
    void draw(ge211::Sprite_set& set) override;
    void on_frame(double dt) override;
    void on_key(ge211::Key key) override;
    View::Dimensions initial_window_dimensions() const override;
private:
    Model model_;
    View view_;
};
