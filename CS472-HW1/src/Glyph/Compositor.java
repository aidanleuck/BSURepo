package Glyph;

import Window.Window;

/**
 * Describes interface for a compositor
 */
public abstract class Compositor {
	protected Composition _composition;

	/**
	 * Modifies/sets the composition that the compositor is keeping track of.
	 * @param composition - A composition for the compositor to reference.
	 */
	public void setComposition(Composition composition) {
		_composition = composition;
	}

	/**
	 * Composes a composition to format the document in a particular way.
	 * @param window - Swing window interface.
	 */
	public abstract void compose(Window window);
}
