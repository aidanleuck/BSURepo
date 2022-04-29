package Glyph;

import Iterator.Iterator;
import Window.Window;

/**
 * Defines a type of compositor that implements a simple compositor algorithm
 * Redraws entire window every time a new item is added
 */
public class SimpleCompositor extends Compositor {
	@Override
	public void compose(Window window){
		Point cursor = _composition.createCursor();

		// Goes through each child glyph
		Iterator<Glyph> listIterator = _composition.createIterator();
		for(listIterator.first(); !listIterator.isDone(); listIterator.next()){
			Glyph child = listIterator.currentItem();
			// Asks the child to set size based on window
			child.setSizeChild(window);

			// Adjust position of cursor based on child
			child.setPosition(cursor);

			// Recursively composes child
			child.composeGlyph(window);

			// Asks the composition to adjust cursor based off child
			_composition.adjustCursor(cursor, child);
		}
		// Asks the composition to update its size based off its children
		_composition.setSize(window);
	}
}