#include "User.h"

namespace MiniProject
{
    User::User(const UserId &_uid, const std::string &_uname)
        : userid_(_uid), username_(_uname)
    {
    }

    User::User(const UserId &_uid, const std::string &_uname, const NetId &_netId)
        : userid_(_uid), username_(_uname), cliNetId_(_netId)
    {
        m_b_connected = true;
    }

    void User::setClinetNetId(const NetId &_netid)
    {
        cliNetId_ = _netid;
    }

    NetId User::GetClientNetId()
    {
        return cliNetId_;
    }

    void User::setGateServerNetId(const NetId &_netid)
    {
        gateNetId_ = _netid;
    }
    NetId User::GetGateNetId()
    {
        return gateNetId_;
    }

    void User::set_connected(bool _st)
    {
        m_b_connected = _st;
        if (!m_b_connected)
        {
            cliNetId_ = 0;
            gateNetId_ = 0;
        }
    }

    bool User::isConnected()
    {
        return m_b_connected;
    }

    void User::setServerNetId(const NetId &_netid)
    {
        serVerNetId_ = _netid;
    }

    NetId User::GetServerNetId()
    {
        return serVerNetId_;
    }

    void User::setMaster(bool _master)
    {
        is_master_ = _master;
    }

    bool User::isMaster()
    {
        return is_master_;
    }

    bool User::isInRoom()
    {
        return roomId_ != -1;
    }

    void User::setRoomId(const RoomId &_rid)
    {
        roomId_ = _rid;
    }

    RoomId User::GetRoomId()
    {
        return roomId_;
    }

    bool User::isConnectRoom()
    {
        return m_b_connect_room_service;
    }
    void User::setConnectRoom(bool _st)
    {
        m_b_connect_room_service = _st;
    }

    bool User::isBattle()
    {
        return m_b_connect_battle_service;
    }
    void User::setConnectBattleServce(bool _st)
    {
        m_b_connect_battle_service = _st;
    }

    Time User::GetGameStartTime()
    {
        return gameStartTime_;
    }
    void User::setGameStartTime(const Time _time)
    {
        gameStartTime_ = _time;
    }

    void User::setBattleServer(const std::string _addr, uint32_t _port)
    {
        serverAddr_ = _addr;
        serverPort_ = _port;
    }
    std::string User::GetBattleServerAddr()
    {
        return serverAddr_;
    }
    uint32_t User::GetBattleServerPort()
    {
        return serverPort_;
    }

    bool User::is_busy()
    {
        return currentSt_ != UserState::IDLE;
    }

    void User::setCurrentSt(UserState _st)
    {
        currentSt_ = _st;
    }

    UserState User::GetCurrentSt()
    {
        return currentSt_;
    }

}
