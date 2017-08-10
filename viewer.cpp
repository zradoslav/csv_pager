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
    clip->setText(this->ui->textEdit->toPlainText());
}
