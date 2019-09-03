
#ifndef QRINPUT_H
#define QRINPUT_H

#include "qrencode.h"
#include "bitstream.h"

int QRinput_isSplittableMode(QRencodeMode mode);

typedef struct _QRinput_List QRinput_List;

struct _QRinput_List {
	QRencodeMode mode;
	int size;
	unsigned char *data;
	BitStream *bstream;
	QRinput_List *next;
};

struct _QRinput {
	int version;
	QRecLevel level;
	QRinput_List *head;
	QRinput_List *tail;
	int mqr;
	int fnc1;
	unsigned char appid;
};

typedef struct _QRinput_InputList QRinput_InputList;

struct _QRinput_InputList {
	QRinput *input;
	QRinput_InputList *next;
};

struct _QRinput_Struct {
	int size;
	int parity;
	QRinput_InputList *head;
	QRinput_InputList *tail;
};

extern unsigned char *QRinput_getByteStream(QRinput *input);


extern int QRinput_estimateBitsModeNum(int size);
extern int QRinput_estimateBitsModeAn(int size);
extern int QRinput_estimateBitsMode8(int size);
extern int QRinput_estimateBitsModeKanji(int size);

extern QRinput *QRinput_dup(QRinput *input);

extern const signed char QRinput_anTable[128];

#define QRinput_lookAnTable(__c__) \
	((__c__ & 0x80)?-1:QRinput_anTable[(int)__c__])

#define MODE_INDICATOR_SIZE 4

#define STRUCTURE_HEADER_SIZE 20

#define MAX_STRUCTURED_SYMBOLS 16

#ifdef WITH_TESTS
extern int QRinput_mergeBitStream(QRinput *input, BitStream *bstream);
extern int QRinput_getBitStream(QRinput *input, BitStream *bstream);
extern int QRinput_estimateBitStreamSize(QRinput *input, int version);
extern int QRinput_splitEntry(QRinput_List *entry, int bytes);
extern int QRinput_lengthOfCode(QRencodeMode mode, int version, int bits);
extern int QRinput_insertStructuredAppendHeader(QRinput *input, int size, int index, unsigned char parity);
#endif

#endif