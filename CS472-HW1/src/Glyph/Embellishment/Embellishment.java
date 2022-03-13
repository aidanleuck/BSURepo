package Glyph.Embellishment;

import Glyph.Composition;
import Glyph.Glyph;
import Glyph.Point;
import Window.Window;

public abstract class Embellishment extends Composition {
    @Override
    public void insert(Window window, Glyph glyph, int position){
        glyph.setParent(this);

        // enforces that childlist only has one item
        if(childList.size() > 0){
            childList.remove(0);
        }
        childList.add(0, glyph);
        findRoot().composeGlyph(window);
    }
}
