// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include <windows.h>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:


const int MAX_DEVICES = 2;

#ifndef MYDEVICE_H
#define MYDEVICE_H
struct MyoDevice
{
public:
	int m_pose;
    float m_yaw;
    float m_pitch;
    float m_roll;
	float m_qx;
	float m_qy;
	float m_qz;
	float m_qw;
	float m_accelx;
	float m_accely;
	float m_accelz;
	float m_fv[8];
};
MyoDevice m_devices[MAX_DEVICES];
int m_deviceCount;


#endif

#define FV_NUM 8
#include <myo_raw.hpp>
extern MyoDevice m_devices[MAX_DEVICES];
extern int m_deviceCount;
static std::vector<myo::Myo*> knownMyos;

//static int deviceNum = 0;


// The raw SDK still supports myo::DeviceListener to receive non-raw data such as orientation and poses.
class DataCollector : public myo::DeviceListener {
public:
    DataCollector()
    : roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    {
    }

	void onPair(myo::Myo* myo, uint64_t timestamp);
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
 
    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
    // making a fist, or not making a fist anymore.
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);
   
    // There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
    // For this example, the functions overridden above are sufficient.
	void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel) ;
	
    // We define this function to print the current values that were updated by the on...() functions above.
    void print();

	size_t identifyMyo(myo::Myo* myo) ;

    // These values are set by onOrientationData() and onPose() above.
    int roll_w, pitch_w, yaw_w;
    myo::Pose currentPose;
	int deviceNum;

};

// The raw SDK adds a myo_raw::RawListener type that can be used to be notified of raw events.
// In this case, we'll store the latest average of the feature vector values we received, so
// that we can visualize them.
class FvCollector : public myo_raw::RawListener {
public:
	FvCollector()
		: fv_average(0)
	{
		memset(fvs, '0', sizeof(fvs));
	}

	void onPair(myo::Myo* myo, uint64_t timestamp);
	// onFeatureVectorData is called whenever a new feature vector sample is available.
	// The fv array passed to this function contains 8 values, one for each sensor on Myo. 
	void onFeatureVectorData(myo::Myo* myo, uint64_t timestamp, const uint16_t* fv);
	
	// We add a print routine to show the average feature vector as a bar graph.
	void print() const ;
	size_t identifyMyo(myo::Myo* myo) ;

	uint16_t fv_average;
	uint16_t fvs[FV_NUM];
	int deviceNum;

};




// TODO: reference additional headers your program requires here

#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

extern "C"
{
	EXPORT_API void Initialize();

	EXPORT_API void Uninitialize();

	EXPORT_API bool GetYaw(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetPitch(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetRoll(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetQX(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetQY(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetQZ(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetQW(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetAccelX(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetAccelY(float *val0, float *val1, int deviceId);

	EXPORT_API bool GetAccelZ(float *val0, float *val1, int deviceId);

	EXPORT_API int GetDeviceCount();

	EXPORT_API bool GetPose(int *val0, int *val1, int deviceId);

	EXPORT_API bool GetFv_Myo0(float *fv0, float *fv1, float *fv2, float *fv3, float *fv4, float *fv5, float *fv6,float *fv7);

	EXPORT_API bool GetFv_Myo1(float *fv0, float *fv1, float *fv2, float *fv3, float *fv4, float *fv5, float *fv6,float *fv7);
	//EXPORT_API void Vibrate(int amount, int deviceId);
}
