#include "fielddialog.h"
#include "ui_fielddialog.h"

FieldDialog::FieldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FieldDialog)
{
    ui->setupUi(this);

    widget = new QWidget(this);
    layout = new QVBoxLayout();
    widget->setLayout(layout);
    ui->scrollArea->setWidget(widget);
}

FieldDialog::~FieldDialog()
{
    for(auto& checkBox : checkBoxes)
        delete checkBox;
    checkBoxes.clear();

    delete layout;
    delete widget;
    delete ui;
}

void FieldDialog::setCheckBoxes(const std::vector<std::string> &fields)
{
    for(const auto& field : fields)
    {
        checkBoxes.push_back(new QCheckBox(QString::fromStdString(field), this));
        layout->addWidget(checkBoxes.back());
    }
}

std::vector<size_t> FieldDialog::getFieldList(const std::vector<std::string> &fields, QWidget* parent)
{
    FieldDialog* fView = new FieldDialog(parent);
    fView->setCheckBoxes(fields);
    fView->exec();

    std::vector<size_t> indexes;
    for(size_t i = 0; i < fView->checkBoxes.size(); i++)
        if(!fView->checkBoxes[i]->isChecked())
            indexes.push_back(i);

    return indexes;
}
