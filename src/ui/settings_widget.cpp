//
// Created by root on 12/6/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_settings_widget.h" resolved

#include "settings_widget.h"

#include "main_window.h"
#include "ui_settings_widget.h"


settings_widget::settings_widget(main_window *parent) :
    QWidget(parent), ui(new Ui::settings_widget) {
    ui->setupUi(this);
}

settings_widget::~settings_widget() {
    delete ui;
}
