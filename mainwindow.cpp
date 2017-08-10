#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QScrollArea>
#include <QStatusBar>

#include <fstream>
#include <iostream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("csv_pager");

    viewWidget = new QWidget(this);
    verticalLayout = new QVBoxLayout(this);
    viewWidget->setLayout(verticalLayout);
    ui->scrollArea->setWidget(viewWidget);

    pageLabel = new QLabel(this);
    ui->statusBar->addWidget(pageLabel);

    ui->prevButton->setEnabled(false);
    ui->nextButton->setEnabled(false);

    page = 0;
}

MainWindow::~MainWindow()
{
    for(Viewer* oldView : viewArray)
    {
        verticalLayout->removeWidget(oldView);
        delete oldView;
    }
    viewArray.clear();

    delete pageLabel;
    delete verticalLayout;
    delete viewWidget;
    delete ui;
}

void MainWindow::on_actionOpen_CSV_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open CSV", QString(),
                                                    "CSV (*.csv *.tsv *.txt)");
    if(path.isEmpty())
        return;

    QString delim = QInputDialog::getText(this, "Input delimiter", "Input delimiter:");
    if(delim.isEmpty())
        return;

    readCSV(path.toStdString(), delim.toStdString()[0]);
}

void MainWindow::setupViews()
{
    for(size_t i = 0; i < viewArray.size(); i++)
    {
        verticalLayout->removeWidget(viewArray[i]);
        delete viewArray[i];
    }
    viewArray.clear();

    for(size_t i = 0; i < csv.header.size(); i++)
    {
        auto newView = new Viewer(this);
        newView->setTitle(QString::fromStdString(csv.header[i]));

        verticalLayout->addWidget(newView);
        viewArray.push_back(newView); // save for later use
    }

    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);

    page = 0;
}

void MainWindow::refreshViews()
{
    for(size_t i=0; i < viewArray.size(); i++)
        viewArray[i]->setContent(QString::fromStdString(csv.data[page][i]));

    pageLabel->setText(QString("%1/%2")
                       .arg(QString::number(page+1)) // human-like
                       .arg(QString::number(csv.data.size())));
}

void MainWindow::readCSV(const std::string& path, char delim)
{
    /* yet unstable */
    char delim_str[1] = { delim };

    std::ifstream csvfile(path);
    if(!csvfile)
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    csv.header.clear();
    csv.data.clear();

    std::string line;
    std::getline(csvfile, line);
    boost::split(csv.header, line, boost::is_any_of(delim_str));

    while(!csvfile.eof())
    {
        csv_t::string_v record;
        while(record.size() != csv.header.size() - 1)
        {
            std::getline(csvfile, line, delim);
            record.push_back(line);
        }
        std::getline(csvfile, line);
        record.push_back(line);

        csv.data.push_back(record);
    }

    setupViews();
    refreshViews();
}

void MainWindow::on_prevButton_clicked()
{
    if(page > 0)
    {
        --page;
        refreshViews();
    }
}

void MainWindow::on_nextButton_clicked()
{
    if(page < csv.data.size()-1)
    {
        ++page;
        refreshViews();
    }
}

void MainWindow::on_actionAlways_on_Top_triggered(bool checked)
{
    /* works on Windows */
    Qt::WindowFlags flags = this->windowFlags();

    if (checked)
        this->setWindowFlags(flags | Qt::WindowStaysOnTopHint);
    else
        this->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);

    this->show();
}

void MainWindow::on_actionFix_window_triggered(bool checked)
{
    /* [issue] doesn't work */
    QLayout* layout = this->layout();

    if(checked)
        layout->setSizeConstraint(QLayout::SetFixedSize);
    else
        layout->setSizeConstraint(QLayout::SetNoConstraint);

    this->setLayout(layout);
}
