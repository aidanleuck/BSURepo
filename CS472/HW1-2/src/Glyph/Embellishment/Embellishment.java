package Glyph.Embellishment;

import Glyph.Composition;
import Glyph.Glyph;
import Glyph.Point;
import Window.Window;

public abstract class Embellishment extends Composition {
    public Embellishment(Glyph glyph, Window window){
        super.insert(window, glyph, 0);
    }

    @Override
    public void insert(Window window, Glyph glyph, int position) {
        try{
            if(childList.isEmpty()){
                super.insert(window, glyph, 0);
            }
            else{
                Glyph child = childList.get(0);

                // Forward insert on to child
                child.insert(window, glyph, position);
            }
        }
        catch(Exception ex){ }
    }

    @Override
    public void insert(Window window, Glyph glyph) {
        try{
            if(childList.isEmpty()){
                super.insert(window, glyph);
            }
            else{
                Glyph child = childList.get(0);

                // Forward insert on to child
                child.insert(window, glyph);
            }
        }
        catch(Exception ex){ }
    }

    @Override
    public void remove(Window window, Glyph glyph){
        try{

            Glyph child = childList.get(0);

            // Checks if forward needs to occur
            if(!child.equals(glyph)){
                child.remove(window, glyph);
            }

            // Forward if child is not equal.
            else{
                super.remove(window, glyph);
            }
        }
        catch(Exception ex){}
    }
}
