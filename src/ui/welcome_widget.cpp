//
// Created by root on 11/19/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_welcome_widget.h" resolved
#include <QDesktopServices>
#include "welcome_widget.h"

#include <ui_choose_widget.h>

#include "choose_widget.h"
#include "help_slide_widget.h"
#include "main_window.h"
#include "settings_widget.h"
#include "ui_welcome_widget.h"


welcome_widget::welcome_widget(main_window* parent) :
    QWidget(parent), ui(new Ui::welcome_widget)
{
    ui->setupUi(this);
    map_chooser = new choose_widget(this, "../resources/maps/", "*.tmx");
    map_chooser->setVisible(false);
    connect(map_chooser->ui->button_cancel, &QPushButton::clicked, this, [this]()
    {
        map_chooser->setVisible(false);
    });
    connect(map_chooser->ui->button_ok, &QPushButton::clicked, this, [parent,this]()
    {
        const auto item = map_chooser->ui->listWidget->currentItem();
        if (item == nullptr)
        {
            return;
        }
        map_chooser->setVisible(false);
        auto widget = new battle_widget(parent);
        const auto game = new Game(widget, "../resources/maps/" + item->text().toStdString());
        game->run();
        parent->widget_push(widget);
    });
    connect(this->ui->button_play, &QPushButton::clicked, this, [this]()
    {
        map_chooser->setVisible(true);
    });
    connect(this->ui->button_github, &QPushButton::clicked, this, [=]()
    {
        QDesktopServices::openUrl(QUrl("https://github.com/k88936/UnRustedWarfare"));
    });
    connect(this->ui->button_help, &QPushButton::clicked, this, [=]()
    {
        parent->widget_push(new help_slide_widget(parent));
    });
    connect(this->ui->button_settings, &QPushButton::clicked, this, [=]()
    {
        parent->widget_push(new settings_widget(parent));
    });
}

welcome_widget::~welcome_widget()
{
    // Game::stop();
    // qDebug() << "delete welcome_widget";
    delete ui;
}

void welcome_widget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    map_chooser->setGeometry((this->width() - map_chooser->width()) / 2,
                             (this->height() - map_chooser->height()) / 2, map_chooser->width(),
                             map_chooser->height());
}

void welcome_widget::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    if (visible)
    {
        ui->widget->game->run();
    }
    else
    {
        ui->widget->game->pause();
    }
}
