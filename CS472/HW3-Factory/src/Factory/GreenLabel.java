package Factory;

import Glyph.Glyph;
import Window.Window;

final class GreenLabel extends Label {
    public GreenLabel(Glyph glyph, Window window){
        super(glyph, window);
        setColor("Green");
    }
}
