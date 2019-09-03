#ifndef BITSTREAM_H
#define BITSTREAM_H

typedef struct {
	size_t length;
	size_t datasize;
	unsigned char *data;
} BitStream;

extern BitStream *BitStream_new(void);
#ifdef WITH_TESTS
extern BitStream *BitStream_newWithBits(size_t size, unsigned char *bits);
#endif
extern int BitStream_appendNum(BitStream *bstream, size_t bits, unsigned int num);
extern int BitStream_appendBytes(BitStream *bstream, size_t size, unsigned char *data);
#define BitStream_size(__bstream__) (__bstream__->length)
#define BitStream_reset(__bstream__) (__bstream__->length = 0)
extern unsigned char *BitStream_toByte(BitStream *bstream);
extern void BitStream_free(BitStream *bstream);

#endif /* BITSTREAM_H */
