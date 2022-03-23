package Glyph;

import Window.Window;

/**
 * Defines a rectangle glyph
 */
public class Rectangle extends Glyph {
	private int _width;
	private int _height;
	public Rectangle(int width, int height){
		super();
		_width = width;
		_height = height;
	}
	@Override
	public void draw(Window window) {
		// Gets bounds and calls window method to draw rectangle.
		Bounds bounds = getBounds();
		Point boundsPoint = bounds.getPoint();
		window.drawRectangle(boundsPoint.getX(), boundsPoint.getY(), bounds.getWidth(), bounds.getHeight());
	}

	@Override
	public void setSizeChild(Window window) {
		// Size should be whatever height and width was passed in.
		getBounds().setHeight(_height);
		getBounds().setWidth(_width);
	}
}
