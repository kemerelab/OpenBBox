#include <QtWidgets>

#include "subjectdialog.h"

SubjectDialog::SubjectDialog(QDialog *parent)
    : QDialog(parent)
{
    int frameStyle = QFrame::Sunken | QFrame::Panel;
    textLabel = new QLabel;
    textLabel->setFrameStyle(frameStyle);
    dirLabel = new QLabel;
    dirLabel->setFrameStyle(frameStyle);
    QPushButton *textButton = new QPushButton(tr("Name"));
    QPushButton *taskButton = new QPushButton(tr("Open Task"));
    QPushButton *okButton = new QPushButton(tr("OK"));

    connect(textButton, SIGNAL(clicked()), this, SLOT(setText()));
    connect(taskButton, SIGNAL(clicked()), this, SLOT(setTaskDir()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(sendSignal()));

    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnStretch(0, 1);
    layout->setColumnMinimumWidth(0, 250);
    layout->addWidget(textButton, 0, 1);
    layout->addWidget(textLabel, 0, 0);
    layout->addWidget(taskButton, 1, 1);
    layout->addWidget(dirLabel, 1, 0);
    layout->addWidget(okButton, 2, 1);

    setWindowTitle(tr("Fill Info."));
}

QString SubjectDialog::getTaskDir(){
    return task;
}

void SubjectDialog::setText()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("QInputSubjectDialog::getText()"),
                                         tr("Tag:"), QLineEdit::Normal,
                                         "", &ok);

    if (ok && !text.isEmpty())
    {
        textLabel->setText(text);
        subject.name = text;
    }else {
        subject.name = "";
    }
}

void SubjectDialog::setTaskDir()
{
    QString dir = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("Files (*.MPC)"));

    if (!dir.isNull())
    {
        dirLabel->setText(dir);
        task = dir;
    }else {
        task = "";
    }
}

void SubjectDialog::sendSignal(){
    if(!subject.name.isEmpty()&&!task.isEmpty()){
        close();
        subject.status = true;
        emit processPassinfo(subject);
    }
}




