#ifndef VPC_H
#define VPC_H

#include "Interface.h"
#include "PC.h"
#include "Network.h"

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class Network;
class Interface;
class PC;
class Bridge;

class VPC{
    static uint vpc_id;
    uint id;
    QString vpc_name;
    Network* net;
    PC* pc_connected;
    QList<Interface> included_interfaces;
public:
    VPC(QString name, Network* net);
    VPC();
    void set_pc_connected(PC* const &pc_connected);
    PC* get_connected_pc();
    QJsonObject toObject();
    QString get_vpc_name();
    void push_back_interface(const Interface& intreface);
    QList<Interface>& get_int_connected();
    uint get_id();
};


#endif // VPC_H
