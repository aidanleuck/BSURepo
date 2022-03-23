package Factory;

import Window.Window;
import Glyph.Glyph;

public abstract class WidgetFactory {
    protected static WidgetFactory _factory;

    public static WidgetFactory instance(){
        if(_factory == null){
            String factoryType =System.getenv("Feel");
            if(factoryType != null && factoryType.equalsIgnoreCase("Red")){
                _factory = RedFactory.instance();
            }
            else{
                _factory = GreenFactory.instance();
            }
        }
        return _factory;
    }

    public abstract Button createButton(Glyph glyph, Window window);
    public abstract Label createLabel(Glyph glyph, Window window);
}


