#include <QtWidgets>

#include "dialog.h"

#define MESSAGE \
    Dialog::tr("<p>Message boxes have a caption, a text, " \
               "and any number of buttons, each with standard or custom texts." \
               "<p>Click a button to close the message box. Pressing the Esc button " \
               "will activate the detected escape button (if any).")



Dialog::Dialog(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    errorMessageDialog = new QErrorMessage(this);

    int frameStyle = QFrame::Sunken | QFrame::Panel;

    integerLabel = new QLabel;
    integerLabel->setFrameStyle(frameStyle);
    QPushButton *integerButton = new QPushButton(tr("integer"));

    doubleLabel = new QLabel;
    doubleLabel->setFrameStyle(frameStyle);
    QPushButton *doubleButton = new QPushButton(tr("double"));

    itemLabel = new QLabel;
    itemLabel->setFrameStyle(frameStyle);
    QPushButton *itemButton = new QPushButton(tr("item"));

    textLabel = new QLabel;
    textLabel->setFrameStyle(frameStyle);
    QPushButton *textButton = new QPushButton(tr("Tag"));

    errorLabel = new QLabel;
    errorLabel->setFrameStyle(frameStyle);
    QPushButton *errorButton = new QPushButton(tr("QErrorMessage::showM&essage()"));

    QPushButton *okButton = new QPushButton(tr("OK"));


    connect(integerButton, SIGNAL(clicked()), this, SLOT(setInteger()));
    connect(doubleButton, SIGNAL(clicked()), this, SLOT(setDouble()));
    connect(itemButton, SIGNAL(clicked()), this, SLOT(setItem()));
    connect(textButton, SIGNAL(clicked()), this, SLOT(setText()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(sendsignal()));

    connect(errorButton, SIGNAL(clicked()), this, SLOT(errorMessage()));

    QWidget *page = new QWidget;
    QGridLayout *layout = new QGridLayout(page);
    layout->setColumnStretch(0, 1);
    layout->setColumnMinimumWidth(0, 250);
    layout->addWidget(integerButton, 3, 1);
    layout->addWidget(integerLabel, 3, 0);
    layout->addWidget(doubleButton, 1, 1);
    layout->addWidget(doubleLabel, 1, 0);
    layout->addWidget(itemButton, 2, 1);
    layout->addWidget(itemLabel, 2, 0);
    layout->addWidget(textButton, 0, 1);
    layout->addWidget(textLabel, 0, 0);

    layout->addWidget(okButton, 4, 1);
    //layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding), 4, 0);

    mainLayout->addWidget(page);

    setWindowTitle(tr("Subject Info."));
}

void Dialog::setInteger()
{
//! [0]
    bool ok;
    int i = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"),
                                 tr("Percentage:"), 25, 0, 100, 1, &ok);
    if (ok)
        integerLabel->setText(tr("%1%").arg(i));
//! [0]
}

void Dialog::setDouble()
{
//! [1]
    bool ok;
    double d = QInputDialog::getDouble(this, tr("QInputDialog::getDouble()"),
                                       tr("Amount:"), 37.56, -10000, 10000, 2, &ok);
    if (ok)
        doubleLabel->setText(QString("$%1").arg(d));
//! [1]
}

void Dialog::setItem()
{
//! [2]
    QStringList items;
    items << tr("Spring") << tr("Summer") << tr("Fall") << tr("Winter");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
                                         tr("Season:"), items, 0, false, &ok);
    if (ok && !item.isEmpty())
        itemLabel->setText(item);
//! [2]
}

void Dialog::setText()
{
//! [3]
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("Tag:"), QLineEdit::Normal,
                                         "", &ok);

    if (ok && !text.isEmpty())
    {
        textLabel->setText(text);
        subject.name = text;
    }else {
        subject.name = "";
    }

//! [3]
}

void Dialog::sendsignal(){
    if(!subject.name.isEmpty()){
        subject.status = true;
        emit processPassSubinfo(subject);
        close();
    }
}

void Dialog::errorMessage()
{
    errorMessageDialog->showMessage(
            tr("This dialog shows and remembers error messages. "
               "If the checkbox is checked (as it is by default), "
               "the shown message will be shown again, "
               "but if the user unchecks the box the message "
               "will not appear again if QErrorMessage::showMessage() "
               "is called with the same message."));
    errorLabel->setText(tr("If the box is unchecked, the message "
                           "won't appear again."));
}

void Dialog::closeEvent(QCloseEvent *){

}

