//
// Created by root on 12/1/24.
//

#ifndef HELP_SLIDE_WIDGET_H
#define HELP_SLIDE_WIDGET_H

#include <QWidget>
#include <QToolBox>


class main_window;
QT_BEGIN_NAMESPACE
namespace Ui { class help_slide_widget; }
QT_END_NAMESPACE

class help_slide_widget : public QWidget {
Q_OBJECT

public:
    explicit help_slide_widget(QWidget *parent = nullptr);
    ~help_slide_widget() override;

private:
    Ui::help_slide_widget *ui;
};


#endif //HELP_SLIDE_WIDGET_H
