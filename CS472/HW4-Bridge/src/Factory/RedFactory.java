package Factory;

import Glyph.Glyph;
import Window.Window;

/**
 * Creates products with red look and feel.
 */
class RedFactory extends WidgetFactory {
    private RedFactory(){}

    /**
     * Singleton for returning an instance of red factory.
     * @return - Red factory widget factory
     */
    public static WidgetFactory instance(){
        if(_factory == null){
            _factory = new RedFactory();
        }
        return _factory;
    }

    @Override
    protected Glyph buildButton(Glyph glyph, Window window) {
        return new RedButton(glyph, window);
    }

    @Override
    protected Glyph buildLabel(Glyph glyph, Window window) {
        return new RedLabel(glyph, window);
    }
}
