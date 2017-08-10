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
#include <boost/tokenizer.hpp>

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

    readCSV(path.toStdString(), delim.toStdString());
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

void MainWindow::readCSV(const std::string& path, const std::string& delim_str)
{
    char delim = delim_str[0];

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
    /* here can be changed to tokenizer */
    boost::split(csv.header, line, boost::is_any_of(delim_str));

#if 1
    boost::escaped_list_separator<char> separator('\\', delim, '\"');

    csv_t::string_v record;
    std::string feed;

    bool inside_quotes = false;
    size_t last_quote = 0;

    while(std::getline(csvfile, line))
    {
        /*
         * deal with line breaks in quoted strings by SBHacker
         * http://mybyteofcode.blogspot.com/2010/11/parse-csv-file-with-embedded-new-lines.html
         */
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

        boost::tokenizer<boost::escaped_list_separator<char>> tokenizer(feed, separator);
        record.assign(tokenizer.begin(), tokenizer.end());

        feed.clear(); // clear here, next check could fail

        // checking for correctly parsed N fields per record
        if(record.size() < csv.header.size())
            continue;

        csv.data.push_back(record);
    }
#else /* old csv processor */
    while(!csvfile.eof())
    {
        csv_t::string_v record;
        for(size_t i = 0; i < csv.header.size() - 1; i++)
        {
            std::getline(csvfile, line, delim);
            record.push_back(line);
        }
        std::getline(csvfile, line);
        record.push_back(line);

        csv.data.push_back(record);
    }
#endif
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
