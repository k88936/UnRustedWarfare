//
// Created by root on 12/12/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_dialog_widget.h" resolved

#include "dialog_widget.h"
#include "ui_dialog_widget.h"


dialog_widget::dialog_widget(QWidget *parent) :
    QFrame(parent), ui(new Ui::dialog_widget) {
    ui->setupUi(this);
}

dialog_widget::~dialog_widget() {
    delete ui;
}
