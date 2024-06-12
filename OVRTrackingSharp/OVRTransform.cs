using System.Runtime.InteropServices;

namespace OVRTrackingSharp 
{
    [StructLayout(LayoutKind.Sequential)]
    public struct OVRTransform
    {
        public OVRVector3 position;
        public OVRQuaternion rotation;
    }
}