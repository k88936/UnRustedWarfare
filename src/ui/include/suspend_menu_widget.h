//
// Created by root on 12/1/24.
//

#ifndef SUSPEND_MENU_WIDGET_H
#define SUSPEND_MENU_WIDGET_H

#include <QFrame>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class suspend_menu_widget; }
QT_END_NAMESPACE

class suspend_menu_widget : public QFrame {
Q_OBJECT

public:
    explicit suspend_menu_widget(QWidget *parent = nullptr);
    ~suspend_menu_widget() override;
    Ui::suspend_menu_widget *ui;
};


#endif //SUSPEND_MENU_WIDGET_H
