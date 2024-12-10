//
// Created by root on 12/9/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_main_window.h" resolved
#include <QPropertyAnimation>

#include "main_window.h"

#include <ui_welcome_widget.h>

#include "battle_widget.h"
#include "Game.h"
#include "ui_main_window.h"


main_window::main_window(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::main_window)
{
    ui->setupUi(this);
    // qDebug()<<widget_stack.size();
    widget_stack.push(ui->centralwidget);


    auto b = new battle_widget(this);;
    Game* game = new Game(b,"../maps/1.tmx");
    auto w = new welcome_widget(this);
    Game* game2 = new Game(w->ui->widget,"../maps/2.tmx");;

    game->run();
    game2->run();
    setCentralWidget(b);
    ui->centralwidget = b;
    // Game::start_on("../maps/1.tmx", w->ui->widget);
    // Game::start_on("../maps/1.tmx", b);
    // b->show();
    // w->show();
    // Game::start_on("../maps/2.tmx", b);


    // widget_push(new battle_widget(this, "../maps/2.tmx"));
    // widget_push(new welcome_widget(this));
}

void main_window::widget_push(QWidget* widget)
{
    qDebug() << widget_stack.size();
    // widget_stack.top()->setVisible(false);
    // widget->setVisible(true);
    widget_stack.push(widget);
    auto in_animation = new QPropertyAnimation(widget, "windowOpacity");
    in_animation->setDuration(500);
    in_animation->setStartValue(0);
    in_animation->setEndValue(1);
    // in_animation->start();
    in_animation->start(QAbstractAnimation::DeleteWhenStopped);
    setCentralWidget(widget);
    ui->centralwidget = widget;
}

void main_window::widget_change(QWidget* widget)
{
    // widget_stack.top()->setVisible(false);
    widget_stack.top()->deleteLater();
    widget_stack.pop();
    // widget->setVisible(true);
    widget_stack.push(widget);
    auto in_animation = new QPropertyAnimation(widget, "Opacity");
    in_animation->setDuration(5000);
    in_animation->setStartValue(0);
    in_animation->setEndValue(1);
    // in_animation->start();
    in_animation->start(QAbstractAnimation::DeleteWhenStopped);
    setCentralWidget(widget);
    ui->centralwidget = widget;
}

void main_window::widget_pop()
{
    // widget_stack.top()->setVisible(false);
    widget_stack.top()->deleteLater();
    widget_stack.pop();
    // widget_stack.top()->setVisible(true);
    auto in_animation = new QPropertyAnimation(widget_stack.top(), "Opacity");
    in_animation->setDuration(500);
    in_animation->setStartValue(0);
    in_animation->setEndValue(1);
    // in_animation->start();
    in_animation->start(QAbstractAnimation::DeleteWhenStopped);
    setCentralWidget(widget_stack.top());
    ui->centralwidget = widget_stack.top();
}

main_window::~main_window()
{
    delete ui;
}
