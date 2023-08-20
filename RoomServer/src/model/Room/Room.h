#ifndef MINIPROJECT_ROOM_HPP
#define MINIPROJECT_ROOM_HPP

#include <list>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

#include "../User/User.h"
#include "../room_typedef.h"
#include <time.h>

namespace MiniProject
{
    enum RoomState
    {
        RoomStatePreparation = 0, // 房间处于准备状态
        RoomStateReady = 1,       // 房间已经准备好，等待开始游戏
        RoomStateRunning = 2,     // 正在运行游戏
        RoomStateSettleMent = 3   // 游戏结束，房间处于结算状态
    };

    enum RoleState
    {
        RoleStateBan = 100,    // 角色禁止使用
        RoleStateNormal = 101, //   角色正常使用
    };

    class Room
    {
    public:
        typedef std::shared_ptr<Room> RoomPtr;

    private:
        // 房间的相关信息
        const int MAX_USER_SIZE;
        RoomId m_ll_roomid_;                                     // 房间的id号
        std::string m_str_name_;                                 // 房间的名称
        int m_cur_room_size_ = 0;                                // 用户当前的人数
        int m_cur_ready_users_ = 0;                              // 房间当前处于准备状态的人数
        RoomState m_rs_state_ = RoomState::RoomStatePreparation; // 房间当前所处的状态

        // 房间的房主信息
        UserId m_userid_master_ = 0;    // 房主的id
        std::string m_master_addr = ""; // 房间所使用的战斗服务的ip
        uint m_master_port = 0;         // 房间所使用的战斗服务的port

        // 关卡相关信息
        LevelId m_lid_level_ = 0; // 房间关卡id
        std::set<LevelId> m_set_levels;

        // 玩家相关信息
        std::list<User::UserPtr> m_list_users_;
        std::unordered_map<UserId, std::list<User::UserPtr>::iterator> m_map_users_;

        // 状态相关信息
        std::unordered_map<RoleId, UserId> m_map_role_users_;     // 角色选择情况（需要初始化）
        std::unordered_map<RoleId, RoleState> m_map_role_states_; // 角色使用情况（需要初始化)
        std::vector<int> num_use;
        time_t lastOpTime_;

    public:
        Room(RoomId _roomId, int max_size = 4);
        Room(RoomId _roomId, const std::string &name, int max_size = 2);
        ~Room();

    public:
        bool findUserById(const UserId &_uid, User::UserPtr &_ptr);

        int userEnter(User::UserPtr &_ptr);
        int userLeave(const UserId &_uid);
        int userReady(const UserId &_uid);
        int userChoseRole(const UserId &_uid, const RoleId &_roleId);
        int userChoseLevel(const UserId &_uid, const LevelId &_levelId);

        int userDisConnect(User::UserPtr _userPtr);

        bool isTimeOut();

        int MAX_USER_COUNT();

        void set_status(RoomState _st)
        {
            m_rs_state_ = _st;
        }

        int isRunning()
        {
            return m_rs_state_ >= RoomState::RoomStateReady;
        }

        int status()
        {
            return m_rs_state_;
        }

        int cur_user_count()
        {
            return m_cur_room_size_;
        }

        RoomId GetRoomId()
        {
            return m_ll_roomid_;
        }

        std::list<User::UserPtr> GetTotalUsers()
        {
            return m_list_users_;
        }

        std::string GetMasterAddr()
        {
            return m_master_addr;
        }

        uint GetMasterPort()
        {
            return m_master_port;
        }

        LevelId Level()
        {
            return m_lid_level_;
        }

        std::string name()
        {
            return m_str_name_;
        }

    private:
        void clear_user_info(User::UserPtr &_ptr);
        void find_new_master();
    };

} // namespace MiniProject

#endif