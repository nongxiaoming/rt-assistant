#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <QObject>


#define FRAME_SYNC1 ((char)0xAA)
#define FRAME_SYNC2 ((char)0xBB)
#define FRAME_END   (0x55)


typedef enum
{
 CMD_MODULE_HEARTBEAT = 0x00,

 CMD_GET_MODULE_INFO = 0x01,

 CMD_LED_CTRL = 0x02,

 CMD_KEY_STATE = 0x03,

 CMD_FADE_DATA = 0x04,

 CMD_ENCODER_DATA = 0x05,

 CMD_DMX_DATA = 0x06,

 CMD_DMX_RDM = 0x07,

 CMD_NETWORK_CONFIG = 0x08,

 CMD_CONFIG_DEFAULT  = 0xf0,

 CMD_DEVICE_RESET  = 0xf1,

 CMD_UPGRADE_REQ = 0xfb,

 CMD_UPGRADE_INFO = 0xfc,

 CMD_UPGRADE_PACK = 0xfd,

 CMD_UPGRADE_FINISH = 0xfe,

 CMD_DISCOVERY = 0xff,

}cmd_t;


typedef enum
{
    BOARD_UNKNOW = 0x00,

    BOARD_ID1 = 0x01,

    BOARD_ID2 = 0x02,

    BOARD_ID3 = 0x03,

} board_id_t;


#endif  /* endif __PROTOCOL_H__ */
