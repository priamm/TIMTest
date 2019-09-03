#ifndef SPLIT_H
#define SPLIT_H

#include "qrencode.h"

extern int Split_splitStringToQRinput(const char *string, QRinput *input,
									  QRencodeMode hint, int casesensitive);

#endif