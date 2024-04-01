using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
// ReSharper disable UnusedMember.Global
// ReSharper disable UnusedType.Global
// ReSharper disable InconsistentNaming

namespace OVRTracking
{
    public class OVRTracking
    {
        private delegate VRError InitTrackingDelegate();

        private delegate IntPtr GetTrackerIndicesDelegate([MarshalAs(UnmanagedType.U8)] ref int size);

        private delegate OVRTransform GetPoseForTrackerDelegate([MarshalAs(UnmanagedType.U4)] int index);

        [return: MarshalAs(UnmanagedType.LPStr)]
        private delegate string GetTrackerSerialDelegate([MarshalAs(UnmanagedType.U4)] int index);

        private readonly InitTrackingDelegate _initTracking;
        private readonly GetTrackerIndicesDelegate _getTrackerIndices;
        private readonly GetPoseForTrackerDelegate _getPoseForTracker;
        private readonly GetTrackerSerialDelegate _getTrackerSerial;

        public OVRTracking(string? nativeLibPath = null)
        {
            nativeLibPath ??= "ovr_tracker_native.dll";

            IntPtr nativeHandle = LoadLibrary(nativeLibPath);
            if (nativeHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeInitTrackingHandle = GetProcAddress(nativeHandle, "init_tracking");
            if (nativeInitTrackingHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeGetTrackerIndicesHandle = GetProcAddress(nativeHandle, "get_tracker_indices");
            if (nativeGetTrackerIndicesHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeGetPoseForTrackerHandle = GetProcAddress(nativeHandle, "get_pose_for_tracker");
            if (nativeGetPoseForTrackerHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeGetTrackerSerialHandle = GetProcAddress(nativeHandle, "get_tracker_serial");
            if (nativeGetTrackerSerialHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            _initTracking =
                Marshal.GetDelegateForFunctionPointer<InitTrackingDelegate>(nativeInitTrackingHandle);
            _getTrackerIndices =
                Marshal.GetDelegateForFunctionPointer<GetTrackerIndicesDelegate>(
                    nativeGetTrackerIndicesHandle);
            _getPoseForTracker =
                Marshal.GetDelegateForFunctionPointer<GetPoseForTrackerDelegate>(nativeGetPoseForTrackerHandle);
            _getTrackerSerial =
                Marshal.GetDelegateForFunctionPointer<GetTrackerSerialDelegate>(nativeGetTrackerSerialHandle);
        }

        public VRError InitTracking()
        {
            return _initTracking.Invoke();
        }

        public int[] GetGenericTrackerIndices()
        {
            int size = 0;
            IntPtr ptr = _getTrackerIndices(ref size);

            if (size == 0 || ptr == IntPtr.Zero) return new int[] { };

            int[] values = new int[size];
            Marshal.Copy(ptr, values, 0, size);
            return values;
        }

        public OVRTransform GetPoseForTracker(int index)
        {
            return _getPoseForTracker.Invoke(index);
        }
        
        public string GetTrackerSerial(int index)
        {
            return _getTrackerSerial.Invoke(index);
        }

        private static void HandleError(int error)
        {
            if (error == 0) return;

            throw new Win32Exception(error);
        }

        [DllImport("kernel32.dll")]
        private static extern IntPtr LoadLibrary(string lpLibFileName);

        [DllImport("kernel32.dll")]
        private static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);
    }
}