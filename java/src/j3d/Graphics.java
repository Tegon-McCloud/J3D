package j3d;

public final class Graphics  {

	@SuppressWarnings("unused")	// used natively
	public long nativeHandle;

    Scene scene;

	Graphics(long handle) {
		nativeHandle = handle;

		scene = new Scene(getSceneHandle());
	}

	private native long getSceneHandle();

	public native void render();

	Scene getScene() {
		return scene;
	}
}
