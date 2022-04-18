package Glyph;

import Window.*;

/**
 * Handles displaying a character glyph in the window.
 */
public class Character extends Glyph {
    private char _character;

    public Character(java.lang.Character character){
        _character = character;
    }

    @Override
    public void draw(Window window){
        // Draws the character on the window by calling window drawCharacter method
        Point boundsPoint = getBounds().getPoint();
        window.drawCharacter(_character, boundsPoint.getX(), boundsPoint.getY());
    }

    @Override
    public void setSizeChild(Window window) {
        Bounds charBounds = getBounds();

        // Sets the size of the bounds based on the character height/width of the window.
        charBounds.setHeight(window.charHeight(_character));
        charBounds.setWidth(window.charWidth(_character));
    }

}
