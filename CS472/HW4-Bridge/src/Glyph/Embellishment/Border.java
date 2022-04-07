package Glyph.Embellishment;

import Glyph.Glyph;
import Glyph.Point;
import Window.Window;

public class Border extends Embellishment{

    private int strokeWeight;
    public Border(int bWidth, Glyph glyph, Window window){
        super(glyph, window);
        strokeWeight = bWidth;
    }

    @Override
    public Point createCursor(){
        Point newCursor = super.createCursor();
        newCursor.setCoords(newCursor.getX() + strokeWeight, newCursor.getY() + strokeWeight);
        return newCursor;
    }

    @Override
    public void setSize(Window window) {
        // Calculates the width based off the window
        int width = strokeWeight*2;
        int height = strokeWeight*2;

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
        window.addBorder(point.getX(), point.getY(), point.getX() + getBounds().getWidth(), point.getY() + getBounds().getHeight(), strokeWeight);
        for(Glyph child: childList){
            child.draw(window);
        }
    }

    // We don't need to do any adjustments since there is only one child and the border will already
    // be in the correct position.
    @Override
    public void adjustCursor(Point cursor, Glyph child){

    }
}
