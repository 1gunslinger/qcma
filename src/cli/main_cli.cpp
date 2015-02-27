/*
 *  QCMA: Cross-platform content manager assistant for the PS Vita
 *
 *  Copyright (C) 2013  Codestation
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef Q_OS_WIN32
#include <signal.h>
#endif

#include <QDebug>
#include <QLibraryInfo>
#include <QLocale>
#include <QStringList>
#include <QTextCodec>
#include <QThread>
#include <QTranslator>

#include <inttypes.h>
#include <vitamtp.h>

#include "singlecoreapplication.h"
#include "headlessmanager.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void noMessageOutput(QtMsgType type, const QMessageLogContext &, const QString & str)
{
    const char * msg = str.toStdString().c_str();
#else
void noMessageOutput(QtMsgType type, const char *msg)
{
#endif
    Q_UNUSED(type);
    Q_UNUSED(msg);
}

int main(int argc, char *argv[])
{
    if(SingleCoreApplication::sendMessage(QObject::tr("A instance of QCMA is already running"))) {
        return 0;
    }

    SingleCoreApplication app(argc, argv);

#ifndef Q_OS_WIN32
    // FIXME: libmtp sends SIGPIPE if a socket write fails crashing the whole app
    // the proper fix is to libmtp to handle the cancel properly or ignoring
    // SIGPIPE on the socket
    signal(SIGPIPE, SIG_IGN);
#endif

    if(app.arguments().contains("--with-debug")) {
        VitaMTP_Set_Logging(VitaMTP_DEBUG);
    } else if(app.arguments().contains("--verbose")) {
        VitaMTP_Set_Logging(VitaMTP_VERBOSE);
    } else {
        VitaMTP_Set_Logging(VitaMTP_NONE);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        qInstallMessageHandler(noMessageOutput);
#else
        qInstallMsgHandler(noMessageOutput);
#endif
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#endif

    qDebug("Starting QCMA %s", QCMA_VER);

    QTranslator translator;
    QString locale = QLocale().system().name();
    qDebug() << "Current locale:" << locale;

    if(app.arguments().contains("--set-locale")) {
        int index = app.arguments().indexOf("--set-locale");
        if(index + 1 < app.arguments().length()) {
            qDebug("Enforcing locale: %s", app.arguments().at(index + 1).toUtf8().data());
            locale = app.arguments().at(index + 1);
        }
    }

    if(translator.load("qcma_" + locale, ":/resources/translations")) {
        app.installTranslator(&translator);
    } else {
        qDebug() << "Cannot load translation for locale:" << locale;
    }

    QTranslator system_translator;
    system_translator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&system_translator);

    qDebug("Starting main thread: 0x%016" PRIxPTR, (uintptr_t)QThread::currentThreadId());

    // set the organization/application for QSettings to work properly
    app.setOrganizationName("codestation");
    app.setApplicationName("qcma");

    HeadlessManager manager;
    manager.start();

    return app.exec();
}
