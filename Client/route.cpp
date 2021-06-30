#include "route.h"

uint Route::route_id = 0;

Route::Route()
{

}

Route::Route(QString ip_network_destination, Interface* gateway, Network *net, VPC *vpc_route, PC *pc_route)
{
    this->id = ++route_id;
    this->net = net;
    this->gateway = gateway;
    this->ip_network_destination = ip_network_destination;
    this->gateway_ip = gateway->get_ip_address();
    this->vpc_route = vpc_route;
    this->pc_route = pc_route;
    this->br_gateway = nullptr;
}

Route::Route(QString ip_network_destination, Bridge* gateway, Network *net, VPC *vpc_route, PC *pc_route)
{
    this->id = ++route_id;
    this->net = net;
    this->gateway = nullptr;
    this->br_gateway = gateway;
    this->ip_network_destination = ip_network_destination;
    this->gateway_ip = gateway->get_ip_address();
    this->vpc_route = vpc_route;
    this->pc_route = pc_route;

}

QJsonObject Route::toObject()
{
    QJsonObject jObj;
    if(this->ip_network_destination.contains('/'))
        jObj.insert("network", this->ip_network_destination);
    else
        jObj.insert("network", this->ip_network_destination + "/" + QString::number(net->get_net_mask()));
    if(gateway)
    jObj.insert("gateway_int", this->gateway->get_int_name());
    else
        jObj.insert("gateway_int", this->br_gateway->get_bridge_name());
    auto strs = this->gateway_ip.split('/');
    jObj.insert("gateway_ip", strs[0]);
    if(this->pc_route){
        QJsonObject jCont;
        jCont.insert("type", "phis");
        jCont.insert("name", "Phisical_PC");
        jObj.insert("container", jCont);
    }
    if (this->vpc_route) {
        QJsonObject jCont;
        jCont.insert("type", "vpc");
        jCont.insert("name", this->vpc_route->get_vpc_name());
        jObj.insert("container", jCont);
    }
    return jObj;
}

PC *Route::get_pc_route()
{
    return this->pc_route;
}

VPC *Route::get_vpc_route()
{
    return this->vpc_route;
}

uint Route::get_id()
{
    return id;
}

QString Route::get_net_destination()
{
    return this->ip_network_destination;
}

Interface *Route::get_int_gateway()
{
    return this->gateway;
}

Bridge *Route::get_br_gateway()
{
    return this->br_gateway;
}
