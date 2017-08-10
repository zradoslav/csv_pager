#include "viewer.h"
#include "ui_viewer.h"

#if QT_VERSION_MAJOR > 4
#include <QGuiApplication>
#else
#include <QApplication>
#endif
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
    ui->resultText->setPlainText(content);
}

void Viewer::on_copyButton_clicked()
{
#if QT_VERSION_MAJOR > 4
    QClipboard* clip = QGuiApplication::clipboard();
#else
    QClipboard* clip = QApplication::clipboard();
#endif
    QString clipText = ui->resultText->toPlainText();

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
