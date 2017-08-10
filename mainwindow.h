#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QCheckBox>

#include <vector>

#include "viewer.h"

class FileDialog : public QFileDialog
{
public:
    FileDialog(QWidget *parent) :
        QFileDialog(parent, "Open CSV", QString(), "CSV (*.csv *.tsv *.txt)")
    {
        this->setOption(QFileDialog::DontUseNativeDialog);

        delimBox = new QCheckBox("Use default delimeter", this);
        delimBox->setChecked(true);

        QGridLayout* grid = dynamic_cast<QGridLayout*>(this->layout());
        rowLayout = new QHBoxLayout(this);
        rowLayout->addWidget(delimBox);

        grid->addLayout(rowLayout, grid->rowCount(), 0, 1, -1);
    }

    ~FileDialog()
    {
        delete delimBox;
        delete rowLayout;
    }

    QCheckBox* delimBox;

private:
    QHBoxLayout* rowLayout;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_CSV_triggered();

    void on_prevButton_clicked();
    void on_nextButton_clicked();

    void on_actionAlways_on_Top_triggered(bool checked);
    void on_actionFix_window_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QVBoxLayout* verticalLayout;
    QWidget* viewWidget;

    struct csv_t
    {
        using string_v = std::vector<std::string>;
        string_v header;
        std::vector<string_v> data;
    } csv;

    unsigned page;

    std::vector<Viewer*> viewArray;

    void readCSV(const std::string &path, char delim);
    void readOptions();
    void setupViews();
    void refreshViews();
};

#endif // MAINWINDOW_H
