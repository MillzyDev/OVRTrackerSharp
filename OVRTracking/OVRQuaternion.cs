using System.Runtime.InteropServices;

// ReSharper disable InconsistentNaming

namespace OVRTracking
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OVRQuaternion
    {
        public float x, y, z, w;
    }
}