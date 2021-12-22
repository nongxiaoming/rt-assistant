#ifndef __TYPEDEFS_H
#define __TYPEDEFS_H

typedef struct {
    QHostAddress IP;
    QString Name;
    QString MAC;
    QString SerialNum;
    quint32 sw_ver;
    quint32 build_utc;
}Device_t;

#endif /* __TYPEDEFS_H */