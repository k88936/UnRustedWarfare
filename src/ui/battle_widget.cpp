//
// Created by root on 11/20/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_battle_widget.h" resolved

#include "battle_widget.h"
#include "ui_battle_widget.h"


battle_widget::battle_widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::battle_widget) {
    ui->setupUi(this);
}

battle_widget::~battle_widget() {
    delete ui;
}
