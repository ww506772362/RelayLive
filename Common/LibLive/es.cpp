#include "stdafx.h"
#include "es.h"


CES::CES(CLiveObj* pObj)
    : m_pObj(pObj)
{
}


CES::~CES(void)
{
}

int CES::InputBuffer(char* pBuf, uint32_t nLen)
{
    // ES包没有es头，由h264片组成
    //nal_unit_header_t* nalUnit = nullptr;
    //uint8_t nNalType = 0;
    uint32_t pos = 0;
    uint32_t begin_pos = 0;
    char* begin_buff = pBuf;
    while (pos < nLen)
    {
        char* pPos = pBuf + pos;
        if (pPos[0] == 0 && pPos[1] == 0 && pPos[2] == 1)
        {
            //if(pos > 0) Log::debug("nal3 begin width 001 pos:%d",pos);
            // 是h264报文开头
            if (pos > begin_pos)
            {
                // 回调处理H264帧
                if (m_pObj != nullptr)
                {
                    m_pObj->ESParseCb(begin_buff, pos-begin_pos/*, nNalType*/);
                }
            }
            //nalUnit = (nal_unit_header_t*)(pPos+3);
            //nNalType = nalUnit->nal_type;
            begin_pos = pos;
            begin_buff = (char*)pPos;
            pos += 3;
        }
        else if (pPos[0] == 0 && pPos[1] == 0 && pPos[2] == 0 && pPos[3] == 1)
        {
			//if(pos > 0) Log::debug("nal4 begin width 0001 pos:%d",pos);
            // 是h264报文开头
            if (pos > begin_pos)
            {
                // 回调处理H264帧
                if (m_pObj != nullptr)
                {
                    m_pObj->ESParseCb(begin_buff, pos-begin_pos/*, nNalType*/);
                }
            }
            //nalUnit = (nal_unit_header_t*)(pPos+4);
            //nNalType = nalUnit->nal_type;
            begin_pos = pos;
            begin_buff = (char*)pPos;
            pos += 4;
        }
        else
        {
            // 不是h264开头
            pos++;
        }
    }
    // 最后一帧
    if (nLen > begin_pos)
    {
        // 回调处理H264帧
        if (m_pObj != nullptr)
        {
            m_pObj->ESParseCb(begin_buff, nLen-begin_pos/*, nNalType*/);
        }
    }
    return 0;
}