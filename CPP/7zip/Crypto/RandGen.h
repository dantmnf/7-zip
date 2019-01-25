// RandGen.h

#ifndef __CRYPTO_RAND_GEN_H
#define __CRYPTO_RAND_GEN_H

#include "../../../C/7zTypes.h"

#ifdef _WIN32
typedef BOOL (WINAPI *FuncRtlGenRandom)(void *dest, unsigned long size);
#endif

class CRandomGenerator
{
#ifdef _WIN32
  FuncRtlGenRandom pRtlGenRandom;
#else
  int rng_fd;
#endif
  bool _needInit;

  void Init();
public:
  CRandomGenerator(): _needInit(true) {};
  void Generate(Byte *data, unsigned size);
#ifndef _WIN32
  ~CRandomGenerator();
#endif
};

extern CRandomGenerator g_RandomGenerator;

#endif
