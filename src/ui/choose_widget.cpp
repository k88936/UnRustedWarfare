//
// Created by root on 12/18/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_choose_widget.h" resolved

#include "choose_widget.h"

#include <QDir>

#include "ui_choose_widget.h"


choose_widget::choose_widget(QWidget* parent, const std::string& dir_path, const std::string& name_filter) :
    QFrame(parent), ui(new Ui::choose_widget)
{
    ui->setupUi(this);
    const QDir dir((dir_path.data()));
    const QStringList tmxFiles = dir.entryList(QStringList() << name_filter.data(), QDir::Files);

    ui->listWidget->addItems(tmxFiles);
}

choose_widget::~choose_widget()
{
    delete ui;
}

