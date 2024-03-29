using System.Runtime.InteropServices;

// ReSharper disable InconsistentNaming

namespace OVRTracking
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OVRTransform
    {
        public OVRVector3 position;
        public OVRQuaternion rotation;
    }
}