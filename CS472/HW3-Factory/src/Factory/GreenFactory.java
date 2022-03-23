package Factory;

import Window.Window;
import Glyph.Glyph;

final class GreenFactory extends WidgetFactory {
    private GreenFactory(){};

    public static WidgetFactory instance(){
        if(_factory == null){
            _factory = new GreenFactory();
        }
        return _factory;
    }

    @Override
    public Button createButton(Glyph glyph, Window window) {
        return new GreenButton(glyph, window);
    }

    @Override
    public Label createLabel(Glyph glyph, Window window) {
        return new GreenLabel(glyph, window);
    }
}
