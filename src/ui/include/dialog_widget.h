//
// Created by root on 12/12/24.
//

#ifndef DIALOG_WIDGET_H
#define DIALOG_WIDGET_H

#include <QFrame>


QT_BEGIN_NAMESPACE
namespace Ui { class dialog_widget; }
QT_END_NAMESPACE

class dialog_widget : public QFrame {
Q_OBJECT

public:
    explicit dialog_widget(QWidget *parent = nullptr);
    ~dialog_widget() override;

private:
    Ui::dialog_widget *ui;
};


#endif //DIALOG_WIDGET_H
