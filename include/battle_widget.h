//
// Created by root on 11/20/24.
//

#ifndef BATTLE_WIDGET_H
#define BATTLE_WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class battle_widget; }
QT_END_NAMESPACE

class battle_widget : public QWidget {
Q_OBJECT

public:
    explicit battle_widget(QWidget *parent = nullptr);
    ~battle_widget() override;

private:
    Ui::battle_widget *ui;
};


#endif //BATTLE_WIDGET_H
