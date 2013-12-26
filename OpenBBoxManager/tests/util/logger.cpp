#include "logger.h"
#include "string.h"

Logger::Logger(QObject *parent) :
    QObject(parent)
{
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
void customMessageHandler(QtMsgType type, const char *msg)
#else
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
#endif
{
    QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    QString txt = QString("(%1) ").arg(dt);

    switch (type)
    {
        case QtDebugMsg:
            txt += QString("\t   [Debug]\t %1").arg(msg);
            fprintf(stdout, "%s\n", qPrintable(txt));
            break;
        case QtWarningMsg:
            txt += QString("\t [Warning]\t %1").arg(msg);
            fprintf(stderr, "%s\n", qPrintable(txt));
            break;
        case QtCriticalMsg:
            txt += QString("\t[Critical]\t %1").arg(msg);
            fprintf(stderr, "%s\n", qPrintable(txt));
            break;
        case QtFatalMsg:
            txt += QString("\t   [Fatal]\t %1").arg(msg);
            fprintf(stderr, "%s\n", qPrintable(txt));
            abort();
            break;
    }

#ifdef LOG_TO_FILE
    QFile outFile("log.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream textStream(&outFile);
    textStream << txt << endl;
#endif

    fflush(stdout);
    fflush(stderr);
}

void Logger::install()
{

#ifdef LOG_TO_FILE
    QFile outFile("LogFile.log");
    outFile.remove();
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    qInstallMsgHandler(customMessageHandler);
#else
    qInstallMessageHandler(customMessageHandler);
#endif
}
