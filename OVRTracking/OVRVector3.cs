using System.Runtime.InteropServices;

// ReSharper disable InconsistentNaming

namespace OVRTracking
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OVRVector3
    {
        public float x, y, z;
    }
}