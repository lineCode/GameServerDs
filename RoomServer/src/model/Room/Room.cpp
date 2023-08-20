#include "Room.h"
#include <cassert>

namespace MiniProject
{

    Room::Room(RoomId _roomId, int max_size)
        : MAX_USER_SIZE(max_size), m_ll_roomid_(_roomId), num_use(max_size, 0)
    {
        m_set_levels = {0, 1, 2};
        lastOpTime_ = time(nullptr);
    }

    Room::Room(RoomId _roomId, const std::string &name, int max_size)
        : MAX_USER_SIZE(max_size), m_str_name_(name), m_ll_roomid_(_roomId), num_use(max_size, 0)
    {
        m_set_levels = {0, 1, 2};
        lastOpTime_ = time(nullptr);
    }

    Room::~Room()
    {
    }

    /*********************************************************
    Function:	userEnter
    Description:用户加入房间
    Input:_ptr:用户的信息
    Output:（这一步是否需要加锁，待考虑）
    Return：int:是否成功加入 0:成功
                            1：人数已满
                            2：已存在该用户
                            3：用户已加入其它房间
                            4：房间不可加入
    **********************************************************/
    int Room::userEnter(User::UserPtr &_ptr)
    {
        if (m_rs_state_ >= RoomState::RoomStateReady)
        {
            return 4;
        }

        if (m_cur_room_size_ >= MAX_USER_SIZE)
        {
            return 1;
        }

        if (_ptr->isInRoom())
        {
            return _ptr->roomId() == m_ll_roomid_ ? 3 : 2;
        }

        m_list_users_.push_back(_ptr);
        m_map_users_[_ptr->userId()] = std::prev(m_list_users_.end());

        if (m_cur_room_size_ == 0)
        {
            m_userid_master_ = _ptr->userId();
            m_master_addr = _ptr->addr();
            m_master_port = _ptr->MasterPort();
            _ptr->set_master(true);
        }
        else
        {
            _ptr->set_master(false);
        }

        int t = -1;
        for (int i = 0; i < MAX_USER_SIZE; ++i)
        {
            if (num_use[i] == 0)
            {
                num_use[i] = 1;
                t = i + 1;
                break;
            }
        }

        _ptr->set_player_id(t);
        _ptr->set_room_id(m_ll_roomid_);

        ++m_cur_room_size_;
        return 0;
    }

    /*********************************************************
    Function:	userLeave
    Description:用户离开房间
    Input:_uid:用户的uid
    Output:
    Return：int:用户是否正常离开，0：正常，1:房间忙碌，拒绝退出, 2：用户不在房间
    **********************************************************/
    int Room::userLeave(const UserId &_uid)
    {
        if (isRunning())
        {
            return 1;
        }

        auto it = m_map_users_.find(_uid);

        if (it == m_map_users_.end())
        {
            return 2;
        }

        User::UserPtr _userPtr = (*it->second);

        bool need_find_master = (_userPtr->userId() == m_userid_master_);

        if (_userPtr->playerId() <= num_use.size() && _userPtr->playerId() > 0)
        {
            num_use[_userPtr->playerId() - 1] = 0;
        }

        if (_userPtr->isReady())
        {
            --m_cur_ready_users_;
        }

        clear_user_info(*it->second);

        m_list_users_.erase(it->second);
        m_map_users_.erase(it);
        --m_cur_room_size_;

        if (need_find_master)
        {
            m_userid_master_ = 0;
            find_new_master();
        }

        return 0;
    }

    /*********************************************************
    Function:	userReady
    Description:用户准备
    Input:_uid:用户的uid
    Output:
    Return：int:用户准备 ， 0：正常(普通用户准备)
                          1:正常（房主启动成功）
                          2:系统错误，
                          3:用户不存在
                          4:用户已经准备
                          5:用户尚未选择角色
                          6:有玩家尚未准备(房主命令时回复)
    **********************************************************/
    int Room::userReady(const UserId &_uid)
    {
        if (m_rs_state_ >= RoomState::RoomStateRunning)
        {
            return 2;
        }

        auto it = m_map_users_.find(_uid);

        if (it == m_map_users_.end())
        {
            return 3;
        }

        if ((*(it->second))->isReady())
        {
            return 4;
        }

        // if (!(*(it->second))->hasChooseRole())
        // {
        //     return 5;
        // }

        if ((*(it->second))->isMaster())
        {
            if (m_cur_room_size_ != MAX_USER_SIZE || m_cur_ready_users_ < (m_cur_room_size_ - 1))
            {
                return 6;
            }
        }

        (*(it->second))->set_user_ready_state(true);
        ++m_cur_ready_users_;

        if ((*(it->second))->isMaster())
        {
            m_master_port = (*(it->second))->MasterPort();
        }

        return (*(it->second))->isMaster() ? 1 : 0;
    }

    /*********************************************************
    Function:	userChoseRole
    Description:用户选择角色
    Input:_uid:用户的uid
    Output:
    Return：int:用户选择角色结果  0：用户选择角色成功
                                1：关卡已运行
                                2：用户不存在
                                3：用户已处于准备状态
                                4：角色已被禁止/不存在
                                5：角色已被选择
    **********************************************************/
    int Room::userChoseRole(const UserId &_uid, const RoleId &_roleId)
    {
        if (isRunning())
        {
            return 1;
        }

        auto it = m_map_users_.find(_uid);

        if (it == m_map_users_.end())
        {
            return 2;
        }

        if ((*(it->second))->isReady())
        {
            return 3;
        }

        auto _roles = m_map_role_states_.find(_roleId);

        if (_roles == m_map_role_states_.end() || _roles->second != RoleStateNormal)
        {
            return 4;
        }

        auto itusers = m_map_role_users_.find(_roleId);

        if (itusers != m_map_role_users_.end() && itusers->second != _uid)
        {
            return 5;
        }

        m_map_role_users_[_roleId] = _uid;
        (*(it->second))->set_user_role(_roleId);

        return 0;
    }

    /*********************************************************
    Function:	userChoseLevel
    Description:用户关卡选择
    Input:_userId:待选择用户的id，Level:待选择的关卡
    Output:
    Return:int:返回选择关卡的结果-->
                                    0:选择成功
                                    1：房间忙，无法选择
                                    2: 用户不在该房间
                                    3：非房主选择关卡
                                    4：关卡id不存在
    **********************************************************/
    int Room::userChoseLevel(const UserId &_userId, const LevelId &_levelId)
    {
        if (m_rs_state_ >= RoomState::RoomStateReady)
        {
            return 1;
        }

        auto it = m_map_users_.find(_userId);
        if (it == m_map_users_.end())
        {
            return 2;
        }

        if (m_userid_master_ != _userId)
        {
            return 3;
        }

        if (!m_set_levels.count(_levelId))
        {
            return 4;
        }

        m_lid_level_ = _levelId;

        return 0;
    }

    /*********************************************************
    Function:	findUserById
    Description:根据userid查找用户的结构
    Input:_uid:用户的_uid
    Output:
    Return:bool:是否查找到用户，_ptr:用户的指针
    **********************************************************/
    bool Room::findUserById(const UserId &_uid, User::UserPtr &_ptr)
    {
        auto it = m_map_users_.find(_uid);
        if (it != m_map_users_.end())
        {
            _ptr = *(it->second);
            return true;
        }
        return false;
    }

    /*********************************************************
    Function:	userDisConnect
    Description:用户断开连接时，触发的回收工作
    Input:_ptr:用户的数据指针
    Output:
    Return:int:回调处理的结果：  0:正常处理，用户离开房间，
                                1:正常处理，游戏正在运行，无法退出，标记用户状态，设置游戏
                                2：非法请求
    **********************************************************/
    int Room::userDisConnect(User::UserPtr _userPtr)
    {
        if (!_userPtr || _userPtr->roomId() != m_ll_roomid_)
        {
            return 2;
        }

        // 游戏正在运行，无法直接使用户退出，需等待用户重连
        if (m_rs_state_ == RoomState::RoomStateRunning)
        {
            return 1;
        }

        return userLeave(_userPtr->userId());
    }

    /*********************************************************
    Function:	clear_user_info
    Description:用户离开时，清理用户的状态
    Input:_ptr:用户的数据指针
    Output:
    Return:
    **********************************************************/
    void Room::clear_user_info(User::UserPtr &_ptr)
    {
        assert(_ptr->roomId() == m_ll_roomid_);

        if (_ptr->hasChooseRole())
        {
            RoleId _rid = _ptr->roleId();
            auto it = m_map_role_users_.find(_rid);
            assert(it != m_map_role_users_.end());
            m_map_role_users_.erase(it);
        }

        _ptr->init();
    }

    int Room::MAX_USER_COUNT()
    {
        return MAX_USER_SIZE;
    }

    /*********************************************************
    Function:	find_new_master
    Description:房主离开时，寻找新的房主(默认以第一人作为房主)
    Input:_ptr:用户的数据指针
    Output:
    Return:
    **********************************************************/
    void Room::find_new_master()
    {
        if (m_list_users_.size() == 0)
        {
            return;
        }
        User::UserPtr _ptr = m_list_users_.front();

        m_userid_master_ = _ptr->userId();
        m_master_addr = _ptr->addr();
        m_master_port = _ptr->MasterPort();

        _ptr->set_master(true);

        return;
    }

    bool Room::isTimeOut()
    {
        time_t now = time(nullptr);
        unsigned long long _diff = now - lastOpTime_;
        return _diff >= 120;
    }

} // namespace MiniProject
