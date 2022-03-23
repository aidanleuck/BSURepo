package Factory;

import Glyph.Glyph;
import Window.Window;

final class RedLabel extends Label {
    public RedLabel(Glyph glyph, Window window){
        super(glyph, window);
        setColor("Red");
    }
}
