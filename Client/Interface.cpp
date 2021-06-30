#include "Interface.h"

uint Interface::int_id = 0;

Interface::Interface(QString name, QString ip_address, Network* net, VPC* connected_vpc, PC* connected_pc, Interface* neighbor)
{
    this->id = ++int_id;
    this->int_name = name;
    this->ip_addr = ip_address;
    this->net = net;
    this->vpc_connected = connected_vpc;
    this->pc_connected = connected_pc;
    this->neighbor = neighbor;
    this->connect_bridge = nullptr;
    this->traffic_rules = nullptr;
    this->phisical = false;
}
Interface::Interface()
{
    this->id = ++int_id;
    this->int_name = "";
    this->neighbor = nullptr;
    this->traffic_rules = nullptr;
}

QJsonObject Interface::toObject()
{
    QJsonObject jObj;
    jObj.insert("int_name", this->int_name);
    if(this->vpc_connected) {
        jObj.insert("conected_stack", "vpc");
        jObj.insert("vpc_name", this->vpc_connected->get_vpc_name());
    }
    if(this->connect_bridge) {
        jObj.insert("connected_stack", "bridge");
        jObj.insert("bridge_name", this->connect_bridge->get_bridge_name());
    }
    jObj.insert("address", this->ip_addr);
    return jObj;
}

void Interface::set_ip_addr(QString ip_addr)
{
    this->ip_addr = ip_addr;
    if(this->get_int_name() == "ens33")
        if(this->net->get_network_bridge())
            this->net->get_network_bridge()->set_ip_address(ip_addr);

}

QString Interface::get_int_name()
{
    return this->int_name;
}

bool Interface::set_pc_connect(PC *connected_pc)
{
    for(auto& pc: net->get_connected_pc())
    {
        if(pc.get_pc_id() == connected_pc->get_pc_id())
        {
            this->pc_connected = connected_pc;
            return true;
        }
    }
    return false;
}

bool Interface::set_vpc_connect(VPC *connected_vpc)
{
    for(auto& vpc: net->get_connected_vpc())
    {
        if(vpc.get_vpc_name() == connected_vpc->get_vpc_name())
        {
            this->vpc_connected = connected_vpc;
            return true;
        }
    }
    return false;
}

QString Interface::get_ip_address()
{
    if(!this->ip_addr.isEmpty())
        return this->ip_addr;
    else
        return "";
}

bool Interface::set_neighbor(Interface *neighbor_int)
{
    for(auto& interface: this->net->get_exists_interface())
    {
        if(&interface == neighbor_int)
        {
            this->neighbor = neighbor_int;
            return true;
        }
    }
    return false;
}

bool Interface::set_connected_bridge()
{
    if(net->get_network_bridge())
    {
        this->connect_bridge = net->get_network_bridge();
        return true;
    }
    else
        return false;
}

Interface *Interface::get_neighbor()
{
    return this->neighbor;
}

void Interface::set_type(bool type)
{
    this->phisical = type;
}

bool Interface::is_phisical()
{
    if(this->phisical)
    {
        return true;
    }
    else {
        return false;
    }
}

uint Interface::get_id()
{
    return this->id;
}

VPC *Interface::get_connected_vpc()
{
    return this->vpc_connected;
}

PC *Interface::get_connected_pc()
{
    return this->pc_connected;
}

Bridge *Interface::get_connected_bridge()
{
    return this->connect_bridge;
}

tc *Interface::get_tc()
{
    return this->traffic_rules;
}

void Interface::set_tc(tc *traffic_rules)
{
    this->traffic_rules = traffic_rules;
}
