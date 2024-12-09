//
// Created by root on 11/19/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_welcome_widget.h" resolved

#include "welcome_widget.h"

#include "main_window.h"
#include "ui_welcome_widget.h"


welcome_widget::welcome_widget(QWidget* parent) :
    QWidget(parent), ui(new Ui::welcome_widget)
{
    ui->setupUi(this);
    qDebug() << "new welcome_widget";
    connect(this->ui->button_play, &QPushButton::clicked, this, [=]()
    {
        // qDebug()<<parent->widget_stack.top();
        // qDebug()<<parent->widget_stack.size();
        // parent->widget_push(new battle_widget(parent, "../maps/2.tmx"));
    });
}

welcome_widget::~welcome_widget()
{
    // Game::stop();
    qDebug() << "delete welcome_widget";
    delete ui;
}
