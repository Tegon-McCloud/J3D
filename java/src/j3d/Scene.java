package j3d;

import java.io.File;

public class Scene {
    
    @SuppressWarnings("unused")	// used natively
    private long nativeHandle;

    public Scene(long handle) {
        nativeHandle = handle;
    }

    public void load(File f) {
        load(f.getAbsoluteFile());
    }

    public native void load(String file);

}
