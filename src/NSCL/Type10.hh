#ifndef DATOR_NSCL_HH
#define DATOR_NSCL_HH

#include <iostream>

#include "Reader/Reader.hh"

//File for processing GEB Type-10 events (timestamped NSCL events)
namespace NSCL {
  //Class for Ring type 21 (scalers?)
  //at present this is an undocumented scaler type
  //all the things I can find online are where the Ring Type = 20, but I have Ring Type = 21 in my data
  //so I do what I can
  class Type21 : public DATOR::Processor {
  public:
    int fresh = 0;
    std::vector<unsigned long long int> scalers;
    std::vector<unsigned long long int> raw_scalers;
    std::vector<unsigned long long int> last_scalers;
    std::vector<unsigned long long int> instant_scalers;
    unsigned long long int timestamp;
    unsigned long long int last_timestamp;
    Type21();
    void Process(unsigned long long int timestamp, unsigned short int *data, unsigned short int length);
    void Reset();
    void Clear();
    void ProcessFinal();
    void PrintSummary(std::ostream &out);
  };
}

#endif
