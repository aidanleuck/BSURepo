package Glyph.Embellishment;

import Glyph.Composition;
import Glyph.Glyph;
import Glyph.Point;
import Iterator.Iterator;
import Window.Window;

public abstract class Embellishment extends Composition {
    public Embellishment(Glyph glyph, Window window){
        super.insert(window, glyph, 0);
    }

    @Override
    public void insert(Window window, Glyph glyph, int position) {
        try{
            Iterator<Glyph> listIterator = createIterator();
            listIterator.first();
            if(listIterator.isDone()){
                super.insert(window, glyph, 0);
            }
            else{
                Glyph child = listIterator.currentItem();

                // Forward insert on to child
                child.insert(window, glyph, position);
            }
        }
        catch(Exception ex){ }
    }

    @Override
    public void insert(Window window, Glyph glyph) {
        try{
            Iterator<Glyph> listIterator = createIterator();
            listIterator.first();
            if(listIterator.isDone()){
                super.insert(window, glyph);
            }
            else{
                Glyph child = listIterator.currentItem();

                // Forward insert on to child
                child.insert(window, glyph);
            }
        }
        catch(Exception ex){ }
    }

    @Override
    public void remove(Window window, Glyph glyph){
        try{
            Iterator<Glyph> listIterator = createIterator();
            listIterator.first();
            Glyph child = listIterator.currentItem();

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
