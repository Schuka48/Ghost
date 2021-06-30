#ifndef PC_H
#define PC_H

#include "Interface.h"
#include "VPC.h"
#include "Network.h"

#include <QString>
#include <QList>

class Network;
class Interface;
class VPC;
class Bridge;

class PC {
    static uint pc_id;
    uint id;
    Network* net;
    QString ip_addr;
    uint mask;
    QList<VPC*> vpc_connected;
    QList<Interface*> int_connected;
    Bridge* connected_bridge;
public:
    QList<Interface*>& get_connected_interfaces();
    QList<VPC*>& get_connected_vpc();
    PC(Network*, QString& ip_addr);
    PC();
    uint get_pc_id();
    Interface* get_int(QString name);
    bool set_pc_bridge(Bridge* bridge);
    Bridge* get_connected_bridge();
    void set_mask(uint mask);
    void push_back_vpc(VPC* vpc);
    void push_back_int(Interface* interface);
    QString get_ip_addr();
    uint get_mask();
//    QJsonObject toObject();   
};

#endif // PC_H
