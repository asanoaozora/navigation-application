/**
* @licence app begin@
* SPDX-License-Identifier: MPL-2.0
*
* \copyright Copyright (C) 2013-2014, PCA Peugeot Citroen
*
* \file main.cpp
*
* \brief This file is part of the FSA HMI.
*
* \author Philippe Colliot <philippe.colliot@mpsa.com>
* \author Tanibata, Nobuhiko <NOBUHIKO_TANIBATA@denso.co.jp>
*
* \version 1.0
*
* This Source Code Form is subject to the terms of the
* Mozilla Public License (MPL), v. 2.0.
* If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/.
*
* For further information see http://www.genivi.org/.
*
* List of changes:
*
* 13-10-2014, Tanibata, Nobuhiko, adaptation to layer management
*
* @licence end@
*/
#include <QtQuick/QQuickView>
#include <QApplication>
#include <QPalette>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSurfaceFormat>
#include <QQmlContext>
#include "dbusif.h"
#include "wheelarea.h"
#include "preference.h"
#include "settings.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    // Register component types with QML.
    qmlRegisterType<DBusIf>("lbs.plugin.dbusif", 1, 0, "DBusIf");
    qmlRegisterType<Preference>("lbs.plugin.preference", 1,0, "Preference");
    qmlRegisterType<WheelArea>("lbs.plugin.wheelarea", 1, 0, "WheelArea");

    //get settings stored into the conf file (in $HOME/.config/navigation/fsa.conf)

    app.setOrganizationName(QString("navigation"));
    app.setApplicationName(QString("fsa"));

    Settings* settings=new Settings;
    settings->setIniCodec("UTF-8");

    int rc = 0;

    QQmlEngine engine;
    engine.rootContext()->setContextProperty("Settings",settings);

    QQmlComponent *component = new QQmlComponent(&engine);

    QPalette pal = app.palette();
    pal.setColor(QPalette::Window, Qt::transparent);
    app.setPalette(pal);

    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    component->loadUrl(QUrl(argv[1]));

    if (!component->isReady() ) {
        qWarning("%s", qPrintable(component->errorString()));
        return -1;
    }

    QQmlContext *context = engine.rootContext();

    QObject *topLevel = component->create();

    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    window->setFlags(Qt::FramelessWindowHint);
    window->setColor(Qt::transparent);

    QSurfaceFormat surfaceFormat = window->requestedFormat();
    window->setFormat(surfaceFormat);
    window->show();

    rc = app.exec();
    delete component;
    return rc;
}
