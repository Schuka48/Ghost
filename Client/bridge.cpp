#include "bridge.h"

uint Bridge::bridge_id = 0;

Bridge::Bridge()
{
    this->id = ++bridge_id;
}

Bridge::Bridge(Network *net, QString name)
{
    this->id = ++bridge_id;
    this->net = net;
    this->name = name;

}

void Bridge::set_ip_address(const QString& ip_address)
{
    this->ip_addr = ip_address;
}

void Bridge::push_back_int(Interface *interface)
{
    this->connected_int.push_back(interface);
//    this->get_int_by_name(interface->get_int_name())->set_ip_addr(this->ip_addr);
}

Interface *Bridge::get_int_by_name(QString name)
{
    for(auto& interface: net->get_exists_interface())
    {
        if(interface.get_int_name() == name)
            return &interface;
    }
    return nullptr;
}

QList<Interface *> &Bridge::get_int()
{
    return this->connected_int;
}

QString Bridge::get_bridge_name()
{
    return this->name;
}

QJsonObject Bridge::toObject()
{
    QJsonObject jObj;
    jObj.insert("id", int(this->id));
    jObj.insert("name", this->name);
    jObj.insert("br_ip", this->ip_addr);
    return jObj;
}

QString Bridge::get_ip_address()
{
    return this->ip_addr;
}

uint Bridge::get_id()
{
    return this->id;
}
