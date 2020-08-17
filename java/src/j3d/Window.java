package j3d;

import java.io.File;

public class Window {

	static {
		System.loadLibrary("J3D");
	}

	@SuppressWarnings("unused")	// used natively
	private long nativeHandle;

	private Keyboard keyboard;
	private Mouse mouse;
	private Graphics graphics;

	public Window(int width, int height, String title) {
		nativeHandle = init(width, height, title);
		mouse = new Mouse(getMouseHandle());
		keyboard = new Keyboard(getKeyboardHandle());
		graphics = new Graphics(getGraphicsHandle());
	}

	private native long init(int width, int height, String title);
	private native long getMouseHandle();
	private native long getKeyboardHandle();
	private native long getGraphicsHandle();

	public native void close();

	public Keyboard getKeyboard() {
		return keyboard;
	}

	public Mouse getMouse() {
		return mouse;
	}

	public Graphics getGraphics() {
		return graphics;
	}

	public static native boolean poll();

	public static void test() {
		
		Window wnd;
		
		wnd = new Window(1080, 720, "test window");

		Keyboard kbd = wnd.getKeyboard();
		Mouse mouse = wnd.getMouse();
		Graphics gfx = wnd.getGraphics();

		Scene scene = new Scene(gfx, new File("models/barrel/barrel.gltf"));
		gfx.setScene(scene);

		KeyEvent keyEvent;
		MouseEvent mouseEvent;

		while(!poll()) {

			while((keyEvent = kbd.nextEvent()) != null) {
				
			}

			while((mouseEvent = mouse.nextEvent()) != null) {
				
			}

			gfx.render();

		}

		wnd.close();
	}
	public static void main(String[] args) {
		test();
	}
}