#include "dpsdata.h"

#include <qdebug.h>
DpsData::DpsData()
{
    uset_rw   = 0.0;
    iset_rw   = 0.0;
    uout_r    = 0.0;
    iout_r    = 0.0;
    power_r   = 0.0;
    uin_r     = 0.0;
    lock_rw   = 0;
    protect_r = 0;
    cvcc_r    = 0;
    on_off    = 0;
    bled_rw   = 0;
    model_r   = 0;
    version_r = 0;
}

void DpsData::dumpData()
{
    qDebug() << "uset:" << uset_rw << "\tiset:" << iset_rw << "\tuout:" << uout_r << "\tpower:" << power_r <<
      "\tuin:" << uin_r << "\tlock:" << lock_rw << "\tprotect:" << protect_r << "\tcvcc:" << cvcc_r;
    //  "\ton_off:" << on_off << "\tbled:" << bled_rw << "\tmodel:" << model_r << "\tversion:" << version_r;

}


void DpsData::setValue(int no,int data)
{

    //qDebug() << "setValue : " << no << " data: " << data;

    float f = data;
    switch(no)
    {
    case 0:   uset_rw=f/100.0;break;  // DP=2
    case 1:   iset_rw=f/1000.0;break;  // DP=3
    case 2:   uout_r=f/100.0;break;  // DP=2
    case 3:   iout_r=f/1000.0;break;  // DP=3
    case 4:   power_r=f/10.0;break;  // DP=1
    case 5:   uin_r=f/100.0;break;  // DP=2
    case 6:   lock_rw=data;break;
    case 7:   protect_r=data;break;
    case 8:   cvcc_r=data;break;
    case 9:   on_off=data;break;
    case 10:  bled_rw=data;break;
    case 11:  model_r=data;break;
    case 12:  version_r=data;break;


    default:
        qDebug() << "setValue unknonw: " << no << " data: " << data;

    }

}

float DpsData::getVI()
{
    return uin_r;
}

float DpsData::getVO()
{
    return uout_r;
}



QString DpsData::getValue(int no)
{
    QString ret;
    switch(no)
    {
    case 0:   ret = QString::number(uset_rw, 'g', 2);break;
    case 1:   ret = QString::number(iset_rw, 'g', 2);break;
    case 2:   ret = QString::number(uout_r, 'g', 2);break;
    case 3:   ret = QString::number(iout_r, 'g', 2);break;
    case 4:   ret = QString::number(power_r, 'g', 2);break;
    case 5:   ret = QString::number(uin_r, 'g', 2);break;
    //case 6:   lock_rw=data;break;
    //case 7:   protect_r=data;break;
    //case 8:   cvcc_r=data;break;
    //case 9:   on_off=data;break;
    //case 10:  bled_rw=data;break;
    //case 11:  model_r=data;break;
    //case 12:  version_r=data;break;


    default:
        qDebug() << "getValue unknonw no: " << no;

    }
    return ret;
}
