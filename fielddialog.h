#ifndef FIELDDIALOG_H
#define FIELDDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QVBoxLayout>

namespace Ui {
class FieldDialog;
}

class FieldDialog : private QDialog
{
    Q_OBJECT

public:
    static std::vector<size_t> getFieldList(const std::vector<std::string> &fields, QWidget* parent);

private:
    explicit FieldDialog(QWidget *parent = 0);
    ~FieldDialog();

    Ui::FieldDialog *ui;

    QWidget* widget;
    QVBoxLayout* layout;

    void setCheckBoxes(const std::vector<std::string>& fields);
    std::vector<QCheckBox*> checkBoxes;
};

#endif // FIELDDIALOG_H
