#ifndef QRENCODE_INNER_H
#define QRENCODE_INNER_H

typedef struct {
	int dataLength;
	int eccLength;
	unsigned char *data;
	unsigned char *ecc;
} RSblock;

typedef struct {
	int version;
	int dataLength;
	int eccLength;
	unsigned char *datacode;
	unsigned char *ecccode;
	int b1;
	int blocks;
	RSblock *rsblock;
	int count;
} QRRawCode;

extern QRRawCode *QRraw_new(QRinput *input);

typedef struct {
	int version;
	int dataLength;
	int eccLength;
	unsigned char *datacode;
	unsigned char *ecccode;
	RSblock *rsblock;
	int oddbits;
	int count;
} MQRRawCode;

extern MQRRawCode *MQRraw_new(QRinput *input);

extern unsigned char *FrameFiller_test(int version);
extern unsigned char *FrameFiller_testMQR(int version);

extern QRcode *QRcode_encodeMask(QRinput *input, int mask);
extern QRcode *QRcode_encodeMaskMQR(QRinput *input, int mask);
extern QRcode *QRcode_new(int version, int width, unsigned char *data);

#endif /