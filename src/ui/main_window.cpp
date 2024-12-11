//
// Created by root on 12/9/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_main_window.h" resolved
#include <QPropertyAnimation>

#include "main_window.h"

#include <qevent.h>
#include <ui_welcome_widget.h>

#include "battle_widget.h"
#include "Game.h"
#include "ui_main_window.h"


main_window::main_window(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::main_window)
{
    ui->setupUi(this);
    widget_stack.push(ui->centralwidget);

    // auto widget = new welcome_widget(this);
    // auto game = new Game(widget->ui->widget, "../maps/3.tmx");;
    // widget_push(widget);

    auto widget =new battle_widget(this);
    // auto game = new Game(widget, "../maps/3.tmx");
    auto game = new Game(widget, "../maps/1.tmx");
    widget_push(widget);
    // Game::start_on("../maps/3.tmx", w->ui->widget);
    // Game::start_on("../maps/3.tmx", b);
    // b->show();
    // w->show();
    // Game::start_on("../maps/2.tmx", b);


    // widget_push(new battle_widget(this, "../maps/2.tmx"));
    // widget_push(new welcome_widget(this));
}

void main_window::widget_push(QWidget* widget)
{
    // qDebug() << widget_stack.size();
    widget_stack.top()->setVisible(false);
    widget->setVisible(true);
    widget_stack.push(widget);
    auto in_animation = new QPropertyAnimation(widget, "windowOpacity");
    in_animation->setDuration(500);
    in_animation->setStartValue(0);
    in_animation->setEndValue(1);
    // in_animation->start();
    in_animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->centralwidget = widget;
    ui->centralwidget->resize(this->size());
}

void main_window::widget_change(QWidget* widget)
{
    widget_stack.top()->setVisible(false);
    widget_stack.top()->deleteLater();
    widget_stack.pop();
    widget->setVisible(true);
    widget_stack.push(widget);
    auto in_animation = new QPropertyAnimation(widget, "windowOpacity");
    in_animation->setDuration(5000);
    in_animation->setStartValue(0);
    in_animation->setEndValue(1);
    // in_animation->start();
    in_animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->centralwidget = widget;
    ui->centralwidget->resize(this->size());
}

void main_window::widget_pop()
{
    widget_stack.top()->setVisible(false);
    widget_stack.top()->deleteLater();
    widget_stack.pop();
    widget_stack.top()->setVisible(true);
    auto in_animation = new QPropertyAnimation(widget_stack.top(), "windowOpacity");
    in_animation->setDuration(500);
    in_animation->setStartValue(0);
    in_animation->setEndValue(1);
    // in_animation->start();
    in_animation->start(QAbstractAnimation::DeleteWhenStopped);
    ui->centralwidget = widget_stack.top();
    ui->centralwidget->resize(this->size());
}

main_window::~main_window()
{
    delete ui;
}

void main_window::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    ui->centralwidget->resize(event->size());
    // for (auto widget : widget_stack)
    // {
    //     widget->resize(event->size());
    // }
}
