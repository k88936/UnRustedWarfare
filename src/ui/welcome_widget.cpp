//
// Created by root on 11/19/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_welcome_widget.h" resolved

#include "welcome_widget.h"
#include "ui_welcome_widget.h"


welcome_widget::welcome_widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::welcome_widget) {
    ui->setupUi(this);
}

welcome_widget::~welcome_widget() {
    delete ui;
}
void welcome_widget::update() {
    this->ui->widget->update();
}

BattlefieldWidget* welcome_widget::get_battleFieldWidget()
{
    return this->ui->widget;
}

