package Command;

import Glyph.Glyph;
import Iterator.PreOrderIterator;
import Visitor.GlyphVisitor;
import Visitor.WordsVisitor;
import Window.Window;

/**
 * ConcreteCommand (Command 233)
 * Concrete Prototype (Prototype 117)
 * Finds all words in a document
 */
public class WordFinder implements Command{
    @Override
    // Does nothing since it can't be unexecuted
    public void unExecute(Window window) {}

    @Override
    // Can't be undone
    public boolean isUndoable() {
        return false;
    }

    @Override
    public void execute(Window window) {
        // Gets root glyph to start traversal from
        Glyph root = window.getRoot();
        // Create a new word visitor
        GlyphVisitor visitor = new WordsVisitor();

        // Create a pre order iterator starting from root.
        PreOrderIterator<Glyph> pOrderIterator = new PreOrderIterator<Glyph>(root);

        // Goes through each item in iteration and accepts/visits each one.
        for(pOrderIterator.first(); !pOrderIterator.isDone(); pOrderIterator.next()){
            Glyph nextGlyph = pOrderIterator.currentItem();
            nextGlyph.accept(visitor);
        }

        // Prints out after all glyphs have been visited
        System.out.println(visitor);
    }

    @Override
    public Command cloneCommand() {
        return this;
    }
}
