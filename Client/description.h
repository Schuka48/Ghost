#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <QDialog>

#include "secondwindow.h"

namespace Ui {
class Description;
}

class Description : public QDialog
{
    Q_OBJECT

public:
    explicit Description(QWidget *parent = nullptr);
    ~Description();

private slots:
    void on_pushButton_clicked();

signals:
    void OnSaveDescription(QString description);

private:
    Ui::Description *ui;
};

#endif // DESCRIPTION_H
