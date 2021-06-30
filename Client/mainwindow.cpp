#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(SockDisc()));
//    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidjetItem *)), this, SLOT(OnButtonSend(QListWidgetItem *)));
//    connect(this, SIGNAL(SendData(int)), window, SLOT(DataReceived(int)));

    //connect(ui->listWidget,SIGNAL(mouseDoubleClickEvent()),window,SIGNAL(Item_Count()));
    ui->lineEdit->setText("10.33.74.12");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString text_out;
    text_out = "IP адрес агента: ";
    ui->listWidget->addItem(text_out + ui->lineEdit->text());
    servers_ip.push_back(ui->lineEdit->text());
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    // Необходимо по индексу итема в QListWidget находить ip в servers_ip
    QString server_address = item->text().split(' ')[3];
    window = new SecondWindow(server_address);
    window->show();
    current_agent_index = ui->listWidget->row(item);

}

void MainWindow::SockDisc(){
    socket->deleteLater();
}


void MainWindow::onReadyRead()
{

}

void MainWindow::OnButtonSend(QListWidgetItem *item){
    emit SendData(ui->listWidget->row(item));
}


