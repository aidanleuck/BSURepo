package Factory;

import Glyph.Glyph;
import Glyph.Point;
import Window.Window;

final class GreenButton extends Button{
    public GreenButton(Glyph glyph, Window window){
        super(glyph, window);
        setColor("Green");
    }
}
