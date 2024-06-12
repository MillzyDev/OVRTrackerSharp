using System.Runtime.InteropServices;

namespace OVRTrackingSharp 
{
    public sealed class OVRTracking 
    {
        private delegate VRError InitTrackingDelegate();

        private delegate IntPtr GetTrackerIndicesDelegate([MarshalAs(UnmanagedType.U8)] ref int size);

        private delegate OVRTransform GetPoseForTrackerDelegate([MarshalAs(UnmanagedType.U4)] int index);
        
        private readonly InitTrackingDelegate _initTracking;
        private readonly GetTrackerIndicesDelegate _getTrackerIndices;
        private readonly GetPoseForTrackerDelegate _getPoseForTracker;

        public OVRTracking(string? nativeLibPath = null)
        {
            nativeLibPath ??= "OVRTrackingNative.dll";

            if (!NativeLibrary.TryLoad(nativeLibPath, out IntPtr nativeHandle))
            {
                throw new ArgumentException("Invalid OVRTrackingNative supplied.");
            }

            if (!NativeLibrary.TryGetExport(nativeHandle, "initialiseTracking", out IntPtr nativeInitTrackingHandle))
            {
                throw new ArgumentException("initialiseTracking function not found in supplied OVRTrackingNative.");
            }

            if (!NativeLibrary.TryGetExport(nativeHandle, "getTrackerIndices", out IntPtr nativeGetTrackerIndicesHandle))
            {
                throw new ArgumentException("getTrackerIndices function not found in supplied OVRTrackingNative.");
            }

            if(!NativeLibrary.TryGetExport(nativeHandle, "getPoseForTracker", out IntPtr nativeGetPoseForTrackerHandle))
            {
                throw new ArgumentException("getPoseForTracker function not found in supplied OVRTrackingNative.");
            }

            _initTracking =
                Marshal.GetDelegateForFunctionPointer<InitTrackingDelegate>(nativeInitTrackingHandle);
            _getTrackerIndices =
                Marshal.GetDelegateForFunctionPointer<GetTrackerIndicesDelegate>(
                    nativeGetTrackerIndicesHandle);
            _getPoseForTracker =
                Marshal.GetDelegateForFunctionPointer<GetPoseForTrackerDelegate>(nativeGetPoseForTrackerHandle);
        }
        
        public VRError InitTracking()
        {
            return _initTracking.Invoke();
        }

        public int[] GetGenericTrackerIndices()
        {
            int size = 0;
            IntPtr ptr = _getTrackerIndices(ref size);

            if (size == 0 || ptr == IntPtr.Zero)
            {
                return Array.Empty<int>();
            }

            int[] values = new int[size];
            Marshal.Copy(ptr, values, 0, size);
            return values;
        }

        public OVRTransform GetPoseForTracker(int index)
        {
            return _getPoseForTracker.Invoke(index);
        }
    }
    
}