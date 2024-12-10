//
// Created by root on 11/19/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_welcome_widget.h" resolved
#include <QDesktopServices>
#include "welcome_widget.h"

#include "help_slide_widget.h"
#include "main_window.h"
#include "settings_widget.h"
#include "ui_welcome_widget.h"


welcome_widget::welcome_widget(main_window* parent) :
    QWidget(parent), ui(new Ui::welcome_widget)
{
    ui->setupUi(this);
    // qDebug() << "new welcome_widget";
    connect(this->ui->button_play, &QPushButton::clicked, this, [=]()
    {
        // qDebug()<<parent->widget_stack.top();
        // qDebug()<<parent->widget_stack.size();
        BattlefieldWidget* widget = new battle_widget(parent);
        const auto game = new Game(widget, "../maps/2.tmx");
        game->run();
        parent->widget_push(widget);
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
