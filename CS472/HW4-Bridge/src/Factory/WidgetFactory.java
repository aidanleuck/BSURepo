package Factory;

import Window.Window;
import Glyph.Glyph;

/**
 * Factory for creating widget products.
 */
public abstract class WidgetFactory {
    protected static WidgetFactory _factory;

    /**
     * Singleton method to get instance of factory
     * @return - Instance of widget factory depending on environment variable.
     */
    public static WidgetFactory instance(){
        if(_factory == null){
            // Uses environment variable to determine Look and Feel.
            String factoryType = System.getenv("Feel");

            // By default use green factory
            // use red if environment variable is equal to red.
            if(factoryType != null && factoryType.equalsIgnoreCase("Red")){
                _factory = RedFactory.instance();
            }
            else{
                _factory = GreenFactory.instance();
            }
        }
        return _factory;
    }

    /**
     * Creator method for buttons via indirection.
     * @param glyph - Glyph that button will hold.
     * @param window - Swing window.
     * @return - button glyph.
     */
    public final Glyph createButton(Glyph glyph, Window window){
        return buildButton(glyph, window);
    }

    /**
     * Creator method for label via indirection.
     * @param glyph - Glyph that button will hold.
     * @param window - Swing window.
     * @return - label glyph.
     */
    public final Glyph createLabel(Glyph glyph, Window window){
        return buildLabel(glyph, window);
    }

    /**
     * Factory method for building a button
     * @param glyph - Glyph that the button will hold.
     * @param window - Window to use
     * @return - Button glyph.
     */
    protected abstract Glyph buildButton(Glyph glyph, Window window);

    /**
     * Factory method for building a label.
     * @param glyph - Glyph that the label will hold.
     * @param window - Window to use.
     * @return - Button glyph.
     */
    protected abstract Glyph buildLabel(Glyph glyph, Window window);
}


