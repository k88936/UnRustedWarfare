//
// Created by root on 11/20/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_battle_widget.h" resolved

#include "battle_widget.h"

#include <Game.h>
#include <qdatetime.h>
#include <QMouseEvent>

#include "BoidSensor.h"
#include "FlowField.h"
#include "PathFind.h"

#include "ui_battle_widget.h"
#include "Flock.h"

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
        if (elapsed > DRAG_DELAY) //drag
        {
            const int x1 = Game::grids_manager.x_in_which(m_press_pos_world.x());
            const int y1 = Game::grids_manager.y_in_which(m_press_pos_world.y());
            units_selected.clear();
            Game::ui_image_draw_config_map["_select"].clear();
            Game::ui_image_draw_config_map["_arrow_highlight"].clear();
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
                            if (unit->team != 0)
                            {
                                continue;
                            }
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
        const QVector3D move = ui->widget->screen_relative_to_world_relative(event->pos() - m_press_pos_screen);
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
        if (elapsed <= DRAG_DELAY) //click
        {
            std::set<Unit*> units_to_select;
            std::set<Unit*> enermy_to_select;
            int x = static_cast<int>(m_press_pos_world.x() / Game::grids_manager.grid_size);
            int y = static_cast<int>(m_press_pos_world.y() / Game::grids_manager.grid_size);
            for (const auto& object : Game::grids_manager.grids[x][y]->objects)
            {
                // qDebug()<<object->position;
                if (const auto unit = dynamic_cast<Unit*>(object))
                {
                    if (QVector2D(unit->position - m_press_pos_world).lengthSquared() < unit->radius * unit->radius)
                    {
                        if (unit->team == 0)
                            units_to_select.insert(unit);
                        else if (unit->team == 1)
                        {
                            enermy_to_select.insert(unit);
                        }
                        break;
                    }
                }
            }
            if (!units_to_select.empty())
            {
                units_selected = units_to_select;
                Game::ui_image_draw_config_map["_select"].clear();
                Game::ui_image_draw_config_map["_arrow_highlight"].clear();
                for (auto& selected : units_selected)
                {
                    Game::ui_image_draw_config_map["_select"].push_back(selected);
                }
            }
            else
            {
                auto flock = new Flock();

                for (auto& selected : units_selected)
                {
                    if (selected->boid_sensor->flock != nullptr)
                    {
                        selected->boid_sensor->flock->boids.erase(selected);
                    }
                    flock->boids.insert(selected);
                    selected->boid_sensor->flock = flock;
                }

                if (!flock->boids.empty())
                {
                    flock->move(m_press_pos_world);
                    move_flag->render_transform.setToIdentity();
                    move_flag->render_transform.translate(m_press_pos_world);
                    Game::ui_image_draw_config_map["_arrow_orange"].clear();
                    Game::ui_image_draw_config_map["_arrow_highlight"].clear();
                    if (!enermy_to_select.empty())
                    {
                        auto preferred_target = *(enermy_to_select.begin());
                        for (auto& selected : units_selected)
                        {
                            for (const auto turret : selected->turrets)
                            {
                                Object::ptr_change_to(turret->preferred_target, preferred_target);
                            }
                        }
                        Game::ui_image_draw_config_map["_arrow_orange"].push_back(move_flag);
                    }
                    else
                    {
                        Game::ui_image_draw_config_map["_arrow_highlight"].push_back(move_flag);
                    }
                    Game::flocks.insert(flock);
                }
                else
                {
                    delete flock;
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

void battle_widget::keyReleaseEvent(QKeyEvent* event)
{
    // qDebug()<<event->key();
    if (event->key() == Qt::Key_Space)
    {
        // qDebug()<<"space";
        event->accept();
        units_selected.clear();
        Game::ui_image_draw_config_map["_select"].clear();
        Game::ui_image_draw_config_map["_arrow_highlight"].clear();
    }
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
