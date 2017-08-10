#ifndef VIEWER_H
#define VIEWER_H

#include <QGroupBox>

namespace Ui {
class Viewer;
}

class Viewer : public QGroupBox
{
    Q_OBJECT

public:
    explicit Viewer(QWidget *parent = 0);
    ~Viewer();

    void setContent(const QString& content);

private slots:
    void on_copyButton_clicked();

private:
    Ui::Viewer *ui;
};

#endif // VIEWER_H
