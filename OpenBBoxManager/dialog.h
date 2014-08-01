#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include "obbnode.h"


QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QErrorMessage;
QT_END_NAMESPACE

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);



private slots:
    void setInteger();
    void setDouble();
    void setItem();
    void setText();

    void errorMessage();

    void sendsignal();
    void closeEvent(QCloseEvent *);

private:
    QLabel *integerLabel;
    QLabel *doubleLabel;
    QLabel *itemLabel;
    QLabel *textLabel;

    QLabel *errorLabel;
    QErrorMessage *errorMessageDialog;

    SubInfo subject;



signals:
    void processPassSubinfo(SubInfo sub);


};

#endif
