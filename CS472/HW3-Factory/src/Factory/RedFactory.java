package Factory;

import Glyph.Glyph;
import Window.Window;

final class RedFactory extends WidgetFactory {
    private RedFactory(){}

    public static WidgetFactory instance(){
        if(_factory == null){
            _factory = new RedFactory();
        }
        return _factory;
    }

    @Override
    public Button createButton(Glyph glyph, Window window) {
        return new RedButton(glyph, window);
    }

    @Override
    public Label createLabel(Glyph glyph, Window window) {
        return new RedLabel(glyph, window);
    }
}
