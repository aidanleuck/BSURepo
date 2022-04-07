package Factory;

import Glyph.Glyph;
import Window.Window;

/**
 * Defines buttons with a green look and feel.
 */
class GreenButton extends Button{
    public GreenButton(Glyph glyph, Window window){
        super(glyph, window);
        // Set color of button to green.
        setColor("Green");
    }
}
