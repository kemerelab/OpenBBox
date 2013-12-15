#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QIODevice>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <iostream>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
   //void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void install();
signals:

public slots:

};

#endif // LOGGER_H
