// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>
#include <QRandomGenerator>
#include <QAudioOutput>
#include <QDir>
#include <QLabel>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QSoundEffect>

#include "UnitConfigs.h"
#include "Game.h"
#include "main_window.h"
#include "splash_widget.h"

QString getRandomFileName(const QString& directoryPath)
{
    QDir dir(directoryPath);
    if (!dir.exists())
    {
        qWarning() << "Directory does not exist:" << directoryPath;
        return QString();
    }

    QStringList fileNames = dir.entryList(QDir::Files);
    if (fileNames.isEmpty())
    {
        qWarning() << "No files found in directory:" << directoryPath;
        return QString();
    }

    int randomIndex = QRandomGenerator::global()->bounded(fileNames.size());
    return fileNames.at(randomIndex);
}

void do_work(QMediaPlayer* player, QString path)
{
    QString fileName = getRandomFileName(path);

    player->setSource(QUrl::fromLocalFile(path + fileName));
    player->play();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    app.setApplicationName("test");
    app.setApplicationVersion("0.1");

    // splash_widget splash;
    // splash.show();
    // auto* music = new QSoundEffect();
    // music->setSource(QUrl::fromLocalFile("../sound/tank_moving0.wav"));
    // // music->setSource(QUrl("qrc:/1.wav"));
    // music->setLoopCount(QSoundEffect::Infinite); //设置无限循环
    // music->setVolume(0.5f); //设置音量，在0到1之间
    // music->play();


    auto player = new QMediaPlayer();
    auto audio = new QAudioOutput();
    player->setAudioOutput(audio);
    audio->setVolume(0.025);
    auto path = "../resources/assets/music/";
    do_work(player, path);
    QObject::connect(player, &QMediaPlayer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::EndOfMedia)
        {
            do_work(player, path);
        }
        else if (status == QMediaPlayer::InvalidMedia)
        {
            do_work(player, path);
        }
    });


    // player->setSource(QUrl("qrc:/b.ogg"));


    UnitConfigs::init();
    main_window window;

    // sleep(1);
    // splash.hide();

    window.showMaximized();
    // Game::init();
    return app.exec();
}
