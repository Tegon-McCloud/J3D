package j3d;


public class SceneNode {

    @SuppressWarnings("unused") // used by native code
    private long nativeHandle;

    public SceneNode(long handle){
        nativeHandle = handle;
    }

    public native void rotateX(float angle);
    public native void rotateY(float angle);
    public native void rotateZ(float angle);

    public native void translate(float x, float y, float z);
    public native void scale(float x, float y, float z);

    
}