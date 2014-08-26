#ifndef SubjectDialog_H
#define SubjectDialog_H

#include <QWidget>
#include "obbnode.h"


QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QErrorMessage;
QT_END_NAMESPACE

class SubjectDialog : public QWidget
{
    Q_OBJECT

public:
    SubjectDialog(QWidget *parent = 0);



private slots:
    void setInteger();
    void setDouble();
    void setItem();
    void setText();

    void errorMessage();

    void sendSignal();


private:
    QLabel *integerLabel;
    QLabel *doubleLabel;
    QLabel *itemLabel;
    QLabel *textLabel;

    QLabel *errorLabel;
    QErrorMessage *errorMessageSubjectDialog;

    SubInfo subject;

signals:
    void processPassSubinfo(SubInfo sub);

};

#endif
