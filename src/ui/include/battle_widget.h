//
// Created by root on 11/20/24.
//

#ifndef BATTLE_WIDGET_H
#define BATTLE_WIDGET_H

#include <qdatetime.h>
#include <QWidget>
#include <set>
#include <Unit.h>

#include "suspend_menu_widget.h"
#include "welcome_widget.h"


QT_BEGIN_NAMESPACE

namespace Ui
{
    class battle_widget;
}

QT_END_NAMESPACE

class battle_widget : public BattlefieldWidget
{
    Q_OBJECT

public:
    suspend_menu_widget* suspend_menu;
    battle_widget(main_window* parent);
    ~battle_widget() override;
    BattlefieldWidget* get_battleFieldWidget();
    void mouseMoveEvent(QMouseEvent* event) override;
    void clean_selected();
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    std::set<Unit*> units_selected;
    QTime m_press_time;
    bool m_l_pressing;
    bool m_r_pressing;

    QVector3D m_press_pos_world;
    QVector3D m_release_pos_world;
    QPoint m_press_pos_screen;
    QPoint m_release_pos_screen;

    QVector3D camera_pos_when_pressed;

private:
    Ui::battle_widget* ui;
    Drawable* move_flag = new Drawable();
};


#endif //BATTLE_WIDGET_H
