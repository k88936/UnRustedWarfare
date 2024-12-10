//
// Created by root on 12/9/24.
//

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <stack>

#include "battle_widget.h"
#include "welcome_widget.h"


QT_BEGIN_NAMESPACE

namespace Ui
{
    class main_window;
}

QT_END_NAMESPACE

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_window(QWidget* parent = nullptr);
    void widget_push(QWidget* widget);
    void widget_change(QWidget* widget);
    void widget_pop();
    void resizeEvent(QResizeEvent* event) override;
    ~main_window() override;
    Ui::main_window* ui;
    std::stack<QWidget*> widget_stack;
};


#endif //MAIN_WINDOW_H
