#include "codec.h"

#include "spdlog/spdlog.h"

#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

namespace MiniProject

{

    /*********************************************************
    Function:	setCheckCallBack
    Description:清空存储的消息记录
    Input:cb:待设置的校验函数
    Output:
    Return:
    **********************************************************/
    void ProtoCodec::setCheckCallBack(CheckCallback cb)
    {
        checkCb_ = cb;
    }

    /*********************************************************
    Function:	clear
    Description:清空存储的消息记录
    Input:
    Output:
    Return:
    **********************************************************/
    void ProtoCodec::clear()
    {
        pb_data.clear();
    }

    /*********************************************************
    Function:	encodebytes
    Description:将消息编码成报文
    Input:message：消息，cmd_id:消息id,len:待存储编码消息长度
    Output:
    Return:char:编码后的消息
    **********************************************************/
    std::string ProtoCodec::encodebytes(google::protobuf::Message *message, std::int32_t cmd_id, int &len)
    {
        if (message == nullptr)
        {
            spdlog::error("Encode Message lacks key information???");
            return "";
        }

        return encodebytes(message->SerializeAsString(), cmd_id, len);
    }

    /*********************************************************
    Function:	decodedata
    Description:通过有限状态机解码报文，转化为消息
    Input:buffer:用户接收区缓冲，curst：当前状态机状态
    Output:
    Return:char:MsgParseState：解码后状态
    **********************************************************/
    MsgParseState ProtoCodec::decodedata(TcpBuffer *buffer, MsgParseState curst)
    {
        if (curst == MsgParseState::MSG_NONE)
        {
            curst = MsgParseState::MSG_START;
        }
        int bufsize = buffer->readAble();

        if (curst == MSG_START)
        {
            if (bufsize >= LEN_START)
            {
                memcpy(&start, &buffer->m_buffer[buffer->readIndex()], LEN_START);
                int16_t st = ntohs(start);
                if (st != PB_START)
                {
                    spdlog::error("Get Header error {},{},{}", st, start, PB_START);
                    buffer->recycleRead(1);
                    return MSG_NONE;
                }
                buffer->recycleRead(LEN_START);
                curst = MSG_CMD_ID;
            }
        }
        else if (curst == MSG_CMD_ID)
        {
            if (bufsize >= LEN_CMD_ID)
            {
                memcpy(&cmd_id_, &buffer->m_buffer[buffer->readIndex()], LEN_CMD_ID);

                // 待补充：校验cmd_id的代码

                cmd_id_ = ntohl(cmd_id_);

                buffer->recycleRead(LEN_CMD_ID);
                curst = MSG_CMD_LEN;
            }
        }
        else if (curst == MSG_CMD_LEN)
        {
            if (bufsize >= LEN_CMD_LEN)
            {
                memcpy(&cmd_len, &buffer->m_buffer[buffer->readIndex()], LEN_CMD_LEN);

                // 待补充：校验cmd_len的代码

                buffer->recycleRead(LEN_CMD_LEN);
                curst = MSG_ERR_CODE;
            }
        }
        else if (curst == MSG_ERR_CODE)
        {
            if (bufsize >= LEN_ERR_CODE)
            {
                memcpy(&err_code, &buffer->m_buffer[buffer->readIndex()], LEN_ERR_CODE);
                err_code = ntohl(err_code);
                // 待补充：校验err_code的代码

                // 待补充: 错误码详细定义以及错误处理方法

                buffer->recycleRead(LEN_ERR_CODE);
                curst = MSG_CHECK_NUM;
            }
        }
        else if (curst == MSG_CHECK_NUM)
        {
            if (bufsize >= LEN_CHECK_NUM)
            {
                memcpy(&check_num, &buffer->m_buffer[buffer->readIndex()], LEN_CHECK_NUM);

                // 待补充：校验err_code的代码

                // 待补充: 错误码详细定义以及错误处理方法

                buffer->recycleRead(LEN_CHECK_NUM);
                curst = MSG_CONTENT;
            }
        }
        else if (curst == MSG_CONTENT)
        {
            int len = ntohl(cmd_len);

            if (bufsize >= len)
            {
                pb_data.resize(len);
                memcpy(&pb_data[0], &buffer->m_buffer[buffer->readIndex()], len);

                buffer->recycleRead(len);
                curst = MSG_END;
            }
        }
        else if (curst == MSG_END)
        {
            memcpy(&end, &buffer->m_buffer[buffer->readIndex()], LEN_END);
            int16_t st = ntohs(end);
            if (st != PB_END)
            {
                spdlog::error("Get Tail error");
                buffer->recycleRead(1);
                return MSG_NONE;
            }
            buffer->recycleRead(LEN_END);
            curst = MSG_START;
        }
        else
        {
            spdlog::error("Error state,change to start");
            curst = MSG_START;
        }
        return curst;
    }

    /*********************************************************
    Function:	decode
    Description:直接解析包
    Input:data：缓存数据，length:缓存数据长度
    Output:
    Return:char:MsgParseState：解码后状态
    **********************************************************/
    int ProtoCodec::decode(const char *data, int len)
    {
        int _offset = 0;
        if (_offset + LEN_START > len)
        {
            return -1;
        }

        int16_t _start;
        memcpy(&_start, data + _offset, LEN_START);
        _start = ntohs(_start);
        _offset += LEN_START;

        if (_start != PB_START)
        {
            spdlog::info("Check Header error(start = {},{})", _start, start);
            return -1;
        }

        if (_offset + LEN_CMD_ID > len)
        {
            return -1;
        }

        memcpy(&cmd_id_, data + _offset, LEN_CMD_ID);
        cmd_id_ = ntohl(cmd_id_);
        _offset += LEN_CMD_ID;

        if (_offset + LEN_CMD_LEN > len)
        {
            spdlog::info("Check Header error(start = {},{})", _start, start);
            return -1;
        }

        memcpy(&cmd_len, data + _offset, LEN_CMD_LEN);
        cmd_len = ntohl(cmd_len);
        _offset += LEN_CMD_LEN;

        if (MSG_LEN + cmd_len != len)
        {
            spdlog::info("Check Header error(length err = {},{})", MSG_LEN + cmd_len, len);
            return -1;
        }

        memcpy(&err_code, data + _offset, LEN_ERR_CODE);
        err_code = ntohl(err_code);
        _offset += LEN_ERR_CODE;
        if (err_code != 0)
        {
            spdlog::info("Check Header error(error code = {},{})", err_code, 0);
            return 1;
        }

        memcpy(&check_num, data + _offset, LEN_CHECK_NUM);
        check_num = ntohl(check_num);
        _offset += LEN_CHECK_NUM;

        if (check_num != 0)
        {
            spdlog::info("Check Header error(check_num code = {},{})", check_num, 0);
            // 手动完成校验，先省略
            return 2;
        }

        pb_data = std::string(cmd_len, '\0');
        memcpy(&pb_data[0], data + _offset, cmd_len);
        _offset += cmd_len;

        int16_t _end;
        memcpy(&_end, data + _offset, LEN_END);
        _end = ntohs(_end);
        _offset += LEN_END;

        if (_end != PB_END)
        {
            spdlog::info("Check Header error(end code = {},{})", _end, PB_END);
            return -1;
        }
        return 0;
    }

    std::string ProtoCodec::encodebytes(const std::string &message, std::int32_t cmd_id, int &len)
    {
        if (message.empty())
        {
            spdlog::error("Encode Message lacks key information");
            return "";
        }

        int totallen = MSG_LEN + message.size();

        std::string data(totallen, '\0');

        // char *data = (char *)malloc(totallen);

        if (data.empty())
        {
            spdlog::error("Data Malloc error");
            return nullptr;
        }

        int pos = 0;

        int16_t _start = htons(PB_START);

        memcpy(&data[pos], &_start, LEN_START);

        pos += LEN_START;

        int32_t _cmd_id = htonl(cmd_id);

        memcpy(&data[pos], &_cmd_id, LEN_CMD_ID);

        pos += LEN_CMD_ID;

        int32_t bytesize = message.size();

        bytesize = htonl(bytesize);

        memcpy(&data[pos], &bytesize, LEN_CMD_LEN);

        pos += LEN_CMD_LEN;

        // 暂不设置error
        int32_t _err_code = 0;

        _err_code = htonl(_err_code);

        memcpy(&data[pos], &_err_code, LEN_ERR_CODE);

        pos += LEN_ERR_CODE;

        int32_t _check_num = 0;

        if (checkCb_)
        {
            _check_num = checkCb_(pb_data);
            _check_num = htonl(_check_num);
        }

        memcpy(&data[pos], &_check_num, LEN_CHECK_NUM);

        pos += LEN_CHECK_NUM;

        memcpy(&data[pos], &message[0], message.size());

        pos += message.size();

        int16_t _end = htons(PB_END);

        memcpy(&data[pos], &_end, LEN_END);

        pos += LEN_END;

        if (pos != totallen)
        {
            spdlog::error("Data Copy error");
            return std::string();
        }
        len = totallen;
        return data;
    }

}