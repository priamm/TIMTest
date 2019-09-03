#ifndef QRENCODE_H
#define QRENCODE_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum {
	QR_MODE_NUL = -1,   ///< Terminator (NUL character). Internal use only
	QR_MODE_NUM = 0,    ///< Numeric mode
	QR_MODE_AN,         ///< Alphabet-numeric mode
	QR_MODE_8,          ///< 8-bit data mode
	QR_MODE_KANJI,      ///< Kanji (shift-jis) mode
	QR_MODE_STRUCTURE,  ///< Internal use only
	QR_MODE_ECI,        ///< ECI mode
	QR_MODE_FNC1FIRST,  ///< FNC1, first position
	QR_MODE_FNC1SECOND, ///< FNC1, second position
} QRencodeMode;

typedef enum {
	QR_ECLEVEL_L = 0, ///< lowest
	QR_ECLEVEL_M,
	QR_ECLEVEL_Q,
	QR_ECLEVEL_H      ///< highest
} QRecLevel;

#define QRSPEC_VERSION_MAX 40

#define MQRSPEC_VERSION_MAX 4

typedef struct _QRinput QRinput;

extern QRinput *QRinput_new2(int version, QRecLevel level);

extern QRinput *QRinput_newMQR(int version, QRecLevel level);

extern int QRinput_append(QRinput *input, QRencodeMode mode, int size, const unsigned char *data);

extern int QRinput_getVersion(QRinput *input);

extern int QRinput_setVersion(QRinput *input, int version);

extern void QRinput_free(QRinput *input);

extern int QRinput_check(QRencodeMode mode, int size, const unsigned char *data);

typedef struct _QRinput_Struct QRinput_Struct;

extern QRinput_Struct *QRinput_Struct_new(void);

extern void QRinput_Struct_setParity(QRinput_Struct *s, unsigned char parity);

extern int QRinput_Struct_appendInput(QRinput_Struct *s, QRinput *input);

extern void QRinput_Struct_free(QRinput_Struct *s);

extern QRinput_Struct *QRinput_splitQRinputToStruct(QRinput *input);

extern int QRinput_Struct_insertStructuredAppendHeaders(QRinput_Struct *s);

typedef struct {
	int version;         ///< version of the symbol
	int width;           ///< width of the symbol
	unsigned char *data; ///< symbol data
} QRcode;

typedef struct _QRcode_List {
	QRcode *code;
	struct _QRcode_List *next;
} QRcode_List;

extern QRcode *QRcode_encodeInput(QRinput *input);

extern QRcode *QRcode_encodeString(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive);

extern QRcode *QRcode_encodeStringMQR(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive);

extern QRcode *QRcode_encodeData(int size, const unsigned char *data, int version, QRecLevel level);

extern QRcode *QRcode_encodeDataMQR(int size, const unsigned char *data, int version, QRecLevel level);

extern void QRcode_free(QRcode *qrcode);

extern QRcode_List *QRcode_encodeInputStructured(QRinput_Struct *s);

extern QRcode_List *QRcode_encodeStringStructured(const char *string, int version, QRecLevel level, QRencodeMode hint, int casesensitive);

extern QRcode_List *QRcode_encodeDataStructured(int size, const unsigned char *data, int version, QRecLevel level);

extern void QRcode_List_free(QRcode_List *qrlist);

extern char *QRcode_APIVersionString(void);

/**
 * @deprecated
 */
#ifndef _MSC_VER
extern void QRcode_clearCache(void) __attribute__ ((deprecated));
#else
extern void QRcode_clearCache(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif