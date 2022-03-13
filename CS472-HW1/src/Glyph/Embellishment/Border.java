package Glyph.Embellishment;

import Glyph.Glyph;
import Glyph.Point;
import Window.Window;

public class Border extends Embellishment{

    private int borderWidth;
    public Border(int bWidth){
        borderWidth = bWidth;
    }

    @Override
    public Point createCursor(){
        Point newCursor = super.createCursor();
        newCursor.setCoords(newCursor.getX() + borderWidth, newCursor.getY() + borderWidth);
        return newCursor;
    }

    @Override
    public void setSize(Window window) {
        // Calculates the width based off the window
        int width = borderWidth*2;
        int height = borderWidth*2;

        // Loops through children (should only be one)
        for(Glyph child: childList){
            width+= child.getBounds().getWidth();
            height+= child.getBounds().getHeight();
        }
        getBounds().setDims(width, height);
    }

    @Override
    public void draw(Window window){
        Point point = getBounds().getPoint();
        window.addBorder(point.getX(), point.getY(), point.getX() + getBounds().getWidth(), point.getY() + getBounds().getHeight(), borderWidth);
        for(Glyph child: childList){
            child.draw(window);
        }
    }

    // We don't need to do any adjustments since there is only one child and the border will already
    // be in the correct position.
    @Override
    public void adjustCursor(Point cursor, Glyph child){}

}
