package Glyph;

import Window.Window;

/**
 * Defines a column a composition that has children laid out vertically
 */
public class Column extends Composition {
	public Column(){
		super();
	}

	@Override
	public void adjustCursor(Point cursor, Glyph child){

		// Adjusts the cursor in the y direction based of height of child.
		int cursorY = cursor.getY();
		int height = child.getBounds().getHeight();
		cursorY += height;

		cursor.setY(cursorY);
	}

	@Override
	public void setSize(Window window){
		int width = 0;
		int height = 0;

		// Sets the width to be the max width of its child
		// Height is the sum of its children's height
		for(Glyph child: childList){
			width = Math.max(width, child.getBounds().getWidth());
			height += child.getBounds().getHeight();
		}

		// Sets width, height of the bounds
		getBounds().setDims(width, height);
	}

}
