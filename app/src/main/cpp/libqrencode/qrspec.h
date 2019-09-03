#ifndef QRSPEC_H
#define QRSPEC_H

#include "qrencode.h"

#define QRSPEC_WIDTH_MAX 177

extern int QRspec_getDataLength(int version, QRecLevel level);

extern int QRspec_getECCLength(int version, QRecLevel level);

extern int QRspec_getMinimumVersion(int size, QRecLevel level);

extern int QRspec_getWidth(int version);

extern int QRspec_getRemainder(int version);

extern int QRspec_lengthIndicator(QRencodeMode mode, int version);

extern int QRspec_maximumWords(QRencodeMode mode, int version);

void QRspec_getEccSpec(int version, QRecLevel level, int spec[5]);

#define QRspec_rsBlockNum(__spec__) (__spec__[0] + __spec__[3])
#define QRspec_rsBlockNum1(__spec__) (__spec__[0])
#define QRspec_rsDataCodes1(__spec__) (__spec__[1])
#define QRspec_rsEccCodes1(__spec__) (__spec__[2])
#define QRspec_rsBlockNum2(__spec__) (__spec__[3])
#define QRspec_rsDataCodes2(__spec__) (__spec__[4])
#define QRspec_rsEccCodes2(__spec__) (__spec__[2])

#define QRspec_rsDataLength(__spec__) \
	((QRspec_rsBlockNum1(__spec__) * QRspec_rsDataCodes1(__spec__)) + \
	 (QRspec_rsBlockNum2(__spec__) * QRspec_rsDataCodes2(__spec__)))
#define QRspec_rsEccLength(__spec__) \
	(QRspec_rsBlockNum(__spec__) * QRspec_rsEccCodes1(__spec__))

extern unsigned int QRspec_getVersionPattern(int version);

extern unsigned int QRspec_getFormatInfo(int mask, QRecLevel level);

extern unsigned char *QRspec_newFrame(int version);

#define QRSPEC_MODEID_ECI        7
#define QRSPEC_MODEID_NUM        1
#define QRSPEC_MODEID_AN         2
#define QRSPEC_MODEID_8          4
#define QRSPEC_MODEID_KANJI      8
#define QRSPEC_MODEID_FNC1FIRST  5
#define QRSPEC_MODEID_FNC1SECOND 9
#define QRSPEC_MODEID_STRUCTURE  3
#define QRSPEC_MODEID_TERMINATOR 0

#endif