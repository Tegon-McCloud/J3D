package j3d;

public final class KeyEvent {
	private byte key;
	private boolean press;

	private KeyEvent(byte key, boolean press) {
		this.key = key;
		this.press = press;
	}

	public byte getKey() {
		return key;
	}

	public boolean isPress() {
		return press;
	}

	public boolean isRelease() {
		return !press;
	}
}
