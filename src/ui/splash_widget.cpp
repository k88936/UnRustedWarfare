//
// Created by root on 11/20/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_splash_widget.h" resolved

#include "splash_widget.h"
#include "ui_splash_widget.h"


splash_widget::splash_widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::splash_widget) {
    ui->setupUi(this);
}

splash_widget::~splash_widget() {
    delete ui;
}
