package Glyph;

import Iterator.Iterator;
import Visitor.GlyphVisitor;
import Window.Window;

/**
 * Defines a glyph that lays out children horizontally.
 * Concrete Element in Visitor (331)
 */
public class Row extends Composition {
	public Row(){
		super();
	}

	@Override
	public void adjustCursor(Point cursor, Glyph child){
		// Adjusts cursor in the x direction since rows layout in x direction
		int cursorX = cursor.getX();
		int width = child.getBounds().getWidth();
		cursorX += width;

		cursor.setX(cursorX);
	}

	@Override
	public void setSize(Window window){

		int width = 0;
		int height = 0;

		// Width/height of a row is the sum of all of children's width
		// Since rows are laid out in x direction height is the max height of children.
		Iterator<Glyph> listIterator = createIterator();
		for(listIterator.first(); !listIterator.isDone(); listIterator.next()){
			Glyph child = listIterator.currentItem();
			width += child.getBounds().getWidth();
			height = Math.max(height, child.getBounds().getHeight());
		}

		// Set the dimensions of the row
		getBounds().setDims(width, height);
	}

	@Override
	public void accept(GlyphVisitor v) {
		v.visit(this);
	}
}
