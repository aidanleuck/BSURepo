package Factory;

import Glyph.Glyph;
import Glyph.Point;
import Window.Window;

final class RedButton extends Button {
    public RedButton(Glyph glyph, Window window){
        super(glyph, window);
        setColor("Red");
    }
}
