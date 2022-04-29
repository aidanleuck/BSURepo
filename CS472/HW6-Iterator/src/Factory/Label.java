package Factory;

import Glyph.Embellishment.Embellishment;
import Glyph.Glyph;
import Iterator.Iterator;
import Window.Window;
import Glyph.Point;

/**
 * Defines a label glyph.
 */
abstract class Label extends Embellishment {
    private String color;
    public Label(Glyph glyph, Window window) {
        super(glyph, window);
    }

    @Override
    public void draw(Window window) {
        // Get size of label and draws it
        Point point = getBounds().getPoint();
        window.drawLabel(point.getX(), point.getY(), getBounds().getWidth(), getBounds().getHeight(), color);

        // Draws child of label.
        Iterator<Glyph> listIterator = this.createIterator();
        for(listIterator.first(); !listIterator.isDone(); listIterator.next()){
            Glyph child = listIterator.currentItem();
            child.draw(window);
        }
    }

    @Override
    public void setSize(Window window) {
        // Calculates the width based off the window
        int width = 0;
        int height = 0;

        // Loops through children (should only be one)
        // Sets size based on size of child.
        Iterator<Glyph> listIterator = this.createIterator();
        for(listIterator.first(); !listIterator.isDone(); listIterator.next()){
            Glyph child = listIterator.currentItem();
            width+= child.getBounds().getWidth();
            height+= child.getBounds().getHeight();
        }
        getBounds().setDims(width, height);
    }

    @Override
    public void adjustCursor(Point cursor, Glyph child) {
        // Cursor doesn't need to adjust since size is equal to child.
    }

    /**
     * Sets color of the label
     * @param color - Color to set button to.
     */
    public void setColor(String color){
        this.color = color;
    }
}
