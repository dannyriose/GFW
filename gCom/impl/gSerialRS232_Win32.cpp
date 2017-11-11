//Find all ports registered on system


#ifdef WIN32
#include "gSerialRS232Private.h"
#include <objbase.h>
#include <initguid.h>
#include <setupapi.h>
#include <dbt.h>
/*!
    \internal
    Get value of specified property from the registry.
        \a key handle to an open key.
        \a property property name.

        return property value.
*/
static gString getRegKeyValue(HKEY key, LPCTSTR property)
{
    DWORD size = 0;
    DWORD type;
    if (RegQueryValueEx(key, property, NULL, NULL, NULL, &size) != ERROR_SUCCESS)
        return gString();
    gString buff;
#ifndef UNICODE
    buff.alloc(size,gString::UTF);
#else
    buff.alloc(size,gString::ASCII);
#endif
    if (RegQueryValueEx(key, property, NULL, &type, buff, &size) == ERROR_SUCCESS){


    }
    buff.setShared(true);
    return buff;
}

/*!
     \internal
     Get specific property from registry.
     \a devInfoSet pointer to the device information set that contains the interface
        and its underlying device. Returned by SetupDiGetClassDevs() function.
     \a devInfoData pointer to an SP_DEVINFO_DATA structure that defines the device instance.
        this is returned by SetupDiGetDeviceInterfaceDetail() function.
     \a property registry property. One of defined SPDRP_* constants.

     return property string.
 */
static gString getDeviceRegistryProperty(HDEVINFO devInfoSet, PSP_DEVINFO_DATA devInfoData, DWORD property)
{
    DWORD buffSize = 0;
    SetupDiGetDeviceRegistryProperty(devInfoSet, devInfoData, property, NULL, NULL, 0, &buffSize);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        return gString();
    gString buff;
#ifndef UNICODE
    buff.alloc(buffSize,gString::UTF);
#else
    buff.alloc(buffSize,gString::ASCII);
#endif
    SetupDiGetDeviceRegistryProperty(devInfoSet, devInfoData, property, NULL, buff, buffSize, NULL);
    buff.setShared(true);
    return buff;
}

/*!
     \internal
*/
static bool getDeviceDetailsInformation(QextPortInfo *portInfo, HDEVINFO devInfoSet, PSP_DEVINFO_DATA devInfoData
                                 , WPARAM wParam = DBT_DEVICEARRIVAL)
{
    portInfo->friendName = getDeviceRegistryProperty(devInfoSet, devInfoData, SPDRP_FRIENDLYNAME);
    if (wParam == DBT_DEVICEARRIVAL)
        portInfo->physName = getDeviceRegistryProperty(devInfoSet, devInfoData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME);
    portInfo->enumName = getDeviceRegistryProperty(devInfoSet, devInfoData, SPDRP_ENUMERATOR_NAME);

    HKEY devKey = ::SetupDiOpenDevRegKey(devInfoSet, devInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
    portInfo->portName = getRegKeyValue(devKey, TEXT("PortName"));
    ::RegCloseKey(devKey);

    QString hardwareIDs = getDeviceRegistryProperty(devInfoSet, devInfoData, SPDRP_HARDWAREID);
    QRegExp idRx(QLatin1String("VID_(\\w+)&PID_(\\w+)"));
    if (hardwareIDs.toUpper().contains(idRx)) {
        bool dummy;
        portInfo->vendorID = idRx.cap(1).toInt(&dummy, 16);
        portInfo->productID = idRx.cap(2).toInt(&dummy, 16);
        //qDebug() << "got vid:" << vid << "pid:" << pid;
    }
    return true;
}





#endif
