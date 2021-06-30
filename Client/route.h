#ifndef ROUTE_H
#define ROUTE_H

#include "bridge.h"
#include "Interface.h"
#include "Network.h"
#include "PC.h"
#include "VPC.h"

#include <QString>
#include <QJsonObject>

class Bridge;
class VPC;
class PC;
class Network;
class Interface;


class Route
{
public:
    Route();
    Route(QString ip_network_destination, Interface* gateway, Network* net, VPC* vpc_route = nullptr, PC* pc_route = nullptr);
    Route(QString ip_network_destination, Bridge* gateway, Network* net, VPC* vpc_route = nullptr, PC* pc_route = nullptr);
    QJsonObject toObject();
    PC* get_pc_route();
    VPC* get_vpc_route();
    uint get_id();
    QString get_net_destination();
    Interface* get_int_gateway();
    Bridge* get_br_gateway();
private:
    static uint route_id;
    uint id;
    Interface* gateway;
    Network* net;
    Bridge* br_gateway;
    QString ip_network_destination;
    QString gateway_ip;
    VPC* vpc_route;
    PC* pc_route;    

};

#endif // ROUTE_H
