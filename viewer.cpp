#include "viewer.h"
#include "ui_viewer.h"

#include <QGuiApplication>
#include <QClipboard>

Viewer::Viewer(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::Viewer)
{
    ui->setupUi(this);
}

Viewer::~Viewer()
{
    delete ui;
}

void Viewer::setContent(const QString &content)
{
    ui->textEdit->setPlainText(content);
}

void Viewer::on_copyButton_clicked()
{
    QClipboard* clip = QGuiApplication::clipboard();
    QString clipText = ui->textEdit->toPlainText();

    if(copyWithQuots)
    {
        clipText.push_front('"');
        clipText.push_back('"');
    }
    clip->setText(clipText);
}

void Viewer::on_quotBox_toggled(bool checked)
{
    copyWithQuots = checked;
}
