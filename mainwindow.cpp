#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QScrollArea>

#include <fstream>
#include <iostream>
#include <boost/tokenizer.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("csv_pager - v0.0.3");

    viewWidget = new QWidget(this);
    verticalLayout = new QVBoxLayout(this);
    viewWidget->setLayout(verticalLayout);
    ui->scrollArea->setWidget(viewWidget);

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

    delete verticalLayout;
    delete viewWidget;
    delete ui;
}

void MainWindow::on_actionOpen_CSV_triggered()
{
//    QString path = QFileDialog::getOpenFileName(this, "Open CSV", QString(),
//                                                    "CSV (*.csv *.tsv *.txt)");
    FileDialog* dialog = new FileDialog(this);
    if (!dialog->exec())
        return;
    QString path = dialog->selectedFiles().first();
    bool default_delim = dialog->delimBox->isChecked();
    if(path.isEmpty())
        return;

    QString delim_str;
    if(!default_delim)
    {
        delim_str = QInputDialog::getText(this, "Input delimiter", "Input delimiter:");
        if(delim_str.isEmpty())
            return;
    }
    else
        delim_str = ",";

    readCSV(path.toStdString(), delim_str.toStdString()[0]);
    setupViews();
    refreshViews();
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
        viewArray.push_back(newView);
    }

    ui->prevButton->setEnabled(true);
    ui->nextButton->setEnabled(true);

    page = 0;
}

void MainWindow::refreshViews()
{
    for(size_t i = 0; i < viewArray.size(); i++)
        viewArray[i]->setContent(QString::fromStdString(csv.data[page][i]));

    ui->pageLabel->setText(QString("%1/%2")
                       .arg(QString::number(page + 1)) // human-like
                       .arg(QString::number(csv.data.size())));
}

void MainWindow::readCSV(const std::string& path, char delim)
{
    std::ifstream csvfile(path);
    if(!csvfile)
    {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    csv.header.clear();
    csv.data.clear();

    boost::escaped_list_separator<char> separator('\\', delim, '\"');

    std::string line;
    std::getline(csvfile, line);
    boost::tokenizer<boost::escaped_list_separator<char>> tokenizer(line, separator);
    csv.header.assign(tokenizer.begin(), tokenizer.end());

    std::string feed;
    bool inside_quotes = false;
    size_t last_quote = 0;

    while(std::getline(csvfile, line))
    {
        /**
         * deal with line breaks in quoted strings by SBHacker
         * http://mybyteofcode.blogspot.com/2010/11/parse-csv-file-with-embedded-new-lines.html
         **/
        last_quote = line.find_first_of('"');
        while(last_quote != std::string::npos)
        {
            inside_quotes = !inside_quotes;
            last_quote = line.find_first_of('"', last_quote + 1);
        }

        feed.append(line);

        if(inside_quotes)
        {
            feed.push_back('\n');
            continue;
        }

        csv_t::string_v record;
        boost::tokenizer<boost::escaped_list_separator<char>> tokenizer(feed, separator);
        record.assign(tokenizer.begin(), tokenizer.end());

        feed.clear(); // clear here, next check could fail

        // checking for correctly parsed N fields per record
        if(record.size() < csv.header.size())
            continue;

        csv.data.push_back(record);
    }
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
    /* TODO */
}
