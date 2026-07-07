#include <iomanip>

#include "Type10.hh"

//File for processing GEB Type-10 events
namespace NSCL {
  Type21::Type21() {};
  void Type21::Reset() { fresh = 0; };
  void Type21::Clear() { 
    scalers.clear(); 
    raw_scalers.clear(); 
    instant_scalers.clear();
  }
  void Type21::Process(unsigned long long ts, unsigned short int *data, unsigned short int length) {
     unsigned int size=*((unsigned int*)&data[0]);
     unsigned int type=*((unsigned int*)&data[2]); 
     unsigned int offset = 4;
     if (type==21) { 
       last_timestamp = timestamp;
       timestamp = ts;
       if (timestamp == last_timestamp) { //timetamps are equal
         std::cout << "scaler records with identical timetamps found, skipping second" << std::endl;
         return;
       }
       fresh = 1;
       //the initial words I'm not sure how to interpret
       offset += 4;
       offset += 4*2;
       unsigned int scalerCount = *((unsigned int*)&data[offset]);
       offset += 2;
       last_scalers = scalers;
       scalers.clear(); //only clear if new data about to be processed
       raw_scalers.clear(); //only clear if new data about to be processed
       instant_scalers.clear();
       scalers.reserve(scalerCount); 
       raw_scalers.reserve(scalerCount); 
       instant_scalers.reserve(scalerCount); 
       for (int i=0; i<scalerCount; ++i) {
          unsigned long long int scalerVal = *((unsigned int*)&data[offset]);
          offset += 2;
          scalers.push_back(scalerVal);
          raw_scalers.push_back(scalerVal);
          if ((last_scalers.size() > 0) ) {
            //if (i==18 && timestamp/1e8 > 1530 && timestamp/1e8 < 1580) { 
            //  std::cout << i <<" checking for overflows: " << scalerCount << "   " << size <<  std::endl;
            //  std::cout << timestamp << "   " << timestamp/1e8 << "   " << scalers[i] << "   " << last_scalers[i] << std::endl;
            //}
            while (scalers[i] < last_scalers[i]) { //overflow, seems to be 2^24 max
              scalers[i] += (1<<24);
            }
            //if (i==18 && timestamp/1e8 > 1530 && timestamp/1e8 < 1580) { 
            //  std::cout << timestamp << "   " << timestamp/1e8 << "   " << scalers[i] << "   " << last_scalers[i] << std::endl;
            //}
            if (scalers[i] == last_scalers[i]) { 
              //no change since last scalers
              //std::cout << "no change in scaler " << i << std::endl;
              instant_scalers.push_back(0);
              continue;
            }
            if (last_timestamp != timestamp) {
              instant_scalers.push_back(((double)scalers[i] - (double)last_scalers[i])/(double)((timestamp - last_timestamp)/1e8)); 
              if ((timestamp-last_timestamp)/1e8 < 0.0001) { 
                std::cout << i << "   " << scalers[i] << "   " << last_scalers[i] << "   " << timestamp << "   " << last_timestamp << "   " << instant_scalers.back() << std::endl;
              }
            }
            //else {
            //  std::cout << "timestamps are equal" << std::endl;
            //  std::cout << "this is weird" << std::endl;
            //}
	    //if (i==18) { std::cout << ((double)scalers[i] - (double)last_scalers[i])/(double)((timestamp - last_timestamp)/1e8) << "   " << scalers[i] << "   " << last_scalers[i] << "   " << ((double)scalers[i] - (double)last_scalers[i]) << "   " << timestamp << "   " << last_timestamp << std::endl; }
          }
       }
     }
  }
  void Type21::ProcessFinal() {}
  void Type21::PrintSummary(std::ostream &out) {
    out << "--------- Type 21 NSCL Scaler Summary ---------" << std::endl;
    for (int i=0; i<scalers.size()/2; ++i) {
     
       int indx = i;
       out << "    SC"<<std::setw(2) << i<< " : ";
       if (scalers[indx] == 0) {
         out << ANSI_COLOR_GREY;
       }
       else if (scalers[indx] < 100000) {
         out << ANSI_COLOR_GREEN;
       }
       else if (scalers[indx] < 10000000) {
         out << ANSI_COLOR_YELLOW;
       }
       else {
         out << ANSI_COLOR_RED;
       }
       out << std::setw(12) << scalers[indx] <<std::setw(0);
       out << ANSI_COLOR_RESET;

       indx = i+scalers.size()/2;
       out << "      SC"<< std::setw(2) << indx << " : ";
       if (scalers[indx] == 0) {
         out << ANSI_COLOR_GREY;
       }
       else if (scalers[indx] < 100000) {
         out << ANSI_COLOR_GREEN;
       }
       else if (scalers[indx] < 10000000) {
         out << ANSI_COLOR_YELLOW;
       }
       else {
         out << ANSI_COLOR_RED;
       }
       out << std::setw(12) << scalers[indx] << std::endl;
       out << ANSI_COLOR_RESET;
       out << std::setw(0);
    }
  }
}
