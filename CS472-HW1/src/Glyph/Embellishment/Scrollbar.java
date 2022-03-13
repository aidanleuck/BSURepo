package Glyph.Embellishment;

import Glyph.Glyph;
import Glyph.Bounds;
import Glyph.Point;
import Window.Window;

public class Scrollbar extends Embellishment {
    // default scrollbar widths
    private final int SCROLLBAR_WIDTH = 10;

    public Scrollbar(){

    }
    @Override
    public void setSize(Window window) {
        // Bounds of scrollbar is width/height of children + its own width
        int height = 0;
        int width = SCROLLBAR_WIDTH;
        for(Glyph child: childList){
           Bounds childBounds =  child.getBounds();
            height += childBounds.getHeight();
            width += childBounds.getWidth();
        }
        getBounds().setDims(width, height);
    }

    @Override
    public void draw(Window window){
        for(Glyph child: childList){
            child.draw(window);
        }
        Point currentPoint = getBounds().getPoint();
        window.addScrollBar(currentPoint.getX(), currentPoint.getY(), SCROLLBAR_WIDTH, getBounds().getHeight());
    }
    /**
     * Adjust the scrollbar to the right side of its component.
     * @param cursor - The cursor to update
     * @param child  - The child thats bounds will update the cursor
     */
    @Override
    public void adjustCursor(Point cursor, Glyph child) {
        getBounds().getPoint().setCoords(cursor.getX() + child.getBounds().getWidth(), cursor.getY());
    }
}
