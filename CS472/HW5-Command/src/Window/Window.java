package Window;

import Command.*;
import Command.KeyMap;
import Glyph.Glyph;

/**
 * Abstract window implementation for Lexi
 * Abstraction class in (Bridge 151)
 * Receiver class in Command(233)
 */
public abstract class Window {
    // Holds reference to the window implementation to use
    protected WindowImp _windowImp;
    protected Glyph glyph;
    protected KeyMap _keyMap;

    /**
     * Creates a new window
     * @param title - Title to set window
     */
    public Window(String title){
        // Gets a window factory
        WindowFactory wFact = WindowFactory.instance();

        // Sets window imp based off product generated by factory
        _windowImp = wFact.createWindow(title, this);
    }

    /**
     * Draws the window (used in left side of bridge to draw glyph contents)
     */
    public abstract void draw();

    /**
     * Draws a character using the window implementation
     * @param c - Character to draw
     * @param x - X coordinate at which to draw character
     * @param y - Y coordinate at which to draw character
     */
    public void drawCharacter(char c, int x, int y) {
        _windowImp.drawCharacter(c, x, y);
    }

    /**
     * Draws a rectangle using the window implementation
     * @param x - X coordinate for top left coordinate of rectangle
     * @param y - Y coordinate for top left coordinate of rectangle
     * @param width - Width of rectangle from origin
     * @param height - Height of rectangle from origin
     */
    public void drawRectangle(int x, int y, int width, int height) {
        _windowImp.drawRectangle(x, y, width, height);
    }

    /**
     * Gets the character width of the specified character.
     * @param c - Character to retrieve width for.
     * @return - Width (in pixels)
     */
    public int charWidth(char c) {
        return _windowImp.charWidth(c);
    }

    /**
     * Gets the character height of the specified character.
     * @param c - Character to retrieve height for.
     * @return - Height (in pixels)
     */
    public int charHeight(char c) {
        return _windowImp.charHeight(c);
    }

    /**
     * Sets the window content (Sets glyph and refreshes window)
     * @param glyph - Glyph to set the content of the window to.
     */
    public void setContents(Glyph glyph) {
        this.glyph = glyph;
        _windowImp.setContents();
    }

    /**
     * Adds a border to the window
     * @param x1 - Sets x coordinate of the left corner of the border.
     * @param y1 - Sets the y coordinate of the left corner of the border.
     * @param x2 - Sets the x coordinate of the right corner of the border
     * @param y2 - Sets the y coordinate of the right corner of the border.
     * @param width - Thickness/stroke of border
     */
    public void addBorder(int x1, int y1, int x2, int y2, int width) {
        _windowImp.addBorder(x1, y1, x2, y2, width);
    }

    /**
     * Adds a scrollbar to the window
     * @param x - Sets the x coordinate for the top left corner of the scrollbar.
     * @param y - Sets the y coordinate for the left corner of the scrollbar.
     * @param width - Thickness of the scrollbar
     * @param height - Height of the scrollbar in the window
     */
    public void addScrollBar(int x, int y, int width, int height) {
        _windowImp.addScrollBar(x, y, width, height);
    }

    /**
     * Draws a button with specified color on the window.
     * @param x - X coordinate of the top left corner of the button.
     * @param y - Y coordinate of the top left corner of the button.
     * @param width - Width of the button from origin x,y
     * @param height - Height of the button from origin x,y
     * @param color - Color of the button fill.
     */
    public void drawButton(int x, int y, int width, int height, String color) {
        _windowImp.drawButton(x, y, width, height, color);
    }

    /**
     * Draws a label in the window
     * @param x - X coordinate of the top left corner of the button.
     * @param y - Y coordinate of the top left corner of the button.
     * @param width - Width of the label from origin x,y
     * @param height - Height of the label from origin x,y
     * @param color - Color of the label stroke.
     */
    public void drawLabel(int x, int y, int width, int height, String color) {
        _windowImp.drawLabel(x, y, width, height, color);
    }

    /**
     * Sets the key map for the window
     * @param kMap - Key map to use
     */
    public void setKeyMap(KeyMap kMap){
        _keyMap = kMap;
    }

    /**
     * Performs a command when a key is pressed
     * @param c - Character pressed
     */
    public void key(char c) {
        if(_keyMap != null){
            Command command = _keyMap.get(c);
            if(command != null){
                try {
                    CommandHistory history = CommandHistory.getHistory();
                    history.push(this, command);
                }
                catch (IndexOutOfBoundsException ex){}
            }
        }
    }

    /**
     * Performs a command on a glyph when clicked
     * @param x - x coordinate pressed
     * @param y - y coordinate pressed
     */
    public void click(int x, int y) {
        Command command = glyph.click(x, y);
        if(command != null){
            try {
                CommandHistory history = CommandHistory.getHistory();
                history.push(this, command);
            }
            catch (IndexOutOfBoundsException ex){}
        }
    }

    /**
     * Repaints the window
     */
    public void repaint(){
        // If you are repainting it is likely we will need to recompose.
        glyph.composeGlyph(this);
        _windowImp.repaint();
    }

    /**
     * Gets the font size of the window
     * @return - font size
     */
    public int getFontSize(){
        return _windowImp.getFontSize();
    }

    /**
     * Sets the font size of the window
     * @param fontSize - Font size to set the window to
     */
    public void setFontSize(int fontSize){
        _windowImp.setFontSize(fontSize);
        draw();
        repaint();
    }

}
