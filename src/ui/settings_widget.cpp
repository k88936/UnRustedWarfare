//
// Created by root on 12/6/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_settings_widget.h" resolved

#include "settings_widget.h"

#include "main_window.h"
#include "ui_settings_widget.h"
#include "../game/config/GameConfig.h"


settings_widget::settings_widget(main_window* parent) :
    QWidget(parent), ui(new Ui::settings_widget)
{
    ui->setupUi(this);
    ui->game_speed_slider->setValue(GameConfig::game_speed*100);
    connect(ui->button_quit, &QPushButton::clicked, parent, [=]
    {
        parent->widget_pop();
    });
    connect(ui->game_speed_slider, &QSlider::valueChanged, parent, [this](int value)
    {
        GameConfig::game_speed = value / 100.0;
        ui->game_speed_label->setText(QString("game speed: %1%").arg(value));
    });
    ui->volume_slider->setValue(GameConfig::game_volume*100);
    connect(ui->volume_slider, &QSlider::valueChanged, parent, [this](int value)
    {
        GameConfig::game_volume = value / 100.0;
        ui->volume_label->setText(QString("volume: %1%").arg(value));
    });
}

settings_widget::~settings_widget()
{
    delete ui;
}
