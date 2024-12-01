//
// Created by root on 12/1/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_suspend_menu_widget.h" resolved

#include "suspend_menu_widget.h"
#include "ui_suspend_menu_widget.h"


suspend_menu_widget::suspend_menu_widget(QWidget *parent) :
    QWidget(parent), ui(new Ui::suspend_menu_widget) {
    ui->setupUi(this);
}

suspend_menu_widget::~suspend_menu_widget() {
    delete ui;
}
