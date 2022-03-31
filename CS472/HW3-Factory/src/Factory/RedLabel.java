package Factory;

import Glyph.Glyph;
import Window.Window;

/**
 * Label with red look and feel.
 */
class RedLabel extends Label {
    public RedLabel(Glyph glyph, Window window){
        super(glyph, window);
        // Set color of label to red.
        setColor("Red");
    }
}
