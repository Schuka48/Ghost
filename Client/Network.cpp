#include "Network.h"

uint Network::net_id = 0;

QJsonObject Network::getJsonNetwork()
{
    QJsonObject jObj;
    jObj.insert("id", int(this->get_id()));
    jObj.insert("network_ip", this->get_network_ip());
    jObj.insert("network_mask", "/" + QString::number(this->get_net_mask()));
    jObj.insert("network_phisical_pc", getJsonPhis());
    jObj.insert("network_bridge", getJsonBridge());
    jObj.insert("network_vpcs", getJsonVPC());
    jObj.insert("network_interfaces", getJsonInterfaces());
    jObj.insert("network_routes", getJsonRoute());
    jObj.insert("network_tc", getJsonTC());

    return jObj;
}

QJsonArray Network::getJsonPhis()
{
    QJsonArray jArr;
    for(auto& pc: this->connected_pc) {
        QJsonObject jObj;
        jObj.insert("id", int(pc.get_pc_id()));
        jObj.insert("ip_addr", pc.get_ip_addr());
        jObj.insert("net_mask", "/" + QString::number(pc.get_mask()));
        jArr.append(jObj);
    }
    return jArr;
}

QJsonObject Network::getJsonBridge()
{
    QJsonObject jObj;
    if(this->bridge) {
        jObj.insert("id", int(this->bridge->get_id()));
        jObj.insert("name", this->bridge->get_bridge_name());
        jObj.insert("ip_addr", this->bridge->get_ip_address());
    }
    return jObj;
}

QJsonArray Network::getJsonVPC()
{
    QJsonArray jArr;
    for(auto& vpc: this->connected_vpc) {
        QJsonObject jObj;
        jObj.insert("id", int(vpc.get_id()));
        jObj.insert("name", vpc.get_vpc_name());
        jObj.insert("pc_id", int(vpc.get_connected_pc()->get_pc_id()));
        jArr.append(jObj);
    }
    return jArr;
}

QJsonArray Network::getJsonInterfaces()
{
    QJsonArray jArr;
    for(auto& interface: this->net_int) {
        if(interface.is_phisical()) {
            continue;
        }
        QJsonObject jObj;
        jObj.insert("id", int(interface.get_id()));
        jObj.insert("name", interface.get_int_name());
        jObj.insert("ip_addr", interface.get_ip_address());
        jObj.insert("neighbor", interface.get_neighbor()->get_int_name());
        QString pc_addr, vpc_name, br_name;
        if(interface.get_connected_vpc()) {
            jObj.insert("vpc_name", interface.get_connected_vpc()->get_vpc_name());
            jObj.insert("br_name", br_name);
            jObj.insert("pc_name", pc_addr);
        }
        else if(interface.get_connected_bridge())
        {
            jObj.insert("br_name", interface.get_connected_bridge()->get_bridge_name());
            jObj.insert("pc_name", pc_addr);
            jObj.insert("vpc_name", vpc_name);
        }
        else if(interface.get_connected_pc()) {
            jObj.insert("pc_name", interface.get_connected_pc()->get_ip_addr());
            jObj.insert("vpc_name", vpc_name);
            jObj.insert("br_name", br_name);
        }
        else {
            jObj.insert("pc_name", pc_addr);
            jObj.insert("vpc_name", vpc_name);
            jObj.insert("br_name", br_name);
        }
        jArr.append(jObj);
    }
    return jArr;
}

QJsonArray Network::getJsonRoute()
{
    QJsonArray jArr;

    for(auto& route: this->exists_routes) {
        QJsonObject jObj;
        jObj.insert("id", int(route.get_id()));
        jObj.insert("net_destination", route.get_net_destination());
        if(route.get_pc_route()) {
            jObj.insert("pc_route", true);
            jObj.insert("vpc_route", false);
        }
        else if(route.get_vpc_route()) {
            jObj.insert("pc_route", false);
            jObj.insert("vpc_route", true);
            QJsonObject jParam;
            jParam.insert("name", route.get_vpc_route()->get_vpc_name());
            jObj.insert("vpc_param", jParam);
        }
        QString type_gateway;
        if(route.get_br_gateway()) {
            type_gateway = "bridge";
            jObj.insert("gateway_name", route.get_br_gateway()->get_bridge_name());
            jObj.insert("gateway_ip", route.get_br_gateway()->get_ip_address().split('/')[0]);
        }
        else if(route.get_int_gateway()) {
            type_gateway = "veth";
            jObj.insert("gateway_name", route.get_int_gateway()->get_int_name());
            jObj.insert("gateway_ip", route.get_int_gateway()->get_ip_address().split('/')[0]);
        }
        jObj.insert("gateway_type", type_gateway);
        jArr.append(jObj);
    }

    return jArr;
}

QJsonArray Network::getJsonTC()
{
    QJsonArray jArr;
    for(auto& tc: exists_tc) {
        QJsonObject jObj = tc.toObject();
        jArr.append(jObj);
    }
    return jArr;
}

uint Network::get_id()
{
    return this->id;
}

Network::Network(QString phis_ip_addr, QString network_ip)
{
    this->phis_ip_addr = phis_ip_addr;
    this->id = ++net_id;
    this->network_ip = network_ip;
    this->bridge = nullptr;
}
Network::Network()
{
   this->id = ++net_id;
    this->network_ip = "";
}

QList<VPC>& Network::get_connected_vpc()
{
    return this->connected_vpc;
}

QList<PC> &Network::get_connected_pc()
{
    return this->connected_pc;
}

QList<Interface> &Network::get_exists_interface()
{
    return net_int;
}

QList<tc> &Network::get_tc()
{
    return this->exists_tc;
}

void Network::push_back_pc(const PC& phis_pc)
{
    connected_pc.push_back(phis_pc);
}

void Network::push_back_interface(const Interface &interface)
{
    net_int.push_back(interface);
}

void Network::push_back_vpc(const VPC &virtual_pc)
{
    connected_vpc.push_back(virtual_pc);
}

void Network::push_back_route(const Route &route)
{
    this->exists_routes.push_back(route);
}

void Network::push_back_tc(const tc &traffic_control)
{
    this->exists_tc.push_back(traffic_control);
}

void Network::set_bridge(Bridge *br)
{
    // Возможно придется писать перегрузку "=" для Bridge
    this->bridge = br;
}

QString Network::get_network_ip()
{
    return network_ip;
}

Interface &Network::get_interface(QString name)
{
    for(auto& interface: net_int)
    {
        if(interface.get_int_name() == name)
            return interface;
    }
    // Вернуть особый объект - не существующий!
}

bool Network::vpc_existence_check(QString vpc_name)
{
    // Почему не работает const auto& ???
    for(auto& virtual_pc: connected_vpc)
    {
        if(virtual_pc.get_vpc_name() == vpc_name)
            return true;
    }
    return false;
}

bool Network::int_existence_check(QString int_name)
{
    for(auto& interface: net_int)
    {
        if(interface.get_int_name() == int_name)
            return true;
    }
    return false;
}

QList<Route> &Network::get_routes()
{
    return this->exists_routes;
}

bool Network::LoadConfigFromFile(QString &file_name)
{
    QFile file(file_name);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "Информация", "Невозможно открыть выбранный файл!");
        return false;
    }

    QByteArray data;
    data = file.readAll();
    file.close();

    QJsonObject jObj;
    QJsonParseError* jErr = new QJsonParseError();
    jObj = QJsonDocument::fromJson(data, jErr).object();
    if(jErr->error != QJsonParseError::NoError) {
        QMessageBox::warning(nullptr, "Предупреждение", "Не удалось загрузить конфигурацию: " + jErr->errorString());
        return false;
    }

    QString desc = jObj["description"].toString();
    QDate date = QDate::fromString(jObj["date"].toString(), "dd.MM.yyyy");
    QTime time = QTime::fromString(jObj["time"].toString(), "hh:mm:ss");

    QJsonObject jNet = jObj["network"].toObject();
    this->id = uint(jNet["id"].toInt());
    this->network_ip = jNet["network_ip"].toString();
    this->network_mask = uint(jNet["network_mask"].toString().split('/')[1].toInt());

    QJsonArray jPhis = jNet["network_phisical_pc"].toArray();
    for(auto jValue: jPhis) {
        QJsonObject jPC = jValue.toObject();
        QString phis_ip_addr = jPC["ip_addr"].toString();
        PC pc(this, phis_ip_addr);
        pc.set_mask(uint(jPC["net_mask"].toString().split('/')[1].toInt()));

        // Тут нужно посмотреть, где добавлять физику в сеть!

//        this->push_back_pc(pc);
    }

    // Загрузка мостов из конфиг файла - пока что только один коммутатор

    QJsonObject jBr = jNet["network_bridge"].toObject();
    QString br_name = jBr["name"].toString();
    QString br_ip_addr = jBr["ip_addr"].toString();
    Bridge* br = new Bridge(this, br_name);
    br->set_ip_address(br_ip_addr);
    this->bridge = br;
    this->get_connected_pc().last().set_pc_bridge(this->get_network_bridge());

    // Загрузка в сеть виртуальных стеков

    QJsonArray jVPCs = jNet["network_vpcs"].toArray();
    for(auto jValue: jVPCs) {
        QJsonObject jVPC = jValue.toObject();
        QString vpc_name = jVPC["name"].toString();
        VPC vpc(vpc_name, this);
        vpc.set_pc_connected(&this->get_connected_pc().last());
        this->push_back_vpc(vpc);
        this->get_connected_pc().last().push_back_vpc(&this->get_vpc(vpc_name));
    }

    // Загрузка в сеть интерфейсов из конфига

    QJsonArray jInts = jNet["network_interfaces"].toArray();
    for(auto jValue: jInts) {
        QJsonObject jInt = jValue.toObject();
        QString int_name = jInt["name"].toString();
        QString int_ip_addr = jInt["ip_addr"].toString();
        Interface interface(int_name, int_ip_addr, this);
        this->push_back_interface(interface);
    }

    // Назначение сетевым стекам своих интерфейсов и наоборот
    int i(0);
    for(auto& interface: this->get_exists_interface()) {
        if(interface.is_phisical()) {
            // Пропускаем
            continue;
        }
        // Достаем параметры интерфейса из конфига и устанавливаем их в модель
        QJsonObject jInt = jInts[i].toObject();
        QString int_name = jInt["name"].toString();
        QString neighbor_name = jInt["neighbor"].toString();
        this->get_interface(int_name).set_neighbor(&this->get_interface(neighbor_name));
        QString pc_name = jInt["pc_name"].toString();
        QString vpc_name = jInt["vpc_name"].toString();
        QString bridge_name = jInt["br_name"].toString();
        if(!vpc_name.isEmpty()) {
            interface.set_vpc_connect(&this->get_vpc(vpc_name));
            this->get_vpc(vpc_name).push_back_interface(this->get_interface(int_name));
            i++;
            continue;
        }
        else if(!bridge_name.isEmpty()) {
            if(!interface.set_connected_bridge()) {
                i++;
                continue;
            }
            this->get_network_bridge()->push_back_int(&interface);
            i++;
            continue;
        }
        else if(!pc_name.isEmpty()) {
            interface.set_pc_connect(&this->get_connected_pc().last());
            this->get_connected_pc().last().push_back_int(&interface);
            i++;
            continue;
        }
        i++;
    }
    QJsonArray jRoutes = jNet["network_routes"].toArray();
    for(auto jValue: jRoutes) {
        QJsonObject jRoute = jValue.toObject();
        QString gateway_type = jRoute["gateway_type"].toString();
        QString gateway_name = jRoute["gateway_name"].toString();
        QString net_destination = jRoute["net_destination"].toString();
        bool isPC_route = jRoute["pc_route"].toBool();
        bool isVPC_route = jRoute["vpc_route"].toBool();
        if(isVPC_route) {
            QJsonObject VPC_params;
            QString vpc_name;
            VPC_params = jRoute["vpc_param"].toObject();
            vpc_name = VPC_params["name"].toString();

            if(gateway_type == "veth") {
                Route route(net_destination, &this->get_interface(gateway_name), this, &this->get_vpc(vpc_name));
                this->push_back_route(route);
            }
            else if(gateway_type == "bridge") {
                Route route(net_destination,this->get_network_bridge(), this, &this->get_vpc(vpc_name));
                this->push_back_route(route);
            }
        }
        else if(isPC_route) {
            if(gateway_type == "veth") {
                Route route(net_destination, &this->get_interface(gateway_name), this, nullptr, &this->get_connected_pc().last());
                this->push_back_route(route);
            }
            else if(gateway_type == "bridge") {
                Route route(net_destination,this->get_network_bridge(), this, nullptr, &this->get_connected_pc().last());
                this->push_back_route(route);
            }
        }
    }

    QJsonArray jTCs = jNet["network_tc"].toArray();
    for(auto jValue: jTCs) {
        QJsonObject jTC = jValue.toObject();
        QString int_name = jTC["interface_name"].toString();
        QJsonArray jJitter = jTC["jitter"].toArray();
        QJsonArray jLoss = jTC["loss"].toArray();
        QJsonArray jPack = jTC["package_order"].toArray();
        pair jitter = pair(jJitter.first().toInt(), jJitter.last().toInt());
        pair loss =  pair(jLoss.first().toInt(), jLoss.last().toInt());
        pair package_order = pair(jPack.first().toInt(), jPack.last().toInt());
        int noise = jTC["noise"].toInt();
        if(int_existence_check(int_name)) {
            tc traffic(&this->get_interface(int_name), jitter, loss, package_order, noise);
            this->exists_tc.push_back(traffic);
            this->get_interface(int_name).set_tc(&get_tc().last());
        }
    }
    return true;
}

QJsonObject Network::SaveConfigToFile(QString desc)
{
    // Cохранение конфига в файл
    QJsonObject jObj;
    jObj.insert("description", desc);
    QString date = QDate::currentDate().toString("dd.MM.yyyy");
    QString time = QTime::currentTime().toString("hh:mm:ss");
    jObj.insert("date", date);
    jObj.insert("time", time);
    jObj.insert("network", getJsonNetwork());
    return jObj;
}

tc &Network::get_tc(Interface *interface)
{
    for(auto& tc: this->exists_tc) {
        if (tc.get_int()->get_int_name() == interface->get_int_name())
            return tc;
    }
}


Bridge *Network::get_network_bridge()
{
    return this->bridge;
}

VPC &Network::get_vpc(QString name)
{
    for(auto& vpc: connected_vpc)
    {
        if(vpc.get_vpc_name() == name)
            return vpc;
    }
    // ??? return - ?
}

uint Network::get_net_mask()
{
    return this->network_mask;
}

bool Network::set_int_neighbors(Interface *first_int, Interface *second_int)
{
    bool first_int_exist = false;
    bool second_int_exist = false;
    for(auto& interface: this->net_int)
    {
        if(interface.get_int_name() == first_int->get_int_name())
        {
            first_int_exist = true;
            continue;
        }
        if(interface.get_int_name() == second_int->get_int_name())
        {
            second_int_exist = true;
            continue;
        }
    }
    if(first_int_exist & second_int_exist)
    {
        first_int->set_neighbor(second_int);
        second_int->set_neighbor(first_int);
        return true;
    }
    return false;
}

void Network::set_network_mask(uint mask)
{
    this->network_mask = mask;
}
