//
// Created by root on 12/9/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_main_window.h" resolved

#include "main_window.h"

#include "battle_widget.h"
#include "Game.h"
#include "ui_main_window.h"
#include "welcome_widget.h"


main_window::main_window(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::main_window)
{
    Game::init();
    ui->setupUi(this);
    this->welcome_widget_ = new welcome_widget(this);
    // this->battle_widget_ = new battle_widget(this);
    setCentralWidget(this->welcome_widget_);
    ui->centralwidget = this->welcome_widget_;
    Game::start_on("../maps/2.tmx", this->welcome_widget_->get_battleFieldWidget());
}

main_window::~main_window()
{
    delete ui;
}
