#ifndef MINIPROJECT_GAME_USER_HPP
#define MINIPROJECT_GAME_USER_HPP

#include "../../../../common/network/EventCallBack.h"
#include "../typedef.h"
#include <memory>
#include <string>

namespace MiniProject
{
    enum UserState
    {
        IDLE = 0,
        IN_PROCESS = 1
    };

    enum UserOnlineState
    {
        OFFLINE,
        Logging,
        LogOut
    };

    class User
    {
    public:
        typedef std::shared_ptr<User> UserPtr;

    private:
        // 账号信息相关
        UserId userid_ = 0;    // 账号id
        std::string username_; // 用户名称

        // 连接状态相关
        NetId cliNetId_ = 0;        // 客户端侧网络的id号
        NetId gateNetId_ = 0;       // 客户端侧连接的gateserver对应的网络id
        bool m_b_connected = false; // 用户是否连接到大厅服务器（用于断线重连处理）

        // 房间信息维护
        NetId serVerNetId_ = 0;                // 房间服务器侧的服务器id号
        bool is_master_ = 0;                   // 用户是否为房间的房主
        RoomId roomId_ = -1;                   // 用户所在的房间号
        bool m_b_connect_room_service = false; // 用户是否连接到房间服务器

        // 战斗相关
        bool m_b_connect_battle_service = false; // 用户是否连接到战斗服务器
        Time gameStartTime_;                     // 用户连接到服务器的时间
        std::string serverAddr_ = "";            // 用户所处的战斗服务器的地址
        uint32_t serverPort_ = 0;                // 用户所处的战斗服务器的端口

        UserState currentSt_ = UserState::IDLE;

    public:
        UserId GetUserId()
        {
            return userid_;
        }

        std::string GetUserName()
        {
            return username_;
        }

        void setGateServerNetId(const NetId &_netid);
        NetId GetGateNetId();

        void setClinetNetId(const NetId &_netid);
        NetId GetClientNetId();

        void set_connected(bool _st);
        bool isConnected();

        void setServerNetId(const NetId &_netid);
        NetId GetServerNetId();

        void setMaster(bool _master);
        bool isMaster();

        bool isInRoom();
        void setRoomId(const RoomId &_rid);
        RoomId GetRoomId();

        bool isConnectRoom();
        void setConnectRoom(bool _st);

        bool isBattle();
        void setConnectBattleServce(bool _st);

        Time GetGameStartTime();
        void setGameStartTime(const Time _time);

        void setBattleServer(const std::string _addr, uint32_t _port);
        std::string GetBattleServerAddr();
        uint32_t GetBattleServerPort();

        bool is_busy();
        void setCurrentSt(UserState _st);
        UserState GetCurrentSt();

        

    public:
        User(const UserId &_uid, const std::string &_uname);
        User(const UserId &_uid, const std::string &_uname, const NetId &_cliId);
    };

} // namespace MiniProject

#endif