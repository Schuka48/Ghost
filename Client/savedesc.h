#ifndef SAVEDESC_H
#define SAVEDESC_H

#include <QDialog>

namespace Ui {
class SaveDesc;
}

class SaveDesc : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDesc(QWidget *parent = nullptr);
    ~SaveDesc();

private slots:
    void on_pushButton_clicked();

signals:
    void SaveDescription(QString description);

private:
    Ui::SaveDesc *ui;
};

#endif // SAVEDESC_H
