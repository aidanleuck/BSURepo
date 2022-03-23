package Glyph;

/**
 * Keeps track of x,y coordinates of a glyph.
 */
public class Point {
	private int x;
	private int y;

	public Point(){
		x = 0;
		y = 0;
	}

	/**
	 * Gets x coordinate
	 * @return x coordinate
	 */
	public int getX(){
		return x;
	}

	/**
	 * Gets y coordinate
	 * @return - y coordinate
	 */
	public int getY(){
		return y;
	}

	/**
	 * Sets x coordiante
	 * @param x - value to set x coordinate to
	 */
	public void setX(int x){
		this.x = x;
	}

	/**
	 * Sets y coordinate
	 * @param y - value to set y coordinate to.
	 */
	public void setY(int y){
		this.y = y;
	}

	public void setCoords(int x, int y){
		this.x = x;
		this.y = y;
	}

}
