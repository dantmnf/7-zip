// RandGen.cpp

#include "StdAfx.h"

#include "RandGen.h"

#ifdef _WIN32
#include <ntsecapi.h>
#else
#include <unistd.h>
#endif

void CRandomGenerator::Init()
{
#ifdef _WIN32
  HMODULE hMod = LoadLibraryA("advapi32.dll");
  if (!hMod) throw;
  pRtlGenRandom = (FuncRtlGenRandom)GetProcAddress(hMod, "SystemFunction036");
  if (!pRtlGenRandom) throw;
#else
  rng_fd = open("/dev/random", O_RDONLY);
  if (rng_fd == -1) throw;
#endif
  _needInit = false;
}

void CRandomGenerator::Generate(Byte *data, unsigned size)
{
  if (_needInit)
    Init();
#ifdef _WIN32
  if(!pRtlGenRandom((void*)data, size)) throw;
#else
  ssize_t len = read(rng_fd, (void*)data, (size_t)size);
  if (len != size) throw;
#endif
}

#ifndef _WIN32
~CRandomGenerator() {
  if (!_needInit)
  {
    close(rng_fd);
  }
}
#endif

CRandomGenerator g_RandomGenerator;
