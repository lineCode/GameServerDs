#include "Connection.h"
#include <iostream>
namespace MiniProject
{
    Connection::Connection(int net_id)
    {
        cli_.net_id = net_id;
    }

    Connection::~Connection()
    {
    }

    void Connection::set_connected(bool is_conn)
    {
        is_connected = is_conn;
    }

    void Connection::set_endpoint(int net_id, ConnectType _type)
    {
        server_.net_id = net_id;
        server_._type = _type;
    }

    void Connection::addEnpoints(int net_id)
    {
        for (EndPoint &_ep : endpointsList_)
        {
            if (_ep.net_id == net_id)
            {
                return;
            }
        }
        EndPoint _ep;
        _ep.net_id = net_id;
        endpointsList_.push_back(_ep);
    }

    void Connection::set_type(ConnectType _type)
    {
        cli_._type = _type;
    }

    void Connection::removeEnpoints(int net_id)
    {
        for (auto it = endpointsList_.begin(); it != endpointsList_.end(); ++it)
        {
            if (it->net_id == net_id)
            {
                endpointsList_.erase(it);
                return;
            }
        }
    }

    std::vector<int> Connection::GetConnectedNetIds()
    {
        std::vector<int> idVecs;

        if (endpointsList_.empty())
        {
            return idVecs;
        }

        idVecs.reserve(idVecs.size());

        for (EndPoint &ep : endpointsList_)
        {
            idVecs.push_back(ep.net_id);
        }

        return idVecs;
    }

    void Connection::set_username(const UserName &_uname)
    {
        username_ = _uname;
    }

    void Connection::set_userid(const UserId &_uid)
    {
        userId_ = _uid;
    }

}