package j3d;

public final class MouseEvent {

	private MouseEventType type;
	private int x, y;
	private boolean ldown, rdown;

	private MouseEvent(MouseEventType type, int x, int y, boolean ldown, boolean rdown) {
		this.type = type;
		this.x = x;
		this.y = y;
		this.ldown = ldown;
		this.rdown = rdown;
	}

	MouseEventType getType() {
		return type;
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	boolean leftIsDown() {
		return ldown;
	}

	boolean rightIsDown() {
		return rdown;
	}

	
}

