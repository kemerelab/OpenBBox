#include "logger.h"

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   Q_UNUSED(context);

   QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
   QString txt = QString("[%1] ").arg(dt);

   switch (type)
   {
      case QtDebugMsg:
       txt += QString("(Debug) \t\t %1").arg(msg);
         break;
      case QtWarningMsg:
         txt += QString("(Warning) \t %1").arg(msg);
         break;
      case QtCriticalMsg:
         txt += QString("(Critical) \t %1").arg(msg);
         break;
      case QtFatalMsg:
         txt += QString("(Fatal) \t\t %1").arg(msg);
         abort();
         break;
   }

#ifdef LOG_TO_FILE
   QFile outFile("LogFile.log");
   outFile.open(QIODevice::WriteOnly | QIODevice::Append);

   QTextStream textStream(&outFile);
   textStream << txt << endl;
#else
   std::cout << txt.toAscii().data() << std::endl;
#endif
}

Logger::Logger(QObject *parent) :
    QObject(parent)
{

}

void Logger::start()
{

#ifdef LOG_TO_FILE
    QFile outFile("LogFile.log");
    outFile.remove();
#endif
    qInstallMessageHandler(customMessageHandler);
}
