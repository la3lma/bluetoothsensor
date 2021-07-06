#ifndef BTLEUUIDDECODER_H
#define BTLEUUIDDECODER_H

#include<Arduino.h>

struct BtleUuidEntry {
  int id;
  std::string allocatedFor;
  std::string allocationType;
};

BtleUuidEntry *findUuidEntry(int key);

#endif