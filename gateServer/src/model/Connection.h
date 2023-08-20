#ifndef MINIPROJECT_GATESERVER_CONNECTION_HPP
#define MINIPROJECT_GATESERVER_CONNECTION_HPP

#include "../../../common/network/type.h"
#include "structDef.h"
#include <memory>
#include <list>

namespace MiniProject
{
    class Connection
    {
    public:
        typedef std::shared_ptr<Connection> ConnectionPtr;

    public:
        Connection(int net_id);

        int GetClientNetId()
        {
            return cli_.net_id;
        }

        int GetServerNetId()
        {
            return server_.net_id;
        }

        UserId GetUserId()
        {
            return userId_;
        }

        UserName GetUserName()
        {
            return username_;
        }

        void addEnpoints(int net_id);

        void removeEnpoints(int net_id);

        ConnectType GetServerNetType()
        {
            return server_._type;
        }

        bool isConnected()
        {
            return is_connected;
        }

        void set_connected(bool _conn);

        void set_type(ConnectType _type);

        void set_endpoint(int net_id, ConnectType type);

        std::vector<int> GetConnectedNetIds();

        ~Connection();

        void set_username(const UserName& _uname);

        void set_userid(const UserId& _uid);


    private:
        struct EndPoint
        {
            int net_id = 0;
            ConnectType _type = ConnectType::CT_DEFAULT;
        };

        EndPoint cli_;
        EndPoint server_;
        std::list<EndPoint> endpointsList_;
        bool is_connected = false;

        UserName username_ = "";
        UserId userId_ = 0;
    };

} // namespace MiniProject

#endif