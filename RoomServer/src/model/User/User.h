#ifndef MINIPROJECT_GAME_USER_HPP
#define MINIPROJECT_GAME_USER_HPP

#include "../../../../common/network/EventCallBack.h"
#include "../room_typedef.h"
#include <memory>
#include <string>

namespace MiniProject
{
    class User
    {
    public:
        typedef std::shared_ptr<User> UserPtr;

    private:
        // 账号信息相关
        UserId userid_ = 0;    // 账号id
        std::string username_; // 用户名称

        // 连接状态相关
        NetId netId_ = 0;
        bool m_b_connected = false; // 用户是否连接到大厅（与大厅连接）
        std::string m_str_ip = "";  // 账号的ip地址
        uint32_t m_int_port = 0;    // 账号连接的端口地址

        // 战斗相关
        bool m_b_connectBattle = false; // 用户是否登录到战斗服务器
        uint32_t m_server_port = 0;     // 该客户端作为时使用的端口

        // 房间信息维护
        RoomId m_ll_roomId = -1;       // 用户所在的房间id
        bool m_b_master = false;       // 用户是否为房主
        bool m_b_ready = false;        // 用户是否已准备好进入游戏
        RoleId m_int_role = -1;        // 用户所选择的角色，目前暂不使用
        PlayerId m_int_player_id = -1; // 用户的玩家id

    public:
        User();
        User(const UserId &_uid);
        User(const NetId &_conv, const std::string &_addr, int port);
        ~User();

    public:
        void init();               // 退出房间后初始化房间状态信息
        void initConnectedState(); // 断开连接后初始化连接状态

        void merge(UserPtr &_ptr); // 合并两个userptr，将第二个userptr的连接状态转移

        void set_user_id(UserId _uid);

        inline UserId userId()
        {
            return userid_;
        }

        void set_conv_id(NetId _conv);

        inline NetId ConvId()
        {
            return netId_;
        }

        void set_room_id(RoomId _roomId);

        inline RoomId roomId()
        {
            return m_ll_roomId;
        }

        inline bool isInRoom()
        {
            return m_ll_roomId != -1;
        }

        void set_user_ip(const std::string &_ip);

        void set_user_ready_state(bool _ready);
        bool isReady()
        {
            return m_b_ready;
        }

        void set_user_role(RoleId _role);

        RoleId roleId()
        {
            return m_int_role;
        }

        bool hasChooseRole()
        {
            return m_int_role != -1;
        }

        void set_connect_battle_state(bool _st);

        bool hasConnectBattleServer()
        {
            return m_b_connectBattle;
        }

        void set_connect_state(bool _st);
        bool isConneted()
        {
            return m_b_connected;
        }

        void set_master_port(uint32_t __port);
        uint32_t MasterPort()
        {
            return m_server_port;
        }

        void set_player_id(PlayerId _id);
        PlayerId playerId()
        {
            return m_int_player_id;
        }

        void set_nick_name(const std::string &name);
        std::string name()
        {
            return username_;
        }

        void set_master(bool is_master);
        bool isMaster()
        {
            return m_b_master;
        }

        void set_addr(const std::string &_addr, uint32_t _port);
        std::string addr()
        {
            return m_str_ip;
        }
    };

} // namespace MiniProject

#endif