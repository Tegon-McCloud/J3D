package j3d;


public class Mouse {

	@SuppressWarnings("unused")
	private long nativeHandle;

	Mouse(long handle) {
		nativeHandle = handle;
	}

	public native MouseEvent nextEvent();
	
	public native void flushEvents();
}
