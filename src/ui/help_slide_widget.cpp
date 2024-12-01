//
// Created by root on 12/1/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_help_slide_widget.h" resolved

#include "help_slide_widget.h"
#include "ui_help_slide_widget.h"


help_slide_widget::help_slide_widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::help_slide_widget) {
    ui->setupUi(this);
}

help_slide_widget::~help_slide_widget() {
    delete ui;
}
