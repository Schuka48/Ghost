#include "description.h"
#include "ui_description.h"

Description::Description(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Description)
{
    ui->setupUi(this);
}

Description::~Description()
{
    delete ui;
}

void Description::on_pushButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    emit OnSaveDescription(text);
    this->close();
}
