package Glyph;
import Command.Command;
import Window.Window;

public abstract class Glyph {
    private Bounds bounds;
    private Glyph parent;

    /**
     * Tells a glyph how it should draw itself
     * @param window - The interface to interact with the swing window.
     */
    public abstract void draw(Window window);

    public Glyph(){
        bounds = new Bounds();
    }

    /**
     * Returns the bounds object
     * @returns - The bounds object
     */
    public Bounds getBounds() {return bounds;}

    /**
     * Composes a glyph by calling its compositor (by default does nothing)
     * @param window - The interface to interact with the swing window
     */
    public void composeGlyph(Window window){};

    /**
     * Finds a glyph that was clicked
     * @param x - x coordinate of the click
     * @param y - y coordinate of the click.
     * @return Glyph that was clicked
     */
    protected Glyph find(int x, int y){
        Point point = new Point();
        point.setCoords(x, y);
        Glyph glyph = null;

        if(getBounds().intersects(point)){
            glyph = this;
        }
        return glyph;
    }

    /**
     * Gets the command of a glyph that was clicked
     * @param x - x coordinate of the click
     * @param y - y coordinate of the click
     * @return the command of the glyph that was clicked
     */
    public Command click(int x, int y){
       Glyph foundGlyph = find(x, y);
       Command command = null;
       if(foundGlyph != null){
           command = foundGlyph.getCommand();
       }
       return command;
    }

    /**
     * Sets the command of a glyph
     * @param command - Command to set the glyph to
     */
    public void setCommand(Command command){}

    /**
     * Gets the command for the glyph
     * @return - command for the glyph.
     */
    protected Command getCommand(){
        return null;
    }

    /**
     * Adds a child to a composite glyph, by default throws an exception for leafs.
     * @param window - Interface to interact with swing window.
     * @param glyph - A object of type glyph
     * @param position - Position in which to add into the list.
     * @throws Exception - Throws exception on leaf nodes.
     */
    public void insert(Window window, Glyph glyph, int position) throws Exception{
        throw new Exception("Can't insert on leaf!");
    }

    /**
     * Appends a child to the end of a composite glyph, by default throws an exception for leafs.
     * @param window - Interface to interact with swing window.
     * @param glyph - A object of type glyph
     * @throws Exception - Throws exception on leaf nodes.
     */
    public void insert(Window window, Glyph glyph) throws Exception{
        throw new Exception("Can't insert on leaf!");
    }

    /**
     * Removes a child from a composite glyph, by default throws an exception for leafs.
     * @param window - Interface to interact with swing window
     * @param glyph - The glyph to remove
     * @throws Exception - Throws exception on leaf nodes.
     */
    public void remove(Window window, Glyph glyph) throws Exception{
           throw new Exception("Can't remove from a leaf!");
    }

    /**
     * Returns a child glyph at a specified position
     * @param position - The position the child is at.
     * @return - Child glyph at that position
     * @throws Exception - Throws an exception by default.
     */
    public Glyph child(int position) throws Exception {
        throw new Exception("Can't get child for this object!");
    }

    /**
     * Finds the root of this glyph
     * @return - The root glyph.
     */
    public Glyph findRoot(){
        Glyph root = this;
        while(root.getParent() != null){
            root = root.getParent();
        }
        return root;
    }

    /**
     * Sets the size of a leaf element
     * @param window - Reference to swing window
     */
    public void setSizeChild(Window window){};

    /**
     * Sets the position of the child in relation to the cursor
     * @param cursor - Cursor to modify child
     */
    public void setPosition(Point cursor){
        getBounds().getPoint().setCoords(cursor.getX(), cursor.getY());
    }

    /**
     * Gets the parent for this glyph
     * @return - The glyphs parent
     */
    public Glyph getParent() {
        return parent;
    }

    /**
     * Sets the parent of the glyph
     * @param glyph - The glyphs parent.
     */
    public void setParent(Glyph glyph){
        parent = glyph;
    }
}
