//
// Created by root on 11/20/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_battle_widget.h" resolved

#include "battle_widget.h"

#include <Game.h>
#include <qdatetime.h>
#include <QMouseEvent>

#include "FlowField.h"
#include "PathFind.h"

#include "ui_battle_widget.h"


battle_widget::battle_widget(QWidget* parent) :
    QWidget(parent), ui(new Ui::battle_widget)
{
    ui->setupUi(this);
}

battle_widget::~battle_widget()
{
    delete ui;
}

BattlefieldWidget* battle_widget::get_battleFieldWidget()
{
    return ui->widget;
}

constexpr int DRAG_DELAY = 250;


void battle_widget::mouseMoveEvent(QMouseEvent* event)
{
    const QVector3D mouse_pos = ui->widget->screen_to_world(event->pos());

    int elapsed = m_press_time.msecsTo(QTime::currentTime());
    if (m_l_pressing)
    {
        if (elapsed > DRAG_DELAY)
        {
            units_selected.clear();
            const int x1 = Game::grids_manager.x_in_which(m_press_pos_world.x());
            const int y1 = Game::grids_manager.y_in_which(m_press_pos_world.y());
            Game::ui_image_draw_config_map["_select"].clear();
            const int x2 = Game::grids_manager.x_in_which(mouse_pos.x());
            const int y2 = Game::grids_manager.y_in_which(mouse_pos.y());
            Game::line_draw_config.clear();
            Game::line_draw_config.push_back(QVector3D(m_press_pos_world.x(), m_press_pos_world.y(), 0));
            Game::line_draw_config.push_back(QVector3D(mouse_pos.x(), m_press_pos_world.y(), 0));
            Game::line_draw_config.push_back(QVector3D(mouse_pos.x(), m_press_pos_world.y(), 0));
            Game::line_draw_config.push_back(QVector3D(mouse_pos.x(), mouse_pos.y(), 0));
            Game::line_draw_config.push_back(QVector3D(mouse_pos.x(), mouse_pos.y(), 0));
            Game::line_draw_config.push_back(QVector3D(m_press_pos_world.x(), mouse_pos.y(), 0));
            Game::line_draw_config.push_back(QVector3D(m_press_pos_world.x(), mouse_pos.y(), 0));
            Game::line_draw_config.push_back(QVector3D(m_press_pos_world.x(), m_press_pos_world.y(), 0));

            for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
            {
                for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
                {
                    for (const auto& object : Game::grids_manager.grids[x][y]->objects)
                    {
                        if (const auto unit = dynamic_cast<Unit*>(object))
                        {
                            // qDebug()<<unit;
                            if ((unit->position.x() - m_press_pos_world.x()) * (unit->position.x() - mouse_pos.x()) <= 0
                                &&
                                (unit->position.y() - m_press_pos_world.y()) * (unit->position.y() - mouse_pos.y()) <=
                                0)
                            {
                                units_selected.insert(unit);
                                Game::ui_image_draw_config_map["_select"].push_back(unit);
                            }
                        }
                    }
                }
            }
        }
    }

    if (m_r_pressing)
    {
        QVector3D move = ui->widget->screen_relative_to_world_relative(event->pos() - m_press_pos_screen);
        ui->widget->camera_pos = camera_pos_when_pressed - move;
        ui->widget->update_camera();
    }
}

void battle_widget::mouseReleaseEvent(QMouseEvent* event)
{
    m_release_pos_screen = event->pos();
    m_release_pos_world = ui->widget->screen_to_world(event->pos());
    if (event->button() == Qt::LeftButton)
    {
        m_l_pressing = false;
        Game::line_draw_config.clear();
        int elapsed = m_press_time.msecsTo(QTime::currentTime());
        if (elapsed <= DRAG_DELAY)
        {
            units_selected.clear();
            Game::ui_image_draw_config_map["_select"].clear();
            int x = static_cast<int>(m_press_pos_world.x() / Game::grids_manager.grid_size);
            int y = static_cast<int>(m_press_pos_world.y() / Game::grids_manager.grid_size);
            for (const auto& object : Game::grids_manager.grids[x][y]->objects)
            {
                // qDebug()<<object->position;
                if (const auto unit = dynamic_cast<Unit*>(object))
                {
                    if (QVector2D(unit->position - m_press_pos_world).lengthSquared() < unit->radius * unit->radius)
                    {
                        units_selected.insert(unit);
                        Game::ui_image_draw_config_map["_select"].push_back(unit);
                        break;
                    }
                }
            }
        }
    }
    if (event->button() == Qt::RightButton)
    {
        m_r_pressing = false;
    }
}

void battle_widget::wheelEvent(QWheelEvent* event)
{
    // qDebug() << event->angleDelta().y();
    constexpr float delta = 0.00001;
    // if (event->angleDelta().y()>0)
    // {
    ui->widget->camera_zoom = std::max(0.02f, ui->widget->camera_zoom + delta * event->angleDelta().y());
    // }
    // else if ( event->angleDelta().y()<0)
    // {
    //
    //     // ui->widget->camera_zoom=std::max(0.02f,ui->widget->camera_zoom-delta);
    // }
    ui->widget->update_camera();
}

void battle_widget::mousePressEvent(QMouseEvent* event)
{
    m_press_pos_screen = event->pos();
    m_press_pos_world = ui->widget->screen_to_world(event->pos());
    camera_pos_when_pressed = ui->widget->camera_pos;
    if (event->button() == Qt::LeftButton)
    {
        m_l_pressing = true;
        m_press_time = QTime::currentTime();
    }
    if (event->button() == Qt::RightButton)
    {
        m_r_pressing = true;
    }
    if (event->button() == Qt::MiddleButton)
    {

        // FlowField flowField(m_press_pos_world.x(), m_press_pos_world.y(), movementType::LAND);
        //
        // Game::line_draw_config.clear();
        //
        // for (int i = 0; i < MapConfig::world_width; ++i)
        // {
        //     for (int j = 0; j < MapConfig::world_height; ++j)
        //     {
        //         Game::line_draw_config.emplace_back(i, j, 0);
        //         Game::line_draw_config.emplace_back(flowField.field[i][j] * 0.4 + QVector3D(i, j, 0));
        //     }
        // }

        // PathFind pathFinder(30, 43, m_press_pos_world.x(), m_press_pos_world.y(), movementType::LAND);
        // Game::line_draw_config.clear();
        // for (const auto& p : pathFinder.path)
        // {
        //     Game::line_draw_config.emplace_back(p.first, p.second, 0);
        // }
    }
}

void battle_widget::paintEvent(QPaintEvent* event)
{
}
