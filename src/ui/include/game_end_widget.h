//
// Created by root on 11/29/24.
//

#ifndef GAME_END_WIDGET_H
#define GAME_END_WIDGET_H

#include<QWidget>


class main_window;
QT_BEGIN_NAMESPACE

namespace Ui
{
    class game_end_widget;
}

QT_END_NAMESPACE

class game_end_widget : public QWidget
{
    Q_OBJECT

public:
    explicit game_end_widget(main_window* parent);
    ~game_end_widget() override;
    Ui::game_end_widget* ui;
};


#endif //GAME_END_WIDGET_H
