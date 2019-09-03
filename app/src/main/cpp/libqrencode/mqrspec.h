#ifndef MQRSPEC_H
#define MQRSPEC_H

#include "qrencode.h"

extern int MQRspec_getDataLengthBit(int version, QRecLevel level);

extern int MQRspec_getDataLength(int version, QRecLevel level);

extern int MQRspec_getECCLength(int version, QRecLevel level);

extern int MQRspec_getWidth(int version);

extern int MQRspec_lengthIndicator(QRencodeMode mode, int version);

extern unsigned int MQRspec_getFormatInfo(int mask, int version, QRecLevel level);

extern unsigned char *MQRspec_newFrame(int version);

#define MQRSPEC_MODEID_NUM       0
#define MQRSPEC_MODEID_AN        1
#define MQRSPEC_MODEID_8         2
#define MQRSPEC_MODEID_KANJI     3

#endif