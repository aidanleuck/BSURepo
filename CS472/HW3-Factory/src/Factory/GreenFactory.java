package Factory;

import Window.Window;
import Glyph.Glyph;

/**
 * Creates products with green look and feel.
 */
class GreenFactory extends WidgetFactory {
    private GreenFactory(){};

    /**
     * Singleton for returning an instance of green factory.
     * @return - Green factory widget factory
     */
    public static WidgetFactory instance(){
        if(_factory == null){
            _factory = new GreenFactory();
        }
        return _factory;
    }

    @Override
    protected Glyph buildButton(Glyph glyph, Window window) {
        return new GreenButton(glyph, window);
    }

    @Override
    protected Glyph buildLabel(Glyph glyph, Window window) {
        return new GreenLabel(glyph, window);
    }
}
