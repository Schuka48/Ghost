#ifndef NETWORK_H
#define NETWORK_H

#include "Interface.h"
#include "VPC.h"
#include "PC.h"
#include "bridge.h"
#include "route.h"
#include "tc.h"
#include <QString>
#include <QList>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDate>

typedef QPair<int, int> pair;

class Bridge;
class Interface;
class VPC;
class PC;
class Route;
class tc;


class Network{
    static uint net_id;
    uint id;
    QString network_ip;
    QString phis_ip_addr;
    uint network_mask;
    Bridge* bridge;
    QList<Interface> net_int;
    QList<VPC> connected_vpc;
    QList<PC> connected_pc;
    QList<Route> exists_routes;
    QList<tc> exists_tc;

private:
    QJsonObject getJsonNetwork();
    QJsonArray getJsonPhis();
    QJsonObject getJsonBridge();
    QJsonArray getJsonVPC();
    QJsonArray getJsonInterfaces();
    QJsonArray getJsonRoute();
    QJsonArray getJsonTC();
    uint get_id();

public:

    Network(QString phis_ip_addr, QString = "10.0.0.0");
    Network();

    QList<VPC>& get_connected_vpc();
    QList<PC>& get_connected_pc();
    QList<Interface>& get_exists_interface();
    QList<tc>& get_tc();
    QString get_network_ip();
    Interface& get_interface(QString name);
    Bridge* get_network_bridge();
    VPC& get_vpc(QString name);
    uint get_net_mask();

    void push_back_pc(const PC& phis_pc);
    void push_back_interface(const Interface &interface);
    void push_back_vpc(const VPC& virtual_pc);
    void push_back_route(const Route& route);
    void push_back_tc(const tc& traffic_control);

    void set_bridge(Bridge* br);
    bool set_int_neighbors(Interface* first_int, Interface* second_int);
    void set_network_mask(uint mask);


    bool vpc_existence_check(QString vpc_name);
    bool int_existence_check(QString int_name);
    QList<Route>& get_routes();

    bool LoadConfigFromFile(QString& file_name);

    QJsonObject SaveConfigToFile(QString desc);

    tc& get_tc(Interface* interface);
};



#endif // NETWORK_H
