#if HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdlib.h>
#include <string.h>
#if HAVE_LIBPTHREAD
#include <pthread.h>
#endif

#include "rsecc.h"

#if HAVE_LIBPTHREAD
static pthread_mutex_t RSECC_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static int initialized = 0;

#define SYMBOL_SIZE (8)
#define symbols ((1U << SYMBOL_SIZE) - 1)
static const unsigned int proot = 0x11d;

#define min_length (2)
#define max_length (30)
#define max_generatorSize (max_length)

static unsigned char alpha[symbols + 1];
static unsigned char aindex[symbols + 1];
static unsigned char generator[max_length - min_length + 1][max_generatorSize + 1];
static unsigned char generatorInitialized[max_length - min_length + 1];

static void RSECC_initLookupTable(void)
{
	unsigned int i, b;

	alpha[symbols] = 0;
	aindex[0] = symbols;

	b = 1;
	for(i = 0; i < symbols; i++) {
		alpha[i] = b;
		aindex[b] = i;
		b <<= 1;
		if(b & (symbols + 1)) {
			b ^= proot;
		}
		b &= symbols;
	}
}

static void RSECC_init(void)
{
	RSECC_initLookupTable();
	memset(generatorInitialized, 0, (max_length - min_length + 1));
	initialized = 1;
}

static void generator_init(size_t length)
{
	size_t i, j;
	int g[max_generatorSize + 1];

	g[0] = 1;
	for(i = 0; i < length; i++) {
		g[i + 1] = 1;
		for(j = i; j > 0; j--) {
			g[j] = g[j - 1] ^  alpha[(aindex[g[j]] + i) % symbols];
		}
		g[0] = alpha[(aindex[g[0]] + i) % symbols];
	}

	for(i = 0; i <= length; i++) {
		generator[length - min_length][i] = aindex[g[i]];
	}

	generatorInitialized[length - min_length] = 1;
}

int RSECC_encode(size_t data_length, size_t ecc_length, const unsigned char *data, unsigned char *ecc)
{
	size_t i, j;
	unsigned char feedback;
	unsigned char *gen;

#if HAVE_LIBPTHREAD
	pthread_mutex_lock(&RSECC_mutex);
#endif
	if(!initialized) {
		RSECC_init();
	}
#if HAVE_LIBPTHREAD
	pthread_mutex_unlock(&RSECC_mutex);
#endif

	if(ecc_length > max_length) return -1;

	memset(ecc, 0, ecc_length);
#if HAVE_LIBPTHREAD
	pthread_mutex_lock(&RSECC_mutex);
#endif
	if(!generatorInitialized[ecc_length - min_length]) generator_init(ecc_length);
#if HAVE_LIBPTHREAD
	pthread_mutex_unlock(&RSECC_mutex);
#endif
	gen = generator[ecc_length - min_length];

	for(i = 0; i < data_length; i++) {
		feedback = aindex[data[i] ^ ecc[0]];
		if(feedback != symbols) {
			for(j = 1; j < ecc_length; j++) {
				ecc[j] ^= alpha[(unsigned int)(feedback + gen[ecc_length - j]) % symbols];
			}
		}
		memmove(&ecc[0], &ecc[1], ecc_length - 1);
		if(feedback != symbols) {
			ecc[ecc_length - 1] = alpha[(unsigned int)(feedback + gen[0]) % symbols];
		} else {
			ecc[ecc_length - 1] = 0;
		}
	}

	return 0;
}