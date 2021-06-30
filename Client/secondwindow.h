#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QTextEdit>
#include <QDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include <QNetworkProxy>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QString>
#include <QWidget>
#include <QPair>

#include "Network.h"
#include "VPC.h"
#include "PC.h"
#include "Interface.h"
#include "bridge.h"
#include "savedesc.h"

typedef QPair<int, int> pair;

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SecondWindow(QString server_ip, QWidget *parent = nullptr);
    ~SecondWindow();


private slots:

    void on_create_Link_clicked();

    void on_pushButton_4_clicked();

    void on_create_Bridge_clicked();

    void on_create_NetNS_clicked();

    void on_pushButton_5_clicked();

    void on_comboBox_5_activated(const QString &arg1);

    void on_assign_ip_address_clicked();

    void on_pushDesign_Route_clicked();

    void on_comboBox_4_activated(const QString &arg1);

    void on_tabWidget_currentChanged(int index);

    void onServerConected();

    void onReadyRead();

    void onSockDisc();

    void on_Disconnect_Button_clicked();

    void closeEvent(QCloseEvent* event);

    void on_action_triggered();

    void on_action_2_triggered();

    void OnSaveDescForm(QString description);

    void on_pushButton_6_clicked();

    void on_assign_ip_address_2_clicked();

    void on_pushButton_clicked();

    void on_action_3_triggered();

    void on_comboBox_11_activated(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::SecondWindow *ui;
    SaveDesc* desc;
    Network* network;
    QTcpSocket *socket;
    QByteArray data;
    QString server_ip;
    QJsonParseError *jErr;
    QString open_file_name;
    bool is_saved;
    bool is_save_as;

    QString get_network_address(QString ip_address);

//    QString ip_addr_regexp = "([1-9]|[1-9]\d|1\d{2}|2[0-4]\d|25[0-5])(\.(\d|[1-9]\d|1\d{2}|2[0-4]\d|25[0-5])){3}\/\d+";

};

#endif // SECONDWINDOW_H
