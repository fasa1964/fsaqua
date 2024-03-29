#include "fsaquawindow.h"
#include <QApplication>


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s \n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    }
}


int main(int argc, char *argv[])
{

    qInstallMessageHandler(myMessageOutput);

    QApplication app(argc, argv);

    QApplication::setApplicationName("AQua");
    QApplication::setApplicationVersion("2.0");
    QApplication::setWindowIcon(QIcon(QPixmap(":/icons/appIcon.jpg")));

    qRegisterMetaType<GBadObject *>();
    qRegisterMetaTypeStreamOperators<GBadObject>("GBadObject");

    FSAquaWindow window;
    window.show();

    return app.exec();
}
