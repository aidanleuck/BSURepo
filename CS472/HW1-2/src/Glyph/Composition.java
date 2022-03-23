package Glyph;

import Window.Window;

/**
 * A glyph that requires formatting
 */
public abstract class Composition extends CompositeGlyph {
	protected Compositor compositor;

	public Composition() {
		super();

        // Uses a default compositor implementation (can still be modified at runtime)
        compositor = new SimpleCompositor();
        compositor.setComposition(this);
	}

	/**
	 * Sets the compositor reference of this composition
	 * @param comp - The compositor to set
	 */
	public void setCompositor(Compositor comp) {
		compositor = comp;
		compositor.setComposition(this);
	}

	@Override
	public void insert(Window window, Glyph glyph, int position) {
		super.insert(window, glyph, position);

		// Compositions should format after inserting
		Glyph root = findRoot();
		root.composeGlyph(window);
	}

    @Override
    public void insert(Window window, Glyph glyph){
        super.insert(window, glyph);

        // Compositions should format after inserting
        Glyph root = findRoot();
        root.composeGlyph(window);

    }

	/**
	 * Adjusts the cursor in the x,y direction depending on the formatting of a composition
	 *
	 * @param cursor - The cursor to update
	 * @param child  - The child thats bounds will update the cursor
	 */
	public abstract void adjustCursor(Point cursor, Glyph child);

	@Override
	public void composeGlyph(Window window) {
		// Composes this glyph based on its compositor.
		compositor.compose(window);
	}

	/**
	 * Sets the initial state of the cursor based on the composition type.
	 */
	public Point createCursor(){
		Point cursor = new Point();
		Point currentPoint = getBounds().getPoint();
		cursor.setCoords(currentPoint.getX(), currentPoint.getY());
		return cursor;
	}

	/**
	 * Sets the dimensions (width, height) of a glyph.
	 * @param window - The interface to interact with the swing window.
	 */
	public abstract void setSize(Window window);


	@Override
	public void remove(Window window, Glyph glyph) {

		// Calls parent's remove method
		super.remove(window, glyph);

		// Finds the root and starts composing from it.
		Glyph root = findRoot();
		root.composeGlyph(window);
	}
}