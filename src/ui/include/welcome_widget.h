//
// Created by root on 11/19/24.
//

#ifndef WELCOME_WIDGET_H
#define WELCOME_WIDGET_H

#include "battlefieldwidget.h"
#include <QWidget>
#include <QtOpenGLWidgets/QOpenGLWidget>


class choose_widget;
class main_window;
QT_BEGIN_NAMESPACE

namespace Ui
{
    class welcome_widget;
}

QT_END_NAMESPACE

class welcome_widget : public QWidget
{
    Q_OBJECT

public:
    explicit welcome_widget(main_window* parent);
    ~welcome_widget() override;
    choose_widget* map_chooser;
    Ui::welcome_widget* ui;
    void resizeEvent(QResizeEvent* event) override;
    // std::string map_chose;
    void setVisible(bool visible) override;
};


#endif //WELCOME_WIDGET_H
