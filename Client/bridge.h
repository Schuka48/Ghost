#ifndef BRIDGE_H
#define BRIDGE_H

#include "Interface.h"
#include "PC.h"
#include "Network.h"
#include "VPC.h"

#include <QObject>
#include <QWidget>

class VPC;
class PC;
class Interface;
class Network;

class Bridge
{
    uint id;
    static uint bridge_id;
    Network* net;
    QString name;
    QString ip_addr;
    QList<Interface*> connected_int;
public:

    Bridge();
    Bridge(Network* net, QString name = "Bridge");
    void set_ip_address(const QString& ip_address);
    void push_back_int(Interface* interface);
    Interface* get_int_by_name(QString name);
    QList<Interface*>& get_int();
    QString get_bridge_name();
    QJsonObject toObject();
    QString get_ip_address();
    uint get_id();
};


#endif // BRIDGE_H
