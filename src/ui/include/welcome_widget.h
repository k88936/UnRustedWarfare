//
// Created by root on 11/19/24.
//

#ifndef WELCOME_WIDGET_H
#define WELCOME_WIDGET_H

#include "battlefieldwidget.h"
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class welcome_widget; }
QT_END_NAMESPACE

class welcome_widget : public QWidget {
Q_OBJECT

public:
    explicit welcome_widget(QWidget *parent = nullptr);
    ~welcome_widget() override;

void    setWidget(QOpenGLWidget* widget);
void update();
BattlefieldWidget* get_battleFieldWidget();

private:
    Ui::welcome_widget *ui;
};


#endif //WELCOME_WIDGET_H
