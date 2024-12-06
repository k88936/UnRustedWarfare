// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QAudioOutput>
#include <QLabel>
#include <QMediaPlayer>
#include <QSoundEffect>

#include "UnitConfigs.h"
#include "Game.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    app.setApplicationName("test");
    app.setApplicationVersion("0.1");


    auto* music = new QSoundEffect();
    // music->setSource(QUrl::fromLocalFile(":/1.wav"));
    music->setSource(QUrl("qrc:/1.wav"));
    music->setLoopCount(QSoundEffect::Infinite); //设置无限循环
    music->setVolume(0.5f); //设置音量，在0到1之间
    // music->play();

    auto player = new QMediaPlayer();
    auto audio = new QAudioOutput();
    player->setAudioOutput(audio);
    audio->setVolume(0.5);
    player->setSource(QUrl::fromLocalFile("../music/starting/battletanks1B.ogg"));
    // player->setSource(QUrl("qrc:/b.ogg"));
    audio->setVolume(1.0);
    player->play();
    Game::init();
    return app.exec();
}
