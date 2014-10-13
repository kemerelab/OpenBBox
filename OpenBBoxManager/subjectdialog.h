#ifndef SubjectDialog_H
#define SubjectDialog_H

#include <QWidget>
#include "obbnode.h"


QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QErrorMessage;
QT_END_NAMESPACE

class SubjectDialog : public QDialog
{
    Q_OBJECT

public:
    SubjectDialog(QDialog *parent = 0);
    QString getTaskDir();
    SubInfo getSubject();

private slots:

    void setText();

    void setTaskDir();

    void sendSignal();

private:
    QLabel *textLabel;
    QLabel * dirLabel;
    SubInfo subject;
    QString task;

signals:
    void processPassinfo(SubInfo sub);

};

#endif
