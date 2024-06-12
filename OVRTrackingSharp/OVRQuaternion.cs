using System.Runtime.InteropServices;

namespace OVRTrackingSharp 
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OVRQuaternion 
    {
        public float x;
        public float y;
        public float z;
        public float w;
    }
}