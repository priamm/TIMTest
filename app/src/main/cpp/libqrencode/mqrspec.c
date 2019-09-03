#if HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "mqrspec.h"

typedef struct {
	int width;
	int ec[4];
} MQRspec_Capacity;

static const MQRspec_Capacity mqrspecCapacity[MQRSPEC_VERSION_MAX + 1] = {
		{  0, {0,  0,  0, 0}},
		{ 11, {2,  0,  0, 0}},
		{ 13, {5,  6,  0, 0}},
		{ 15, {6,  8,  0, 0}},
		{ 17, {8, 10, 14, 0}}
};

int MQRspec_getDataLengthBit(int version, QRecLevel level)
{
	int w;
	int ecc;

	w = mqrspecCapacity[version].width - 1;
	ecc = mqrspecCapacity[version].ec[level];
	if(ecc == 0) return 0;
	return w * w - 64 - ecc * 8;
}

int MQRspec_getDataLength(int version, QRecLevel level)
{
	return (MQRspec_getDataLengthBit(version, level) + 4) / 8;
}

int MQRspec_getECCLength(int version, QRecLevel level)
{
	return mqrspecCapacity[version].ec[level];
}

int MQRspec_getWidth(int version)
{
	return mqrspecCapacity[version].width;
}

static const int lengthTableBits[4][4] = {
		{ 3, 4, 5, 6},
		{ 0, 3, 4, 5},
		{ 0, 0, 4, 5},
		{ 0, 0, 3, 4}
};

int MQRspec_lengthIndicator(QRencodeMode mode, int version)
{
	return lengthTableBits[mode][version - 1];
}

static const unsigned int formatInfo[4][8] = {
		{0x4445, 0x55ae, 0x6793, 0x7678, 0x06de, 0x1735, 0x2508, 0x34e3},
		{0x4172, 0x5099, 0x62a4, 0x734f, 0x03e9, 0x1202, 0x203f, 0x31d4},
		{0x4e2b, 0x5fc0, 0x6dfd, 0x7c16, 0x0cb0, 0x1d5b, 0x2f66, 0x3e8d},
		{0x4b1c, 0x5af7, 0x68ca, 0x7921, 0x0987, 0x186c, 0x2a51, 0x3bba}
};

static const int typeTable[MQRSPEC_VERSION_MAX + 1][3] = {
		{-1, -1, -1},
		{ 0, -1, -1},
		{ 1,  2, -1},
		{ 3,  4, -1},
		{ 5,  6,  7}
};

unsigned int MQRspec_getFormatInfo(int mask, int version, QRecLevel level)
{
	int type;

	if(mask < 0 || mask > 3) return 0;
	if(version <= 0 || version > MQRSPEC_VERSION_MAX) return 0;
	if(level == QR_ECLEVEL_H) return 0;
	type = typeTable[version][level];
	if(type < 0) return 0;

	return formatInfo[mask][type];
}

static void putFinderPattern(unsigned char *frame, int width, int ox, int oy)
{
	static const unsigned char finder[] = {
			0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
			0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
			0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
			0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
			0xc1, 0xc0, 0xc1, 0xc1, 0xc1, 0xc0, 0xc1,
			0xc1, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc1,
			0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
	};
	int x, y;
	const unsigned char *s;

	frame += oy * width + ox;
	s = finder;
	for(y = 0; y < 7; y++) {
		for(x = 0; x < 7; x++) {
			frame[x] = s[x];
		}
		frame += width;
		s += 7;
	}
}

static unsigned char *MQRspec_createFrame(int version)
{
	unsigned char *frame, *p, *q;
	int width;
	int x, y;

	width = mqrspecCapacity[version].width;
	frame = (unsigned char *)malloc((size_t)(width * width));
	if(frame == NULL) return NULL;

	memset(frame, 0, (size_t)(width * width));
	putFinderPattern(frame, width, 0, 0);
	p = frame;
	for(y = 0; y < 7; y++) {
		p[7] = 0xc0;
		p += width;
	}
	memset(frame + width * 7, 0xc0, 8);
	memset(frame + width * 8 + 1, 0x84, 8);
	p = frame + width + 8;
	for(y = 0; y < 7; y++) {
		*p = 0x84;
		p += width;
	}
	p = frame + 8;
	q = frame + width * 8;
	for(x = 1; x < width-7; x++) {
		*p =  0x90 | (x & 1);
		*q =  0x90 | (x & 1);
		p++;
		q += width;
	}

	return frame;
}

unsigned char *MQRspec_newFrame(int version)
{
	if(version < 1 || version > MQRSPEC_VERSION_MAX) return NULL;

	return MQRspec_createFrame(version);
}