package j3d;

public final class Graphics  {

	@SuppressWarnings("unused")	// used natively
	public long nativeHandle;

	Graphics(long handle) {
		nativeHandle = handle;
	}

	public native void render();

	public native void setScene(Scene scene);
}
