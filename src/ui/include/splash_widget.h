//
// Created by root on 11/20/24.
//

#ifndef SPLASH_WIDGET_H
#define SPLASH_WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class splash_widget; }
QT_END_NAMESPACE

class splash_widget : public QWidget {
Q_OBJECT

public:
    explicit splash_widget(QWidget *parent = nullptr);
    ~splash_widget() override;

private:
    Ui::splash_widget *ui;
};


#endif //SPLASH_WIDGET_H
