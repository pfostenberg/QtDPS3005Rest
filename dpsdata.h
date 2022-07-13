#ifndef DPSDATA_H
#define DPSDATA_H

#include <QString>

/*
 *
Register Map for this device.
Function Description Number           of bytes Dp UNIT Read/Write Register address
U-SET    Voltage setting              2        2  V    R/W        0000H
I-SET    Current setting              2        3  A    R/W        0001H
UOUT     Output voltage display value 2        2  V    R          0002H
IOUT     Output current display value 2        2  A    R          0003H
POWER    Output power display value   2       1/2 W    R          0004H
UIN      Input voltage display value  2        2  V    R          0005H
LOCK     Key lock                     2        0  -    R/W        0006H
PROTECT  Protection status            2        0  -    R          0007H
CV/CC    Constant Voltage / C.Current 2        0  -    R          0008H
ONOFF    Switch output state          2        0  -    R/W        0009H
B_LED    Backlight brightness level   2        0  -    R/W        000AH
MODEL    Product model                2        0  -    R          000BH
VERSON   Firmware Version             2        0  -    R          000CH
 */
class DpsData
{
public:
    DpsData();
    void setValue(int no,int data);
    void dumpData();
    QString getValue(int i);
    float   getVI();
    float   getVO();


private:
    float uset_rw;
    float iset_rw;
    float uout_r;
    float iout_r;
    float power_r;
    float uin_r;
    int   lock_rw;
    int   protect_r;
    int   cvcc_r;
    int   on_off;
    int   bled_rw;
    int   model_r;
    int  version_r;
};

#endif // DPSDATA_H
