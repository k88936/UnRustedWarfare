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
    std::vector<QString> cmder_profiles;
    explicit dialog_widget(QWidget *parent = nullptr);
void dialog(const std::string& msg);
~dialog_widget() override;
    Ui::dialog_widget *ui;
};


#endif //DIALOG_WIDGET_H
