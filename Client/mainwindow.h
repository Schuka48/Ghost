#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <secondwindow.h>
#include <description.h>
#include <QListWidget>
#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include <QMessageBox>
#include <QModelIndex>
#include <QTimer>
#include <QListWidgetItem>
#include <QList>

#include "Network.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpSocket *socket;
    QByteArray data;
    int current_agent_index;

signals:
    void SendData(int count);

public slots:

    void onReadyRead();
    void SockDisc();
    void OnButtonSend(QListWidgetItem *item);

private slots:
    void on_pushButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    SecondWindow *window;
    QList<QString> servers_ip;
};
#endif // MAINWINDOW_H
