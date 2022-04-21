package Factory;

import Command.Command;
import Glyph.Embellishment.Embellishment;
import Glyph.Glyph;
import Window.Window;
import Glyph.Point;

/**
 * Defines a button glyph.
 */
abstract class Button extends Embellishment {
    private String color;
    private Command command;

    public Button(Glyph glyph, Window window) {
        super(glyph, window);
    }

    @Override
    public void setCommand(Command command){
        this.command = command;
    }

    @Override
    protected Command getCommand(){
        return command;
    }

    @Override
    public void draw(Window window) {
        Point point = getBounds().getPoint();
        window.drawButton(point.getX(), point.getY(), getBounds().getWidth(), getBounds().getHeight(), color);

        for(Glyph child: childList){
            child.draw(window);
        }
    }

    @Override
    public void setSize(Window window) {
        // Calculates the width based off the window
        int width = 0;
        int height = 0;

        // Loops through children (should only be one)
        // Sets size based off size of child
        for(Glyph child: childList){
            width+= child.getBounds().getWidth();
            height+= child.getBounds().getHeight();
        }
        getBounds().setDims(width, height);
    }

    @Override
    public void adjustCursor(Point cursor, Glyph child) {
        // Cursor does not need to be adjusted since a button
        // is the same size as child.
    }


    /**
     * Sets color of the button
     * @param color - Color of the button to be set.
     */
    public void setColor(String color){
        this.color = color;
    }
}
