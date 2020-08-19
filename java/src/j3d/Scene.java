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

    SceneNode getRootNode() {
        return new SceneNode(getRootNodeHandle());
    }

    SceneNode getNode(String name) {
        return new SceneNode(getNodeHandle(name));
    }

    SceneNode getNode(int index) {
        return new SceneNode(getNodeHandle(index));
    }

    private native long getRootNodeHandle();
    private native long getNodeHandle(String name);
    private native long getNodeHandle(int index);
}
