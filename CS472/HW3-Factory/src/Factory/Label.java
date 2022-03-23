package Factory;

import Glyph.Embellishment.Embellishment;
import Glyph.Glyph;
import Window.Window;
import Glyph.Point;

public abstract class Label extends Embellishment {
    String color;
    public Label(Glyph glyph, Window window) {
        super(glyph, window);
    }

    @Override
    public void draw(Window window) {
        Point point = getBounds().getPoint();
        window.drawLabel(point.getX(), point.getY(), getBounds().getWidth(), getBounds().getHeight(), color);

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
        for(Glyph child: childList){
            width+= child.getBounds().getWidth();
            height+= child.getBounds().getHeight();
        }
        getBounds().setDims(width, height);
    }

    @Override
    public void adjustCursor(Point cursor, Glyph child) {

    }

    public void setColor(String color){
        this.color = color;
    }
}
