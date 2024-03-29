using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

// ReSharper disable InconsistentNaming

namespace OVRTracking
{
    public class OVRTracking
    {
        private delegate VRError InitTrackingDelegate();

        private delegate IntPtr GetGenericTrackerIndicesDelegate([MarshalAs(UnmanagedType.U8)] ref int size);

        private delegate OVRTransform GetPoseForTrackerDelegate([MarshalAs(UnmanagedType.U4)] int index);

        private delegate OVRTrackerRole GetRoleForTrackerDelegate([MarshalAs(UnmanagedType.U4)] int index);

        private readonly InitTrackingDelegate _initTracking;
        private readonly GetGenericTrackerIndicesDelegate _getGenericTrackerIndices;
        private readonly GetPoseForTrackerDelegate _getPoseForTracker;
        private readonly GetRoleForTrackerDelegate _getRoleForTracker;

        public OVRTracking(string? nativeLibPath = null)
        {
            nativeLibPath ??= "ovr_tracker_native.dll";

            IntPtr nativeHandle = LoadLibrary(nativeLibPath);
            if (nativeHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeInitTrackingHandle = GetProcAddress(nativeHandle, "init_tracking");
            if (nativeInitTrackingHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeGetGenericTrackerIndicesHandle = GetProcAddress(nativeHandle, "get_generic_tracker_indices");
            if (nativeGetGenericTrackerIndicesHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeGetPoseForTrackerHandle = GetProcAddress(nativeHandle, "get_pose_for_tracker");
            if (nativeGetPoseForTrackerHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            IntPtr nativeGetRoleForTrackerHandle = GetProcAddress(nativeHandle, "get_role_for_tracker");
            if (nativeGetRoleForTrackerHandle == IntPtr.Zero) HandleError(Marshal.GetLastWin32Error());

            _initTracking =
                Marshal.GetDelegateForFunctionPointer<InitTrackingDelegate>(nativeInitTrackingHandle);
            _getGenericTrackerIndices =
                Marshal.GetDelegateForFunctionPointer<GetGenericTrackerIndicesDelegate>(
                    nativeGetGenericTrackerIndicesHandle);
            _getPoseForTracker =
                Marshal.GetDelegateForFunctionPointer<GetPoseForTrackerDelegate>(nativeGetPoseForTrackerHandle);
            _getRoleForTracker =
                Marshal.GetDelegateForFunctionPointer<GetRoleForTrackerDelegate>(nativeGetRoleForTrackerHandle);
        }

        public VRError InitTracking()
        {
            return _initTracking.Invoke();
        }

        public int[] GetGenericTrackerIndices()
        {
            int size = 0;
            IntPtr ptr = _getGenericTrackerIndices(ref size);

            if (size == 0) return new int[] { };

            int[] values = new int[size];
            Marshal.Copy(ptr, values, 0, size);
            return values;
        }

        public OVRTransform GetPoseForTracker(int index)
        {
            return _getPoseForTracker.Invoke(index);
        }

        public OVRTrackerRole GetRoleForTracker(int index)
        {
            return _getRoleForTracker.Invoke(index);
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