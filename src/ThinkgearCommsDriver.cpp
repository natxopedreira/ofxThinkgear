#include "ThinkgearCommsDriver.h"


ThinkgearCommsDriver::ThinkgearCommsDriver() {

    isReady = false;
	connectionId = -1;

    //
	//

    unavailableCount = 0;
    noConnectionRestartCount = 500;
    noDataRestartCount = 1500;
}

ThinkgearCommsDriver::~ThinkgearCommsDriver(){

    reset();
}

void ThinkgearCommsDriver::reset() {
    
    if(connectionId != -1){
        ofLogVerbose() << "Disconnecting ThinkGear device.";
       // TG_Disconnect(connectionId);
        TG_FreeConnection(connectionId); //pincha al cerrar
    }
}


bool ThinkgearCommsDriver::connect() {

    reset();
    
	comPortName = "\\\\.\\COM3";

	// driver version
	dllVersion = TG_GetDriverVersion();
	printf("ThinkGear DLL version: %d\n", dllVersion);

	// id conection
	connectionId = TG_GetNewConnectionId();
	printf("ThinkGear connectionId: %d\n", connectionId);


	if (connectionId < 0) {
		fprintf(stderr, "ERROR: TG_GetNewConnectionId() returned %d.\n",connectionId);
	}

    // attempt to connect    
    int conResult = TG_Connect(connectionId, deviceName.c_str(), TG_BAUD_57600, TG_STREAM_PACKETS);

	ofLogVerbose() << "Connecting to connectionID " << connectionId << ". Driver comPortName: " << deviceName;

    if (conResult != 0) {
        ofLogVerbose() << "Error: Connection Failed! " << conResult;
        if(conResult == -2)            
            ofLogVerbose() << "Serial port could not be opened!";
        isReady = false;
        return isReady;
    }
    
    
    // enable blinking for connection
    int resBlink = TG_EnableBlinkDetection(connectionId, 1);
    if (resBlink != 0)
        ofLogVerbose() << "Blink: Failed to enable blink detection";
    
    isReady = true;
    return isReady;
}



void ThinkgearCommsDriver::update() {
    
    // need to handle connection/reconnection issues
    if(!isReady) {        
        return;
    }
    
    // read each packet individually- use the callback
    int packetsRead = 0;
    while(TG_ReadPackets(connectionId, 1) > 0) {
        packetsRead++;
        if(TG_GetValueStatus(connectionId, TG_DATA_RAW) != 0) {
            float raw = TG_GetValue(connectionId, TG_DATA_RAW);
            callback(TG_DATA_RAW,raw);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_POOR_SIGNAL) != 0) {
            float signal = TG_GetValue(connectionId, TG_DATA_POOR_SIGNAL);
            callback(TG_DATA_POOR_SIGNAL,signal);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_BATTERY) != 0) {
            float battery = TG_GetValue(connectionId, TG_DATA_BATTERY);
            callback(TG_DATA_BATTERY,battery);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_ATTENTION) != 0) {
            float attention = TG_GetValue(connectionId, TG_DATA_ATTENTION);
            callback(TG_DATA_ATTENTION,attention);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_MEDITATION) != 0) {
            float meditation = TG_GetValue(connectionId, TG_DATA_MEDITATION);
            callback(TG_DATA_MEDITATION,meditation);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_DELTA) != 0) {
            float delta = TG_GetValue(connectionId, TG_DATA_DELTA);
            callback(TG_DATA_DELTA,delta);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_THETA) != 0) {
            float theta = TG_GetValue(connectionId, TG_DATA_THETA);
            callback(TG_DATA_THETA,theta);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_ALPHA1) != 0) {
            float alpha1 = TG_GetValue(connectionId, TG_DATA_ALPHA1);
            callback(TG_DATA_ALPHA1,alpha1);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_ALPHA2) != 0) {
            float alpha2 = TG_GetValue(connectionId, TG_DATA_ALPHA2);
            callback(TG_DATA_ALPHA2,alpha2);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_BETA1) != 0) {
            float beta1 = TG_GetValue(connectionId, TG_DATA_BETA1);
            callback(TG_DATA_BETA1,beta1);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_BETA2) != 0) {
            float beta2 = TG_GetValue(connectionId, TG_DATA_BETA2);
            callback(TG_DATA_BETA2,beta2);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_GAMMA1) != 0) {
            float gamma1 = TG_GetValue(connectionId, TG_DATA_GAMMA1);
            callback(TG_DATA_GAMMA1,gamma1);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_GAMMA2) != 0) {
            float gamma2 = TG_GetValue(connectionId, TG_DATA_GAMMA2);
            callback(TG_DATA_GAMMA2,gamma2);
        }
        if(TG_GetValueStatus(connectionId, TG_DATA_BLINK_STRENGTH) != 0) {
            float blinkStrength = TG_GetValue(connectionId, TG_DATA_BLINK_STRENGTH);
            callback(TG_DATA_BLINK_STRENGTH,blinkStrength);
        }

    }

    // no data received.... reconnect
    if(packetsRead == 0) {
        unavailableCount++;
        if(unavailableCount >  noDataRestartCount) {
            ofLogVerbose() << "*** no data available - attempt to reconnect";
            unavailableCount = 0;
            isReady = false;
            
            //connect();
        }
    } else {
        unavailableCount = 0;
    }
}