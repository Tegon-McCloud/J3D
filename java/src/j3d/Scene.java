package j3d;

import java.io.File;

public class Scene {
    
    @SuppressWarnings("unused")	// used natively
    private long nativeHandle;

    public Scene(Graphics gfx, File file) {
        nativeHandle = init(gfx, file.getAbsolutePath());
    }

    private native long init(Graphics gfx, String file);
    private native void close();

}
