package Factory;

import Glyph.Glyph;
import Window.Window;

/**
 * Button with red look and feel.
 */
class RedButton extends Button {
    public RedButton(Glyph glyph, Window window){
        super(glyph, window);
        // Sets color to red.
        setColor("Red");
    }
}
