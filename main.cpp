// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QLabel>
#include "game/Configs.h"
#include "mainwidget.h"
#include "game/Game.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    app.setApplicationName("test");
    app.setApplicationVersion("0.1");

    Game::init();
    return app.exec();
}
