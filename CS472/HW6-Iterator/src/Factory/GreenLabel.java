package Factory;

import Glyph.Glyph;
import Window.Window;

/**
 * Defines label with green look and feel.
 */
class GreenLabel extends Label {
    public GreenLabel(Glyph glyph, Window window){
        super(glyph, window);
        // Set color of label to green.
        setColor("Green");
    }
}
