#ifndef __VERSION_H
#define __VERSION_H

//update time 2020.12.24
#define APP_VERSION_STR    "0.5.9.0"
#define APP_VERSION_WSTR   L"0.5.9.0"
#define APP_VER_TIME_STR   "(2021/08/06)"

#define APP_MAJOR_VERSION 0
#define APP_MIDLE_VERSION 5
#define APP_MINOR_VERSION 9
#define APP_PATCH_VERSION 0

#define APP_VERSION 0x00590

#define APP_VERSION_CHECK(major, middle, minor, patch) ((major<<12)|(middle<<8)|(minor<<4)|(patch))


#endif // __VERSION_H

