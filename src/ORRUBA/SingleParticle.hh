#ifndef LIBORR_SINGLEPARTICLE_HH
#define LIBORR_SINGLEPARTICLE_HH

namespace Orruba {
  enum class DetType {
    NoType,
    QQQ5,
    SX3,
    BB10,
    Track,
    TDC
  };

class SingleParticle  {
  public:
    DetType detType; //1=QQQ5, 2=SX3, 3=BB10
    unsigned int detID;
    unsigned int frontID; //strip/ring
    unsigned int backID; //pad/sector
    unsigned int layer; //dE/E

    float frontEnergy;
    float backEnergy;

    float raw_back;
    float raw_frontL;
    float raw_frontR;
      
    bool valid;

    //coordinate system is gretina system: +z along beam axis, +x towards floor, +y towards beam left
    float r; //radius
    float x,y,z; //cartesian
    float theta; //in spherical coordinates, angle from +z axis
    float phi; //azimuth, angle from +x towards +y

    float r_off;
    float x_off, y_off, z_off;
    float theta_off;
    float phi_off;

    SingleParticle() {}
    SingleParticle(DetType dt, unsigned short int did,
                   unsigned short int fid, unsigned short int bid,
                   unsigned short int lay,
                   float fe, float be, bool val) :
      detType(dt), detID(did), frontID(fid), backID(bid), layer(lay),
      frontEnergy(fe), backEnergy(be), valid(val), raw_back(0), raw_frontL(0), raw_frontR(0) {};

    void OffsetBeam(float beamx, float beamy, bool verbose=false);
    void SetRaw(int rb, int rl, int rr) { raw_back = rb; raw_frontL = rl; raw_frontR = rr; }
    
  };
}

#endif
