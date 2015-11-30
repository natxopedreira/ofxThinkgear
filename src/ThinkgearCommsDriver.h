#pragma once

#include "ofMain.h"
#include "thinkgear.h"

#ifdef _WIN32
#include <functional> // tr1 deprecated in VS
#else
#include <tr1/functional>
#endif


/*
 ThinkgearCommsDriver.
 - Uses the downloadable libraries from ThinkGear.
 - osx only at the moment
 */

/**
 * Baud rate for use with TG_Connect() and TG_SetBaudrate().
 */
#define TG_BAUD_1200         1200
#define TG_BAUD_2400         2400
#define TG_BAUD_4800         4800
#define TG_BAUD_9600         9600
#define TG_BAUD_57600       57600
#define TG_BAUD_115200     115200

/**
 * Data format for use with TG_Connect() and TG_SetDataFormat().
 */
#define TG_STREAM_PACKETS      0
#define TG_STREAM_5VRAW        1
#define TG_STREAM_FILE_PACKETS 2

/**
 * Data types that can be requested from TG_GetValue().  Only
 * certain data types are output by certain ThinkGear chips
 * and headsets.  Please refer to the Communications Protocol
 * document for your chip/headset to determine which data types
 * are available for your hardware.
 */
#define TG_DATA_BATTERY             0
#define TG_DATA_POOR_SIGNAL         1
#define TG_DATA_ATTENTION           2
#define TG_DATA_MEDITATION          3
#define TG_DATA_RAW                 4
#define TG_DATA_DELTA               5
#define TG_DATA_THETA               6
#define TG_DATA_ALPHA1              7
#define TG_DATA_ALPHA2              8
#define TG_DATA_BETA1               9
#define TG_DATA_BETA2              10
#define TG_DATA_GAMMA1             11
#define TG_DATA_GAMMA2             12
#define TG_DATA_BLINK_STRENGTH     37


//class ofxThinkgearEventArgs;


struct TGData {
    int status;
    float value;
};


class ThinkgearCommsDriver {
public:
    

    ThinkgearCommsDriver();
    ~ThinkgearCommsDriver();

    bool isReady;
    std::tr1::function<void(int,float)> callback;
    string deviceName;
    int baudRate;
    
    template <class T>
    void setup(string deviceName, int baudRate,T  * listener, void (T::*listenerMethod)(int,float)) {
        this->deviceName = deviceName;
        this->baudRate = baudRate;
        callback = std::tr1::bind(listenerMethod, listener, std::tr1::placeholders::_1, std::tr1::placeholders::_2);
    };
    
    bool connect();
    void update();
    void reset();
        
    int unavailableCount;
    int noConnectionRestartCount;
    int noDataRestartCount;
    
protected:
    //CFURLRef bundleURL; // path reference to bundle
    //CFBundleRef thinkGearBundle; // bundle reference
  

	char *comPortName = NULL;
	int   dllVersion = 0;
	int   connectionId = 0;
	int   packetsRead = 0;
	int   errCode = 0;

	double secondsToRun = 0;
	time_t startTime = 0;
	time_t currTime = 0;
	char  *currTimeStr = NULL;
    
};
