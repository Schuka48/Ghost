#include "savedesc.h"
#include "ui_savedesc.h"

SaveDesc::SaveDesc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveDesc)
{
    ui->setupUi(this);
}

SaveDesc::~SaveDesc()
{
    delete ui;
}

void SaveDesc::on_pushButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    emit SaveDescription(text);
    this->close();

}
