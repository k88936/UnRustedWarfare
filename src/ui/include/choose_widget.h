//
// Created by root on 12/18/24.
//

#ifndef CHOOSE_WIDGET_H
#define CHOOSE_WIDGET_H

#include <QFrame>


class QListWidgetItem;
QT_BEGIN_NAMESPACE

namespace Ui
{
    class choose_widget;
}

QT_END_NAMESPACE

class choose_widget : public QFrame
{
    Q_OBJECT

public:
    choose_widget(QWidget* parent, const std::string& dir_path, const std::string& name_filter);
    ~choose_widget() override;
    Ui::choose_widget* ui;
};


#endif //CHOOSE_WIDGET_H
