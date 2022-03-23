package Glyph;

/**
 * Holds positioning information about a glyph, height and width, x and y.
 */
public class Bounds {
	private Point point;
	private int width;
	private int height;

	public Bounds(){
		point = new Point();
		width = 0;
		height = 0;
	}

	/**
	 * Returns the internal point object
	 * @return - The bounds point
	 */
	public Point getPoint(){
		return point;
	}

	/**
	 * Returns the width of the bounds
	 * @return - width of the bound.
	 */
	public int getWidth(){
		return width;
	}

	/**
	 * Returns the height of the bounds.
	 * @return - Height of bounds
	 */
	public int getHeight(){
		return height;
	}

	/**
	 * Sets the width of the bound
	 * @param width - The width to set the bounds to
	 */
	public void setWidth(int width){
		this.width = width;
	}

	/**
	 * Sets the height of the bound.
	 * @param height - The height to set the bounds to
	 */
	public void setHeight(int height){
		this.height = height;
	}

	/**
	 * Sets both the height and width of the bounds
	 * @param width - The width to set the bounds to.
	 * @param height - The height to set the bounds to.
	 */
	public void setDims(int width, int height){
		this.height = height;
		this.width = width;
	}
}
