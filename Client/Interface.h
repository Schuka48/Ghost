#ifndef INTERFACE_H
#define INTERFACE_H

#include "VPC.h"
#include "PC.h"
#include "tc.h"

#include <QString>

class VPC;
class PC;
class Bridge;
class tc;

class Interface{
    static uint int_id;
    uint id;
    QString int_name;
    QString ip_addr;
    VPC* vpc_connected;
    PC* pc_connected;
    Interface * neighbor;
    Network* net;
    Bridge* connect_bridge;
    tc* traffic_rules;
    bool phisical;
public:

    Interface(QString name, QString ip_address, Network* net, VPC* connected_vpc = nullptr, PC* connected_pc = nullptr, Interface* neighbor = nullptr);
    Interface();
    QJsonObject toObject();
    void set_ip_addr(QString ip_addr);
    QString get_int_name();
    bool set_pc_connect(PC* connected_pc);
    bool set_vpc_connect(VPC* connected_vpc);
    QString get_ip_address();
    bool set_neighbor(Interface* neighbor_int);
    bool set_connected_bridge();
    Interface* get_neighbor();
    void set_type(bool type);
    bool is_phisical();
    uint get_id();
    VPC* get_connected_vpc();
    PC* get_connected_pc();
    Bridge* get_connected_bridge();

    tc* get_tc();
    void set_tc(tc* traffic_rules);


};


#endif // INTERFACE_H
