package j3d;

public class Keyboard {

	@SuppressWarnings("unused")
	private long nativeHandle;

	Keyboard(long handle) {
		nativeHandle = handle;
	}
	
	public native boolean isKeyDown(byte key);

	public native KeyEvent nextEvent();

	public native void flushEvents();
}

