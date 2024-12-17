//
// Created by root on 11/20/24.
//

#ifndef BATTLE_WIDGET_H
#define BATTLE_WIDGET_H

#include <qdatetime.h>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>
#include <set>
#include <Unit.h>

#include "game_end_widget.h"

class main_window;
class suspend_menu_widget;
class dialog_widget;
QT_BEGIN_NAMESPACE

namespace Ui
{
    class dialog_widget;
    class battle_widget;
}

QT_END_NAMESPACE

class battle_widget : public BattlefieldWidget
{
    Q_OBJECT

public:
    suspend_menu_widget* suspend_menu;
    dialog_widget* dialog_w;
    game_end_widget* end_widget;
    explicit battle_widget(main_window* parent);
    ~battle_widget() override;
    BattlefieldWidget* get_battleFieldWidget();
    void mouseMoveEvent(QMouseEvent* event) override;
    void clean_selected();
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void dialog(const std::string& string) override;
    void info(const std::string& string) override;
    void render() override;
    void game_end(bool win) override;
    std::set<Unit*> units_selected;
    QTime m_press_time;
    bool m_l_pressing = false;
    bool m_r_pressing = false;

    QVector3D m_press_pos_world;
    QVector3D m_release_pos_world;
    QPoint m_press_pos_screen;
    QPoint m_release_pos_screen;

    QVector3D camera_pos_when_pressed;

    class label_with_mouse_event : public QLabel
    {
    public:
        explicit label_with_mouse_event(BattlefieldWidget* parent): QLabel(parent), battle_field_widget(parent)
        {
        }

        void mousePressEvent(QMouseEvent* event) override
        {
            battle_field_widget->camera_pos.setX(event->position().x() / 2);
            battle_field_widget->camera_pos.setY((this->size().height() - event->position().y()) / 2);
            battle_field_widget->update_camera();
        }

        void mouseMoveEvent(QMouseEvent* event) override
        {
            battle_field_widget->camera_pos.setX(event->position().x() / 2);
            battle_field_widget->camera_pos.setY((this->size().height() - event->position().y()) / 2);
            battle_field_widget->update_camera();
        }

        BattlefieldWidget* battle_field_widget;
    };

private:
    Ui::battle_widget* ui;
    Drawable* move_flag = new Drawable();
    main_window* main_win_;
};


#endif //BATTLE_WIDGET_H
