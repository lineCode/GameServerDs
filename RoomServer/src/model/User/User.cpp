#include "User.h"

namespace MiniProject
{

    User::User()
    {
    }

    User::User(const UserId &_userid)
        : userid_(_userid)
    {
    }

    User::User(const NetId &_conv, const std::string &_ip, int _port)
        : netId_(_conv), m_b_connected(true), m_str_ip(_ip), m_int_port(_port)
    {
    }

    User::~User()
    {
    }

    void User::set_nick_name(const std::string &nickname)
    {
        username_ = nickname;
    }

    void User::set_user_id(UserId _uid)
    {
        userid_ = _uid;
    }
    void User::set_conv_id(NetId _conv)
    {
        netId_ = _conv;
    }

    void User::set_master_port(uint32_t _port)
    {
        m_server_port = _port;
    }

    void User::set_player_id(PlayerId _id)
    {
        m_int_player_id = _id;
    }

    /*********************************************************
    Function:	set_room_id
    Description:设置roomid
    Input:_roomid:待设置的roomId
    Output:
    Return:
    **********************************************************/
    void User::set_room_id(RoomId _roomId)
    {
        m_ll_roomId = _roomId;
    }

    /*********************************************************
    Function:	set_master
    Description:设置用户的身份
    Input:is_master:是否为房主
    Output:
    Return:
    **********************************************************/
    void User::set_master(bool is_master)
    {
        m_b_master = is_master;
    }

    /*********************************************************
    Function:	set_user_ip
    Description:设置user的ip的地址
    Input:_ip:user的ip地址
    Output:
    Return:
    **********************************************************/
    void User::set_user_ip(const std::string &_ip)
    {
        m_str_ip = _ip;
    }

    /*********************************************************
    Function:	set_user_ready_state
    Description:设置用户的准备状态
    Input:_ready：设置用户的准备状态
    Output:
    Return:
    **********************************************************/
    void User::set_user_ready_state(bool _ready)
    {
        m_b_ready = _ready;
    }

    /*********************************************************
    Function:	set_user_role
    Description:设置用户选择的角色信息
    Input:_role：用户选择的角色id
    Output:
    Return:
    **********************************************************/
    void User::set_user_role(RoleId _role)
    {
        m_int_role = _role;
    }

    /*********************************************************
    Function:	set_connect_battle_state
    Description:设置用户连接服务器的状态
    Input:_ready：设置用户的准备状态
    Output:
    Return:
    **********************************************************/
    void User::set_connect_battle_state(bool _st)
    {
        m_b_connectBattle = _st;
    }

    /*********************************************************
    Function:	set_connect_state
    Description:设置用户连接状态
    Input:_ready：设置用户的准备状态
    Output:
    Return:
    **********************************************************/
    void User::set_connect_state(bool _st)
    {
        m_b_connected = _st;
    }

    void User::set_addr(const std::string &_addr, uint32_t _port)
    {
        m_str_ip = _addr;
        m_int_port = _port;
    }

    /*********************************************************
    Function:	init()
    Description:清理房间相关的状态
    Input:
    Output:
    Return:
    **********************************************************/
    void User::init()
    {
        m_ll_roomId = -1;
        m_b_master = false;
        m_b_ready = false;
        m_int_role = -1;
        m_b_connectBattle = false;
    }

    /*********************************************************
    Function:	initConnectedState()
    Description:清理用户连接相关的状态
    Input:
    Output:
    Return:
    **********************************************************/
    void User::initConnectedState()
    {
        netId_ = 0;
        m_b_connected = false; // 用户是否连接到大厅（与大厅连接）
        m_str_ip = "";         // 账号的ip地址
        m_int_port = 0;        // 账号连接的端口地址
        m_server_port = 0;     // 该客户端作为时使用的端口
    }

    void User::merge(UserPtr &_ptr)
    {
        netId_ = _ptr->netId_;
        m_b_connected = _ptr->m_b_connected;
        m_str_ip = _ptr->m_str_ip;
        m_int_port = _ptr->m_int_port;
        m_server_port = _ptr->m_server_port;
    }

}