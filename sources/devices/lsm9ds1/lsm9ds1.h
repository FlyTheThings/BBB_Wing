#ifndef LSM9DS1_H
#define LSM9DS1_H

#include "i2c_device.h"

namespace devices
{
    class Lsm9ds1
    {
        class GyroAccel;
        class Mag;

    public:
        Lsm9ds1();
        ~Lsm9ds1();

        bool start(const char* filename = I2cDevice::defaultI2cPath);
        void stop();
        bool isStarted() const;
        bool checkDevicePresent();

        GyroAccel* gyroAccel() const;
        Mag* mag() const;

    private:
        GyroAccel* m_gyroAccel;
        Mag* m_mag;
    };
}

#endif // LSM9DS1_H