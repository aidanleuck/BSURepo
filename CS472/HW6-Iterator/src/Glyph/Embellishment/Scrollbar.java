package Glyph.Embellishment;

import Glyph.Glyph;
import Glyph.Bounds;
import Glyph.Point;
import Iterator.Iterator;
import Visitor.GlyphVisitor;
import Window.Window;

/**
 * Concrete Element in Visitor (331)
 */
public class Scrollbar extends Embellishment {
    // default scrollbar widths
    private final int SCROLLBAR_WIDTH = 10;

    public Scrollbar(Glyph glyph, Window window){
        super(glyph, window);
    }

    @Override
    public void setSize(Window window) {
        // Bounds of scrollbar is width/height of children + its own width
        int height = 0;
        int width = SCROLLBAR_WIDTH;

        Iterator<Glyph> listIterator = createIterator();
        for(listIterator.first(); !listIterator.isDone(); listIterator.next()){
            Glyph child = listIterator.currentItem();
            Bounds childBounds =  child.getBounds();
            height += childBounds.getHeight();
            width += childBounds.getWidth();
        }
        getBounds().setDims(width, height);
    }

    @Override
    public void draw(Window window){
        Iterator<Glyph> listIterator = createIterator();
        for(listIterator.first(); !listIterator.isDone(); listIterator.next()) {
            Glyph child = listIterator.currentItem();
            child.draw(window);
        }
        Point currentPoint = getBounds().getPoint();

        // reset iterator
        listIterator.first();
        Glyph child = listIterator.currentItem();
        Bounds childBounds = child.getBounds();
        window.addScrollBar(currentPoint.getX() + childBounds.getWidth(), currentPoint.getY(), SCROLLBAR_WIDTH, childBounds.getHeight());
    }
    @Override
    public void accept(GlyphVisitor v) {
        v.visit(this);
    }

    /**
     * Adjust the scrollbar to the right side of its component.
     * @param cursor - The cursor to update
     * @param child  - The child thats bounds will update the cursor
     */
    @Override
    public void adjustCursor(Point cursor, Glyph child) {
    }
}
