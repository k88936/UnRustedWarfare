//
// Created by root on 11/29/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_game_end_widget.h" resolved

#include "game_end_widget.h"

#include <QToolBox>

#include "ui_game_end_widget.h"


game_end_widget::game_end_widget(QWidget* parent) :
    QWidget(parent), ui(new Ui::game_end_widget)
{
    ui->setupUi(this);
    QToolBox* toolBox = new QToolBox(this);
}

game_end_widget::~game_end_widget()
{
    delete ui;
}