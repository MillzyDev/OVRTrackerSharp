using System.Runtime.InteropServices;

namespace OVRTrackingSharp 
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OVRVector3 
    {
        public float x;
        public float y;
        public float z;
    }
}