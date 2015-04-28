using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace MyoPlugin
{
    public class MyoSdk : MonoBehaviour
    {
        [DllImport("MyoPlugin")]
        public static extern void Initialize();

        [DllImport("MyoPlugin")]
        public static extern void Uninitialize();

        [DllImport("MyoPlugin")]
        public static extern int GetDeviceCount();

        [DllImport("MyoPlugin")]
        public static extern bool GetYaw(out float val0, out float val1, int deviceId);

        [DllImport("MyoPlugin")]
		public static extern bool GetPitch(out float val0, out float val1, int deviceId);

        [DllImport("MyoPlugin")]
		public static extern bool GetQX(out float val0, out float val1, int deviceId);

        [DllImport("MyoPlugin")]
		public static extern bool GetQY(out float val0, out float val1, int deviceId);

        [DllImport("MyoPlugin")]
		public static extern bool GetQZ(out float val0, out float val1, int deviceId);

        [DllImport("MyoPlugin")]
		public static extern bool GetQW(out float val0, out float val1, int deviceId);

        [DllImport("MyoPlugin")]
		public static extern bool GetRoll(out float val0, out float val1, int deviceId);

        [DllImport("MyoPlugin")]
		public static extern bool GetPose(out int val0, out int val1, int deviceId);
        
        [DllImport("MyoPlugin")]
		public static extern bool GetAccelX(out float val0, out float val1,int deviceId);
        
        [DllImport("MyoPlugin")]
		public static extern bool GetAccelY(out float val0, out float val1, int deviceId);
        
        [DllImport("MyoPlugin")]
		public static extern bool GetAccelZ(out float val0, out float val1, int deviceId);

		[DllImport("MyoPlugin")]
		public static extern bool GetFv_Myo0(out float fv0, out float fv1, out float fv2, out float fv3,out float fv4, out float fv5,out float fv6, out float fv7);

		[DllImport("MyoPlugin")]
		public static extern bool GetFv_Myo1(out float fv0, out float fv1, out float fv2, out float fv3,out float fv4, out float fv5,out float fv6, out float fv7);

//        [DllImport("MyoPlugin")]
//		public static extern void Vibrate(int vibration, int deviceId);

        public enum Poses
        {
            None,
            Fist,
            WaveIn,
            WaveOut,
            FingersSpread,
            TwistIn,

        };

        public enum VibrationType
        {
            VibrationShort = 0,
            VibrationMedium,
            VibrationLong
        };

    }
}