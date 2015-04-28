
// stdafx.cpp : source file that includes just the standard includes
// *.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information



/// START OF MYO THALLMIC CODE

// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include "stdafx.h"
// The only file that needs to be included to use the Myo C++ SDK is myo.hpp.
//#include <myo.hpp>
// Instead of including myo.hpp, include myo_raw.hpp when using the raw SDK
#include <myo_raw.hpp>



// Classes that inherit from myo::DeviceListener can be used to receive events from Myo devices. DeviceListener
// provides several virtual functions for handling different kinds of events. If you do not override an event, the
// default behavior is to do nothing.


//myo_raw::RawHub* hub = NULL;
//myo::Myo* myMYO_Object = NULL;
//FvCollector *fvCollector = NULL;
//OrientationCollector *orientationCollector = NULL;
//HANDLE threadHandle = NULL;
//DWORD threadID = 0;
//bool bContinueThread = true;


	void DataCollector :: onPair(myo::Myo* myo, uint64_t timestamp)
    {
        // Print out the MAC address of the armband we paired with.
        // The pointer address we get for a Myo is unique - in other words, it's safe to compare two Myo pointers to
        // see if they're referring to the same Myo.
        // Add the Myo pointer to our list of known Myo devices. This list is used to implement identifyMyo() below so
        // that we can give each Myo a nice short identifier.
        knownMyos.push_back(myo);

        // Now that we've added it to our list, get our short ID for it and print it out.
        std::cout << "Paired with " << myo->macAddressAsString() << ", now known as Myo " << identifyMyo(myo)
                  << "." << std::endl;

        // Check whether this Myo device has been trained. It will only provide pose information if it's been trained.
        if (myo->isTrained()) {
            std::cout << "Myo " << identifyMyo(myo) << " is trained and will provide pose events." << std::endl;
        } else {
            std::cout << "Myo " << identifyMyo(myo) << " is not trained and will not provide any pose events."
                      << std::endl;
        }
    }

    void DataCollector :: onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
		using std::atan2;
        using std::asin;
        using std::sqrt;

        // Calculate the normalized quaternion.
        float norm = sqrtf(quat.x() * quat.x() + quat.y() * quat.y() + quat.z() * quat.z() + quat.w() * quat.w());
        myo::Quaternion<float> normalized(quat.x()/norm, quat.y()/norm, quat.z()/norm, quat.w()/norm);

        // Calculate Euler angles (roll, pitch, and yaw) from the normalized quaternion.
        float roll = atan2(2.0f * (normalized.w() * normalized.x() + normalized.y() * normalized.z()),
                           1.0f - 2.0f * (normalized.x() * normalized.x() + normalized.y() * normalized.y()));
        float pitch = asin(2.0f * (normalized.w() * normalized.y() - normalized.z() * normalized.x()));
        float yaw = atan2(2.0f * (normalized.w() * normalized.z() + normalized.x() * normalized.y()),
                          1.0f - 2.0f * (normalized.y() * normalized.y() + normalized.z() * normalized.z()));

        // Convert the floating point angles in radians to a scale from 0 to 20.
        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);

		deviceNum = identifyMyo(myo);
		//std::cout << "deviceNum: " << deviceNum << std::endl;
			
		m_devices[deviceNum].m_roll = roll;		
		m_devices[deviceNum].m_pitch = pitch;
		m_devices[deviceNum].m_yaw = yaw;

		m_devices[deviceNum].m_qx = quat.x();
		m_devices[deviceNum].m_qy = quat.y();
		m_devices[deviceNum].m_qz = quat.z();
		m_devices[deviceNum].m_qw = quat.w();
		
  }
 
    void DataCollector :: onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;

		//m_devices[identifyMyo(myo)].m_pose = (int)pose.type();
        // Vibrate the Myo whenever we've detected that the user has made a fist.
		
        if (pose == myo::Pose::fist) {
            myo->vibrate(myo::Myo::VibrationMedium);
        }
	
    }

	void DataCollector :: onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel) 
	{
		/*m_devices[identifyMyo(myo)].m_accelx = accel.x();
		m_devices[identifyMyo(myo)].m_accely = accel.y();
		m_devices[identifyMyo(myo)].m_accelz = accel.z();
		*/
	}

    void DataCollector :: print()
    {
        // Clear the current line
        std::cout << '\r';

        // Pose::toString() provides the human-readable name of a pose. We can also output a Pose directly to an
        // output stream (e.g. std::cout << currentPose;). In this case we want to get the pose name's length so
        // that we can fill the rest of the field with spaces below, so we obtain it as a string using toString().
        std::string poseString = currentPose.toString();

        // Output the current values
        std::cout << '[' << std::string(roll_w, '*') << std::string(18 - roll_w, ' ') << ']'
                  << '[' << std::string(pitch_w, '*') << std::string(18 - pitch_w, ' ') << ']'
                  << '[' << std::string(yaw_w, '*') << std::string(18 - yaw_w, ' ') << ']'
                  << '[' << poseString << std::string(16 - poseString.size(), ' ') << ']'
                  << std::flush;
    }

	size_t DataCollector :: identifyMyo(myo::Myo* myo) {
        // Walk through the list of Myo devices that we've seen pairing events for.
        for (size_t i = 0; i < knownMyos.size(); ++i) {
            // If two Myo pointers compare equal, they refer to the same Myo device.
            if (knownMyos[i] == myo) {
                return i;
            }
        }
        return 0;
    }





	void FvCollector :: onFeatureVectorData(myo::Myo* myo, uint64_t timestamp, const uint16_t* fv)
	{
		unsigned int sum = 0;
		deviceNum = identifyMyo(myo);
		for (unsigned int i = 0; i < FV_NUM; ++i) {
			sum += fv[i];
			fvs[i] = fv[i];

			m_devices[deviceNum].m_fv[i] = fv[i];	
		}
		fv_average = sum / 8;

	}

	void FvCollector :: print() const
	{
		// First we scale down the feature vector average.
		unsigned int fv_scaled = fv_average * 16 / 2200;
		// Then we print it out as some number of 'F' characters.
		std::cout << '[' << std::string(fv_scaled, 'F') << std::string(16 - fv_scaled, ' ') << ']';
	}

	size_t FvCollector :: identifyMyo(myo::Myo* myo) {
        // Walk through the list of Myo devices that we've seen pairing events for.
        for (size_t i = 0; i < knownMyos.size(); ++i) {
            // If two Myo pointers compare equal, they refer to the same Myo device.
            if (knownMyos[i] == myo) {
                return i;
            }
        }
        return 0;
    }

//int main(){
//
//
//	std::vector<myo::Myo*> m_myo;
//	 try {
//        const unsigned int pairingCount = 2;
//
//		myo_raw::RawHub hub;
//
//        std::cout << "Pairing with " << pairingCount << " Myo armbands." << std::endl;
//
//        // pairWithAnyMyos() asynchronously initiates the pairing process with the provided number of Myo devices.
//        // DeviceListeners attached to the hub will receive onPair() events once pairing has completed.
//        hub.pairWithAnyMyos(pairingCount);
//
//        // Instantiate the PrintMyoEvents class we defined above, and attach it as a listener to our Hub.
//        DataCollector printer;
//		FvCollector fvCollector;
//        hub.addListener(&printer);
//		hub.addListener(&fvCollector);
//
//        while (1) {
//            // Process events for 10 milliseconds at a time.
//            hub.runRaw(100);
//			
//			//printer.print();
//			//fvCollector.print();
//			//for(int i = 0; i < FV_NUM;i++)
//			//std::cout << "Myo " << 0 << "   m_devices:" << m_devices[0].m_roll  << std::endl;  //the orientation data is correct!
//			//std::cout << "Myo " << 1 << "   m_devices:" << m_devices[1].m_roll  << std::endl;
//			//{
//			std::cout << "Myo " << 0 << "   m_devices:" << m_devices[0].m_fv[0]  << std::endl; // cannot identify fv belong to which hand?!
//			std::cout << "Myo " << 1 << "   m_devices:" << m_devices[1].m_fv[1] << std::endl;
//			//			//std::cout << "Myo " << 0 << "GetRoll return " << GetRoll(tmp, 0) << ".  GetRoll val:  " << tmp[0] << std::endl;
//			//printer.print();
//        }
//    } catch (const std::exception & e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//        std::cerr << "Press enter to continue.";
//        std::cin.ignore();
//        return 1;
//    }
//
//}


void ClearMyoDevices()
{
	for (int deviceId = 0; deviceId < MAX_DEVICES; ++deviceId)
	{
		m_devices[deviceId].m_pitch = 0;
		m_devices[deviceId].m_pose = 0;
		m_devices[deviceId].m_roll = 0;
		m_devices[deviceId].m_yaw = 0;
		m_devices[deviceId].m_qx = 0;
		m_devices[deviceId].m_qy = 0;
		m_devices[deviceId].m_qz = 0;
		m_devices[deviceId].m_qw = 0;
		m_devices[deviceId].m_accelx = 0;
		m_devices[deviceId].m_accely = 0;
		m_devices[deviceId].m_accelz = 0;
		for(size_t i = 0; i< FV_NUM; i++)
		{
			m_devices[deviceId].m_fv[i] = 0;
		}
	}
	m_deviceCount = 0;
}


struct MyoThreadInfo
{
public:
	//spawn a thrad for MyO capture 
	bool m_keepWorking;
	DWORD m_threadId;
};

MyoThreadInfo m_info;
std::vector<myo::Myo*> m_myo;


DWORD MyoPluginWorker (LPVOID lpdwThreadParam) 
{

	while (m_info.m_keepWorking)
	{
		try
		{
			const unsigned int pairingCount = 2;
			// First, we create a Hub. The Hub provides access to one or more Myos.
			myo_raw::RawHub hub;
		    std::cout << "Pairing with " << pairingCount << " Myo armbands." << std::endl;
			 
			// Next, we try to find a Myo (any Myo) that's nearby and connect to it. waitForAnyMyo() takes a timeout
			// value in milliseconds. In this case we will try to find a Myo for 10 seconds, and if that fails, the function
			// will return a null pointer.
			hub.pairWithAnyMyos(pairingCount); //myo::Myo* myo = hub.waitForAnyMyo(10000);

			// Next we construct an instance of our DeviceListener, so that we can register it with the Hub.
			// Hub::addListener() takes the address of any object whose class inherits from DeviceListener, and will cause
			// Hub::run() to send events to all registered device listeners.
			DataCollector printer;
			FvCollector fvCollector;
			hub.addListener(&printer);
			hub.addListener(&fvCollector);

			// Finally we enter our main loop.
			while (m_info.m_keepWorking) {
				// In each iteration of our main loop, we run the Myo event loop for a set number of milliseconds.
				// In this case, we wish to update our display 20 times a second, so we run for 1000/20 milliseconds.
				hub.runRaw(10);
				// After processing events, we call the print() member function we defined above to print out the values we've
				// obtained from any events that have occurred.
	
			}
			Sleep(0);
		}
		catch (std::exception & e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
			std::cerr << "Press enter to continue.";
			std::cin.ignore();
			return 1;
		}
	}
	return 0;
}

/*extern "C"
{
	EXPORT_API void Initialize()
	{
		__try
		{
			ClearMyoDevices();
			m_info.m_keepWorking = true;
			m_info.m_threadId = -1;

			CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE) MyoPluginWorker,
				NULL,
				0,
				&(m_info.m_threadId));

			std::cout << "MyoPlugin Initialized" << std::endl;

			m_deviceCount = 1;
		}
		__except (true)
		{
		}
	}

	EXPORT_API void Uninitialize()
	{
		__try
		{
			m_info.m_keepWorking = false;

			ClearMyoDevices();

			std::cout << "MyoPlugin Uninitialized" << std::endl;
		}
		__except (true)
		{
		}
	}

	EXPORT_API int GetDeviceCount()
	{
		return m_deviceCount;
	}

	EXPORT_API bool GetYaw(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_yaw;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetPitch(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_pitch;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetRoll(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_roll;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}



	EXPORT_API bool GetQX(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_qx;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetQY(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_qy;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetQZ(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_qz;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetQW(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_qw;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetAccelX(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_accelx;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetAccelY(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_accely;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetAccelZ(float* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_accelz;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetPose(int* val, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				*val = m_devices[deviceId].m_pose;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API void Vibrate(int amount)
	{
		switch (amount)
		{
		case 0: 
			m_myo->vibrate(myo::Myo::VibrationShort);
			break;
		case 1:
			m_myo->vibrate(myo::Myo::VibrationMedium);
			break;
		case 2:
			m_myo->vibrate(myo::Myo::VibrationLong);

		}
	}

	
}
*/
extern "C"
{
	EXPORT_API void Initialize()
	{
		__try
		{
			ClearMyoDevices();
			m_info.m_keepWorking = true;
			m_info.m_threadId = -1;

			CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE) MyoPluginWorker,
				NULL,
				0,
				&(m_info.m_threadId));

			std::cout << "MyoPlugin Initialized" << std::endl;
			m_deviceCount = 2;
		}
		__except (true)
		{
		}
	}

	EXPORT_API void Uninitialize()
	{
		__try
		{
			m_info.m_keepWorking = false;
			ClearMyoDevices();
			std::cout << "MyoPlugin Uninitialized" << std::endl;
		}
		__except (true)
		{
		}
	}

	EXPORT_API int GetDeviceCount()
	{
		return m_deviceCount;
	}

	EXPORT_API bool GetYaw(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_yaw;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_yaw;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetPitch(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_pitch;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_pitch;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetRoll(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_roll;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_roll;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetQX(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_qx;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_qx;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetQY(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_qy;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_qy;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetQZ(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_qz;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_qz;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}

	EXPORT_API bool GetQW(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_qw;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_qw;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetAccelX(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_accelx;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_accelx;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetAccelY(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_accely;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_accely;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetAccelZ(float *val0, float *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_accelz;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_accelz;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetPose(int *val0, int *val1, int deviceId)
	{
		__try
		{
			if (deviceId >= 0 &&
				deviceId < m_deviceCount)
			{
				if(deviceId==0)
					*val0 = m_devices[deviceId].m_pose;
				if(deviceId==1)
					*val1 = m_devices[deviceId].m_pose;
				return true;
			}
			else
			{
				return false;
			}
		}
		__except (true)
		{
			return false;
		}
	}
	EXPORT_API bool GetFv_Myo0(float *fv0, float *fv1, float *fv2, float *fv3, float *fv4, float *fv5, float *fv6,float *fv7)
	{
		__try
		{
			*fv0 = m_devices[0].m_fv[0];
			*fv1 = m_devices[0].m_fv[1];
			*fv2 = m_devices[0].m_fv[2];
			*fv3 = m_devices[0].m_fv[3];
			*fv4 = m_devices[0].m_fv[4];
			*fv5 = m_devices[0].m_fv[5];
			*fv6 = m_devices[0].m_fv[6];
			*fv7 = m_devices[0].m_fv[7];
			return true;
		}
		__except (true)
		{
			return false;
		}

	}
	EXPORT_API bool GetFv_Myo1(float *fv0, float *fv1, float *fv2, float *fv3, float *fv4, float *fv5, float *fv6,float *fv7)
	{
		__try
		{
			*fv0 = m_devices[0].m_fv[0];
			*fv1 = m_devices[0].m_fv[1];
			*fv2 = m_devices[0].m_fv[2];
			*fv3 = m_devices[0].m_fv[3];
			*fv4 = m_devices[0].m_fv[4];
			*fv5 = m_devices[0].m_fv[5];
			*fv6 = m_devices[0].m_fv[6];
			*fv7 = m_devices[0].m_fv[7];
			return true;
		}
		__except (true)
		{
			return false;
		}
	}

	/*EXPORT_API void Vibrate(int amount, int deviceId)
	{
		switch (amount)
		{
		case 0: 
			known->vibrate(myo::Myo::VibrationShort);
			break;
		case 1:
			m_myo[deviceId]->vibrate(myo::Myo::VibrationMedium);
			break;
		case 2:
			m_myo[deviceId]->vibrate(myo::Myo::VibrationLong);

		}
	}*/

	
}