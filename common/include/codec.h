 #ifndef MiniProjectCodeC_H
#define MiniProjectCodeC_H

#include <memory>
#include <functional>
#include <stdint.h>
#include <string>
#include <google/protobuf/message.h>

#include "buffer.h"

namespace MiniProject
{
    static const std::int16_t PB_START = 0xEE; // start char
    static const std::int16_t PB_END = 0xEE;   // end char
    static const std::int32_t MSG_LEN = 20;

    static const std::int32_t LEN_START = 2;
    static const std::int32_t LEN_CMD_ID = 4;
    static const std::int32_t LEN_CMD_LEN = 4;
    static const std::int32_t LEN_ERR_CODE = 4;
    static const std::int32_t LEN_CHECK_NUM = 4;
    static const std::int32_t LEN_END = 2;

    typedef std::function<std::int32_t(const std::string &)> CheckCallback;

    enum MsgParseState
    {
        MSG_NONE,
        MSG_START,
        MSG_CMD_ID,
        MSG_CMD_LEN,
        MSG_ERR_CODE,
        MSG_CHECK_NUM,
        MSG_CONTENT,
        MSG_END
    }; 

    class ProtoCodec
    {
    public:
        typedef std::shared_ptr<ProtoCodec> pb_ptr;
        ProtoCodec() = default;
        ~ProtoCodec() = default;
        ProtoCodec(const ProtoCodec &) = default;
        ProtoCodec &operator=(const ProtoCodec &) = default;
        ProtoCodec(ProtoCodec &&) = default;
        ProtoCodec &operator=(ProtoCodec &&) = default;

        void setCheckCallBack(CheckCallback cb_);

        void clear();

        std::string encodebytes(const std::string &message, std::int32_t cmd_id, int &len);

        std::string encodebytes(google::protobuf::Message *message, std::int32_t cmd_id, int &len);

        MsgParseState decodedata(TcpBuffer *buffer, MsgParseState curst);

        int decode(const char *data, int length);

        std::string data()
        {
            return pb_data;
        }

        inline std::int32_t cmd_id()
        {
            return cmd_id_;
        }

        inline std::int32_t packet_len()
        {
            return cmd_len;
        }

    private:
        CheckCallback checkCb_;

    private:
        std::int16_t start;        // 报文的开始
        std::int32_t cmd_id_;      // 封装的命令的编号
        std::int32_t cmd_len;      // protobuf序列化数据的长度
        std::int32_t err_code{0};  // 错误代码. 0 代表调用正常，非 0 代表调用失败
        std::int32_t check_num{0}; // 包的检验和，用于检验包数据是否有损坏
        std::string pb_data;       // 业务 protobuf 数据，由 google 的 protobuf 序列化后得到d
        int16_t end;               // 报文结束
    };

}

#endif