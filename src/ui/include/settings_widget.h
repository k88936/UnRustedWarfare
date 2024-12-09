//
// Created by root on 12/6/24.
//

#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H

#include <QWidget>


class main_window;
QT_BEGIN_NAMESPACE
namespace Ui { class settings_widget; }
QT_END_NAMESPACE

class settings_widget : public QWidget {
Q_OBJECT

public:
    explicit settings_widget(QWidget *parent = nullptr);
    ~settings_widget() override;

private:
    Ui::settings_widget *ui;
};


#endif //SETTINGS_WIDGET_H
