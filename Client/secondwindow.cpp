#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "QFileDialog"
#include "QJsonDocument"

#include <QTcpSocket>
#include <QtAlgorithms>


SecondWindow::SecondWindow(QString server_ip, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SecondWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Призрак");

    this->server_ip = server_ip;
    QString network_ip = this->get_network_address(this->server_ip);

    network = new Network(this->server_ip, network_ip);
    // Парсить server_ip и доставать маску
    // Пока по умолчанию
    network->set_network_mask(24);

    PC phisycal_connected_pc(network, server_ip);
    phisycal_connected_pc.set_mask(24);
    network->push_back_pc(phisycal_connected_pc);

    Interface phis_int("ens33", server_ip + "/24", network, nullptr, &network->get_connected_pc().front());
    phis_int.set_type(true);
    network->push_back_interface(phis_int);
    network->get_connected_pc()[0].get_connected_interfaces().push_back(&network->get_exists_interface()[0]);

//    ui->comboBox->addItem("Phisical_PC");
    ui->tabWidget->setCurrentIndex(0);

    // Нет необходимости добавлять Phis_pc, пока не реализовано несколько bridge
//    ui->comboBox_5->addItem("Phisical_PC");

    ui->comboBox_5->setCurrentIndex(-1);
    ui->comboBox_4->addItem("Phisical_PC");

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(onServerConected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSockDisc()));

    desc = new SaveDesc(this);
    connect(desc, SIGNAL(SaveDescription(QString)), this, SLOT(OnSaveDescForm(QString)));


//    QNetworkProxy proxy(QNetworkProxy::HttpProxy);
//    proxy.setPort(3128);

//    socket->setProxy(proxy);
    QNetworkProxy::setApplicationProxy(QNetworkProxy::DefaultProxy);


    socket->connectToHost(this->server_ip, 8888);
    socket->waitForConnected(3000);

    jErr = new QJsonParseError();

    is_saved = true;
    is_save_as = false;

    QRegExp ip_regular("^" + ip_reg
                    + "\\." + ip_reg
                    + "\\." + ip_reg
                    + "\\." + ip_reg
                    + "/\\d{1,2}" "$");
    QRegExpValidator* ipValidator = new QRegExpValidator(ip_regular, this);
    ui->ip_address_Edit->setValidator(ipValidator);
    ui->ip_address_Edit_2->setValidator(ipValidator);
    ui->lineEdit_2->setValidator(ipValidator);

    QRegExp name_regular(name_reg);
    QRegExpValidator* nameValidator = new QRegExpValidator(name_regular, this);
    ui->lineEdit->setValidator(nameValidator);
    ui->lineEdit_3->setValidator(nameValidator);
    ui->first_int->setValidator(nameValidator);
    ui->second_int->setValidator(nameValidator);
}

SecondWindow::~SecondWindow()
{
    delete ui;
    delete network;
}

// Обработчик создания линка

void SecondWindow::on_create_Link_clicked()
{
    // Написать условие, при успешном выполнении которого создастся Link

    if (ui->first_int->text().isEmpty() || ui->second_int->text().isEmpty()){
        QMessageBox::critical(this,"Ошибка","Оба параметра должны быть заполнены!");
        return;
    }
    if(ui->first_int->text() == ui->second_int->text())
    {
        QMessageBox::information(this, "Ошибка", "Нельзя создавать интерфейсы с одинаковыми именами!");
        return;
    }
    if(network->int_existence_check(ui->first_int->text()) || network->int_existence_check(ui->second_int->text()))
    {
        QMessageBox::information(this, "Ошибка", "Один из интерфесов с таким именем уже существует!");
        return;
    }
    Interface int_1(ui->first_int->text(), "", network, nullptr, nullptr);
    Interface int_2(ui->second_int->text(), "", network, nullptr, nullptr);
    network->push_back_interface(int_1);
    network->push_back_interface(int_2);

    if(!network->set_int_neighbors(&network->get_interface(ui->first_int->text()), &network->get_interface(ui->second_int->text())))
    {
        return;
    }
    ui->comboBox_3->addItem(ui->first_int->text());
    ui->comboBox_3->addItem(ui->second_int->text());
    ui->comboBox_8->addItem(ui->first_int->text());
    ui->comboBox_8->addItem(ui->second_int->text());
    // --------------- JSON -------------------
    QJsonObject jObj;
    jObj.insert("command", "create");
    jObj.insert("stack", "link");
    QJsonArray jArr;
    jArr.push_back(network->get_interface(ui->first_int->text()).toObject());
    jArr.push_back(network->get_interface(ui->second_int->text()).toObject());
    jObj.insert("interfaces", jArr);
    data = QJsonDocument(jObj).toJson();
    socket->write(data);
    //-----------------------------------------
    ui->first_int->clear();
    ui->second_int->clear();
    ui->first_int->setFocus();

    is_saved = false;
}

// Обработчик присоединения линка и компьютера

void SecondWindow::on_pushButton_4_clicked()
{
    // Написать условие, при успешном выполнении которого Link свяжется с NetNs

    if(ui->comboBox->currentText().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Нельзя выбрать пустой стек!");
        return;
    }
    if(ui->comboBox_3->currentText().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Нельзя выбрать пустой интерфейс!");
        return;
    }

//    if(ui->comboBox->currentText() == "Phisical_PC")
//    {
//        if(network->get_interface(ui->comboBox_3->currentText()).set_pc_connect(&network->get_connected_pc().first()))
//        {
//            network->get_connected_pc().first().get_connected_interfaces().push_back(&network->get_interface(ui->comboBox_3->currentText())); // найти интерфейс по имени
//            ui->comboBox_3->removeItem(ui->comboBox_3->currentIndex());
//            // Создние JSON документа и отправка его на сервер
//        }
//        else {
//            QMessageBox::critical(this, "Ошибка", "Невозможно соединить данный интерфес со стеком!");
//            return;
//        }

//    }
        if(network->get_interface(ui->comboBox_3->currentText()).set_vpc_connect(&network->get_vpc(ui->comboBox->currentText())))
        {
            network->get_vpc(ui->comboBox->currentText()).push_back_interface(network->get_interface(ui->comboBox_3->currentText()));

            // Создние JSON документа и отправка его на сервер
            QJsonObject jObj;
            jObj.insert("command", "connect");
            jObj.insert("stack", "vpc");
            jObj.insert("interface", network->get_interface(ui->comboBox_3->currentText()).toObject());
            data = QJsonDocument(jObj).toJson();
            socket->write(data);
            ui->comboBox_8->removeItem(ui->comboBox_3->currentIndex());
            ui->comboBox_3->removeItem(ui->comboBox_3->currentIndex());

        }
        else {
            QMessageBox::critical(this, "Ошибка", "Невозможно соединить данный интерфес со стеком!");
            return;
        }
    is_saved = false;
}

void SecondWindow::on_create_Bridge_clicked()
{
    // Написать условие, при успешном выполнении которого создастся мост и произойдет переподключение

    if(network->get_network_bridge())
    {
       QMessageBox::critical(this, "Ошибка", "В данной сети уже присутствует сетевой мост!");
       return;
    }
    Bridge* br = new Bridge(network, ui->lineEdit_3->text());
    //br->set_ip_address(network->get_interface("ens33").get_ip_address());

    if(network->get_connected_pc()[0].set_pc_bridge(br))
    {
        network->set_bridge(br);
    }
    ui->comboBox_7->addItem(network->get_network_bridge()->get_bridge_name());
    ui->comboBox_9->addItem(network->get_network_bridge()->get_bridge_name());
    ui->comboBox_11->addItem(network->get_network_bridge()->get_bridge_name());
    ui->lineEdit_3->clear();
    QJsonObject jObj;
    jObj.insert("command", "create");
    jObj.insert("stack", "bridge");
    jObj.insert("params", network->get_network_bridge()->toObject());
    // Отправление на сервер данных на создание bridge
    data = QJsonDocument(jObj).toJson();
    socket->write(data);
    //------------------------------------------------
    is_saved = false;
}



void SecondWindow::on_create_NetNS_clicked()
{
    if (!ui->lineEdit->text().isEmpty()){
        // Написать условие, при успешном выполнении которого добавится NetNS
        if(network->vpc_existence_check(ui->lineEdit->text()))
        {
            QMessageBox::information(this, "Ошибка", "Netns с таким именем уже существует!");
            return;
        }
        VPC vpc(ui->lineEdit->text(), network);
        vpc.set_pc_connected(&network->get_connected_pc()[0]);
        network->push_back_vpc(vpc);
        network->get_connected_pc().last().push_back_vpc(&network->get_connected_vpc().last());
//        network->get_connected_pc()[0].get_connected_vpc().push_back(&network->get_connected_vpc().last());
        ui->comboBox->addItem(ui->lineEdit->text());
        ui->comboBox_5->addItem(ui->lineEdit->text());
        ui->comboBox_4->addItem(ui->lineEdit->text());
        ui->comboBox_11->addItem(ui->lineEdit->text());

        //---------------------------------------------
        //Записать в сокет JSON - документ Netnsa
        QJsonObject jObj;
//        jObj = network->get_vpc(ui->lineEdit->text()).
        jObj.insert("command", "create");
        jObj.insert("stack", "vpc");
        jObj.insert("params", network->get_vpc(ui->lineEdit->text()).toObject());
        data = QJsonDocument(jObj).toJson();
        if(socket->state() != socket->UnconnectedState) {
            socket->write(data);
        }


        //--------------------------------------------
        ui->lineEdit->clear();
        ui->lineEdit->setFocus();
    }
    else{
        QMessageBox::critical(this,"Ошибка","Параметр имени должен быть заполнен!");
    }
    ui->comboBox_4->setCurrentIndex(-1);
    is_saved = false;
}

void SecondWindow::on_pushButton_5_clicked()
{
    // создать Json объект для отправки на сервер и получение обратно информации
    QJsonObject jObj;
    jObj.insert("command", "show");
    jObj.insert("stack", "vpc");
    data = QJsonDocument(jObj).toJson();
    socket->write(data);
}

void SecondWindow::on_comboBox_5_activated(const QString &arg1)
{
    ui->comboBox_2->clear();
    if(arg1 == "Phisical_PC")
    {
        for(const auto& interface: network->get_connected_pc()[0].get_connected_interfaces())
        {
            if(interface->get_int_name() != "ens33")
                ui->comboBox_2->addItem(interface->get_int_name());
        }
        if(ui->comboBox_2->currentIndex() == -1) {
            QMessageBox::information(this, "Предупреждение", "Необходимо добавить стеку интерфейс!");
            ui->comboBox_5->setCurrentIndex(-1);
            return;
        }
    }
    else {
        for(auto& interface: network->get_vpc(ui->comboBox_5->currentText()).get_int_connected())
        {
            ui->comboBox_2->addItem(interface.get_int_name());
        }
        if(ui->comboBox_2->currentIndex() == -1) {
            QMessageBox::information(this, "Предупреждение", "Необходимо добавить стеку интерфейс!");
            ui->comboBox_5->setCurrentIndex(-1);
            return;
        }
    }
}



void SecondWindow::on_assign_ip_address_clicked()
{
    if(ui->comboBox_2->currentText().isEmpty())
    {
        QMessageBox::information(this, "Информация", "Необходимо выбрать стек!");
        return;
    }
    if(ui->ip_address_Edit->text().length()) {    // QRegExp проверка вадидности строки адреса интерфейса
        if(ui->comboBox_5->currentText() == "Phisical_PC") {
            for(auto& interface: network->get_connected_pc()[0].get_connected_interfaces())
            {
                if(interface->get_int_name() == ui->comboBox_2->currentText())
                {
                    interface->set_ip_addr(ui->ip_address_Edit->text());
                    break;
                }
            }
            QJsonObject jObj;
            jObj.insert("command", "assign");
            jObj.insert("stack", "interface");
            jObj.insert("interface", network->get_interface(ui->comboBox_2->currentText()).toObject());
            data = QJsonDocument(jObj).toJson();
            socket->write(data);
        }
        else {
            for(auto& vpc_int: network->get_vpc(ui->comboBox_5->currentText()).get_int_connected())
            {
                if(vpc_int.get_int_name() == ui->comboBox_2->currentText())
                {
                    vpc_int.set_ip_addr(ui->ip_address_Edit->text());
                    network->get_interface(ui->comboBox_2->currentText()).set_ip_addr(ui->ip_address_Edit->text());
                    break;
                }
            }
            QJsonObject jObj;
            jObj.insert("command", "assign");
            jObj.insert("stack", "interface");
            jObj.insert("interface", network->get_interface(ui->comboBox_2->currentText()).toObject());
            data = QJsonDocument(jObj).toJson();
            socket->write(data);
            ui->ip_address_Edit->clear();
            ui->comboBox_5->setCurrentIndex(-1);
            ui->comboBox_2->setCurrentIndex(-1);
        }
    }
    else {
        QMessageBox::information(this, "Внимание!", "Необходимо ввести ip - адрес выбранного вами интерфейса!");
    }
    is_saved = false;
}

void SecondWindow::on_pushDesign_Route_clicked()
{
    // QRegExp проверка на валидность значения comboBox_4
    if(ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::information(this, "Предупреждение", "Необходимо ввести сеть назначения для маршрута!");
        return;
    }

    if(ui->comboBox_4->currentText().length() != 0 && ui->comboBox_6->currentText().length() != 0) {
        if(ui->comboBox_4->currentText() != "Phisical_PC") {
            if(network->get_network_bridge()->get_bridge_name() == ui->comboBox_6->currentText()) {
                // маршрут с шлюзом мостом
                Route route(ui->lineEdit_2->text(), network->get_network_bridge(), network, &network->get_vpc(ui->comboBox_4->currentText()));
                network->push_back_route(route);
                QJsonObject jObj;
                jObj.insert("command", "create");
                jObj.insert("stack", "route");
                jObj.insert("route_params", network->get_routes().last().toObject());
                if(network->get_routes().last().get_vpc_route()) {
                    jObj.insert("container", "vpc");
                    jObj.insert("params", network->get_routes().last().get_vpc_route()->toObject());
                }
                data = QJsonDocument(jObj).toJson();
                socket->write(data);
            }
            else {
                // маршрут с шлюзом интерфейсом
                Route route(ui->lineEdit_2->text(), &network->get_interface(ui->comboBox_6->currentText()), network, &network->get_vpc(ui->comboBox_4->currentText()));
                network->push_back_route(route);
                QJsonObject jObj;
                jObj.insert("command", "create");
                jObj.insert("stack", "route");
                jObj.insert("route_params", network->get_routes().last().toObject());
                if(network->get_routes().last().get_vpc_route()) {
                    jObj.insert("container", "vpc");
                    jObj.insert("params", network->get_routes().last().get_vpc_route()->toObject());
                }
                data = QJsonDocument(jObj).toJson();
                socket->write(data);
            }
        }
        else {
            Route route(ui->lineEdit_2->text(), network->get_network_bridge(), network, nullptr ,&network->get_connected_pc().last());
            network->push_back_route(route);
            QJsonObject jObj;
            jObj.insert("command", "create");
            jObj.insert("stack", "route");
            jObj.insert("route_params", network->get_routes().last().toObject());
            data = QJsonDocument(jObj).toJson();
            socket->write(data);
        }
        ui->comboBox_4->setCurrentIndex(-1);
        ui->comboBox_6->setCurrentIndex(-1);
        ui->lineEdit_2->clear();
        ui->lineEdit_2->setFocus();
    }
}

void SecondWindow::on_comboBox_4_activated(const QString &arg1)
{
    ui->comboBox_6->clear();
    if(network->get_network_bridge()) {
        if(arg1 == "Phisical_PC") {
            ui->comboBox_6->addItem(network->get_network_bridge()->get_bridge_name());
            // Добавляем все интерфейсы, другой конец которых присоединен к мосту
            for(auto& veth: network->get_network_bridge()->get_int())
            {
                ui->comboBox_6->addItem(veth->get_neighbor()->get_int_name());
            }
        }
        else {
            // добавить все интерфейсы из моста

            bool bridge_veths = false;
            for(auto& interface: network->get_vpc(arg1).get_int_connected()) {
                if(interface.get_neighbor()->get_connected_bridge() && bridge_veths == false) {
                    // если сосед вставлен в мост, то необходимо пробежаться по всем соседям интерфейсам моста и добавить их, кроме interface
                    for(auto& veth: network->get_network_bridge()->get_int())
                    {
                        if(veth->get_neighbor()->get_int_name() != interface.get_int_name())
                        {
                            ui->comboBox_6->addItem(veth->get_neighbor()->get_int_name());
                        }
                    }
                    bridge_veths = true;
                    ui->comboBox_6->addItem(network->get_network_bridge()->get_bridge_name());
                }
                else {
                    ui->comboBox_6->addItem(interface.get_neighbor()->get_int_name());
                }
            }

        }
    }
    else {
        if(arg1 == "Phisical_PC") {
            QMessageBox::information(this, "Предупреждение" , "Необходимо создать мост!");
            return;
        }
        else {
            for(auto& interface: network->get_vpc(arg1).get_int_connected()) {
                ui->comboBox_6->addItem((interface.get_neighbor()->get_int_name()));
            }
        }
    }
}

void SecondWindow::on_tabWidget_currentChanged(int index)
{
    ui->comboBox_4->setCurrentIndex(-1);
    ui->comboBox_6->clear();
    ui->comboBox_5->setCurrentIndex(-1);
    ui->comboBox_2->clear();
    ui->comboBox_11->setCurrentIndex(-1);
    ui->comboBox_10->clear();
}

void SecondWindow::onServerConected()
{
    QMessageBox::information(this, "Информация", "Успешное подключение к серверу!");
}

void SecondWindow::onReadyRead()
{
    //-- Прием и парсинг сообщений
    data.clear();
    data = socket->readAll();
    QJsonObject jAnswer = QJsonDocument::fromJson(data, jErr).object();
    if(jErr->error != jErr->NoError) {
        ui->listWidget->addItem("Can't convert received JSON message!");
        return;
    }
    QString command = jAnswer["command"].toString();
    if(jAnswer["answer"].isArray()) {
        // Обработчик ответа пакетной загрузки
        QJsonArray jArr = jAnswer["answer"].toArray();
        for(auto jValue: jArr) {
            QString ans = jValue.toString();
            ui->listWidget->addItem(ans);
        }
    }
    QString answer = jAnswer["answer"].toString();
    ui->listWidget->addItem(answer);
}

void SecondWindow::onSockDisc()
{
    QMessageBox::information(this, "Информация", "Вы отключены от сервера!");
//    this->close();
//    socket->deleteLater();
}

QString SecondWindow::get_network_address(QString ip_address)
{
    QString network_ip = ip_address;
    int pos(0);
    pos = ip_address.lastIndexOf(".");
    network_ip = ip_address.mid(0, pos+1);
    network_ip.append("0");
    return network_ip;
}

void SecondWindow::on_Disconnect_Button_clicked()
{
    QJsonObject jObj;
    jObj.insert("command", "disconnect");
    data = QJsonDocument(jObj).toJson();
    if(socket)
        socket->write(data);
    ui->Disconnect_Button->setEnabled(false);
}

void SecondWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgbx(this);
    QMessageBox svmsgbx(this);
    msgbx.setText("Вы действительно хотете закрыть текущую конфигурацию?");
    msgbx.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgbx.setDefaultButton(QMessageBox::No);

    int res_1 = msgbx.exec();
    if(res_1 == QMessageBox::Yes) {
        disconnect(socket, SIGNAL(disconnected()), this, SLOT(onSockDisc()));
        if(!is_saved) {
            svmsgbx.setText("У вас есть несохраненная версия конфигурации сети.\n"
                            "Хотите ее сохранить?");
            svmsgbx.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            svmsgbx.setDefaultButton(QMessageBox::No);
            int res_2 = svmsgbx.exec();
            if(res_2 == QMessageBox::Yes) {
                emit ui->action_2->triggered();
            }
        }
        QJsonObject jObj;
        jObj.insert("command", "disconnect");
        data = QJsonDocument(jObj).toJson();
        socket->write(data);
        socket->deleteLater();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SecondWindow::on_action_triggered()
{
    QFileDialog* f_d = new QFileDialog(this);
    open_file_name = f_d->getOpenFileName();
    if(network->LoadConfigFromFile(open_file_name)) {
        QMessageBox::information(this, "Информация", "Конфинурация виртуальной сети успешно загружена!");
        // Функции добавления в ComboBox стеков и интерфейсов
        // Добавить в ComboBox мост
        for(auto& vpc: network->get_connected_vpc())
        {
            ui->comboBox->addItem(vpc.get_vpc_name());
            ui->comboBox_5->addItem(vpc.get_vpc_name());
            ui->comboBox_4->addItem(vpc.get_vpc_name());
            ui->comboBox_11->addItem(vpc.get_vpc_name());
        }
        if(network->get_network_bridge()) {
            ui->comboBox_7->addItem(network->get_network_bridge()->get_bridge_name());
            ui->comboBox_9->addItem(network->get_network_bridge()->get_bridge_name());
            ui->comboBox_11->addItem(network->get_network_bridge()->get_bridge_name());
        }
        for(auto& veth: network->get_exists_interface()) {
            if(veth.get_connected_bridge() == nullptr && veth.get_connected_vpc() == nullptr && veth.is_phisical() == false) {
                ui->comboBox_3->addItem(veth.get_int_name());
                ui->comboBox_8->addItem(veth.get_int_name());
            }
        }
    }
}

void SecondWindow::on_action_2_triggered()
{
    data.clear();

    desc->show();
}

void SecondWindow::OnSaveDescForm(QString description)
{
    if(is_save_as) {
        open_file_name.clear();
        is_save_as = false;
    }
    if(open_file_name.isEmpty()) {
        QFileDialog* f_d = new QFileDialog(this);
        open_file_name = f_d->getOpenFileName();
    }
    QFile save_file(open_file_name);
    if(!save_file.open(QIODevice::ReadWrite)) {
        QMessageBox::warning(this, "Предупреждение", "Не удалось открыть выбранный файл!");
        return;
    }
    QJsonObject jObj = network->SaveConfigToFile(description);
    data = QJsonDocument(jObj).toJson();
    if(!save_file.write(data)) {
        QMessageBox::information(this, "Информация", "Конфигурация виртуальной сети успешно сохранена!");
        save_file.close();
        return;
    }
    is_saved = true;
    save_file.close();

}

void SecondWindow::on_pushButton_6_clicked()
{
    if(ui->comboBox_7->currentText().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Нельзя выбрать пустой стек!");
        return;
    }
    if(ui->comboBox_8->currentText().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Нельзя выбрать пустой интерфейс!");
        return;
    }

    if(!network->get_network_bridge())
        {
            QMessageBox::information(this, "Ошибка", "Для присоединения интерфейса к физическому компьютеру необходимо создать мост!");
            return;
        }
        if(network->get_interface(ui->comboBox_8->currentText()).set_connected_bridge())
        {
            network->get_network_bridge()->push_back_int(&network->get_interface(ui->comboBox_8->currentText()));

            // Создние JSON документа и отправка его на сервер
            QJsonObject jObj;
            jObj.insert("command", "connect");
            jObj.insert("stack", "bridge");
            jObj.insert("interface", network->get_interface(ui->comboBox_8->currentText()).toObject());
            data = QJsonDocument(jObj).toJson();
            socket->write(data);
            ui->comboBox_3->removeItem(ui->comboBox_8->currentIndex());
            ui->comboBox_8->removeItem(ui->comboBox_8->currentIndex());
            is_saved = false;
        }
        else {
            QMessageBox::critical(this, "Ошибка", "Невозможно соединить данный интерфес со стеком!");
            return;
        }
}

void SecondWindow::on_assign_ip_address_2_clicked()
{
    if(ui->comboBox_9->currentText().isEmpty())
    {
        QMessageBox::information(this, "Информация", "Необходимо выбрать стек!");
        return;
    }
    if(ui->ip_address_Edit_2->text().length()) {    // QRegExp проверка вадидности строки адреса интерфейса
        network->get_network_bridge()->set_ip_address(ui->ip_address_Edit_2->text());
        QJsonObject jObj;
        jObj.insert("command", "assign");
        jObj.insert("stack", "bridge");
        jObj.insert("params", network->get_network_bridge()->toObject());
        data = QJsonDocument(jObj).toJson();
        socket->write(data);
        is_saved = false;

    }
    else {
        QMessageBox::information(this, "Внимание!", "Необходимо ввести ip - адрес выбранного вами интерфейса!");
    }
    ui->ip_address_Edit_2->clear();
    // Три параметра скрипта: имя_стека, айпи, имя_интерфейса
}

void SecondWindow::on_pushButton_clicked()
{
    data.clear();
    QString description = "Load_Config_On_Server";
    QJsonObject jObj = network->SaveConfigToFile(description);
    jObj.insert("command", "create");
    jObj.insert("stack", "network");
    data = QJsonDocument(jObj).toJson();
    socket->write(data);
}

void SecondWindow::on_action_3_triggered()
{

    desc->show();
    is_save_as = true;
}

                    // traffic control

void SecondWindow::on_comboBox_11_activated(const QString &arg1)
{
    ui->comboBox_10->clear();
    if(network->get_network_bridge()) {
        if(arg1 == network->get_network_bridge()->get_bridge_name())
        {
            for(auto interface: network->get_network_bridge()->get_int())
                ui->comboBox_10->addItem(interface->get_int_name());
        }
        else {
            for(auto& interface: network->get_vpc(arg1).get_int_connected())
                ui->comboBox_10->addItem(interface.get_int_name());
        }
    }
    else {
        for(auto& interface: network->get_vpc(ui->comboBox_11->currentText()).get_int_connected())
            ui->comboBox_10->addItem(interface.get_int_name());
    }
    if(!ui->comboBox_10->count()) {
        QMessageBox::information(this, "Предупреждение", "Необходимо добавить стеку интерфейс!");
        ui->comboBox_11->setCurrentIndex(-1);
        return;
    }
}

void SecondWindow::on_pushButton_2_clicked()
{
    if(ui->comboBox_11->currentIndex() == -1 || ui->comboBox_10->currentIndex() == -1)
    {
        QMessageBox::warning(this, "Предупреждение", "Необходимо добавить стеку интерфейс!");
        return;
    }
//     проверка на наличие у интерфейса traffic_rules
    if(network->get_interface(ui->comboBox_10->currentText()).get_tc()) {
        network->get_tc(&network->get_interface(ui->comboBox_10->currentText())).set_param(pair(ui->spinBox->value(), ui->spinBox_2->value()), pair(ui->spinBox_3->value(), ui->spinBox_4->value()),
                                                                                           pair(ui->spinBox_5->value(), ui->spinBox_6->value()), ui->spinBox_7->value());
        QJsonObject jObj;
        jObj.insert("command", "assign");
        jObj.insert("stack", "tc");
        jObj.insert("params", network->get_tc(&network->get_interface(ui->comboBox_10->currentText())).toObject());
        data = QJsonDocument(jObj).toJson();
        socket->write(data);
    }
    else {
        tc traffic_control(&network->get_interface(ui->comboBox_10->currentText()),
           pair(ui->spinBox->value(), ui->spinBox_2->value()), pair(ui->spinBox_3->value(), ui->spinBox_4->value()),
           pair(ui->spinBox_5->value(), ui->spinBox_6->value()), ui->spinBox_7->value());
        traffic_control.set_is_new();
        network->push_back_tc(traffic_control);
        network->get_interface(ui->comboBox_10->currentText()).set_tc(&network->get_tc().last());

        QJsonObject jObj;
        jObj.insert("command", "create");
        jObj.insert("stack", "tc");
        jObj.insert("params", network->get_tc(&network->get_interface(ui->comboBox_10->currentText())).toObject());
        data = QJsonDocument(jObj).toJson();
        socket->write(data);
    }
    ui->comboBox_11->setCurrentIndex(-1);
    ui->comboBox_10->setCurrentIndex(-1);

//    QList<QSpinBox*> spin_boxs = ui->tab_4->findChildren<QSpinBox*>();
//    for(auto & spin_box: spin_boxs) {
//        spin_box->setValue(0);
//    }
}


