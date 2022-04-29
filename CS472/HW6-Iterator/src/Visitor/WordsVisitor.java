package Visitor;

import Glyph.*;
import Glyph.Character;
import Glyph.Embellishment.Border;
import Glyph.Embellishment.Scrollbar;

/**
 * Concrete Visitor in Visitor (331)
 * Describes how each type of glyph should be handled when looking
 * for words in a document
 */
public class WordsVisitor implements GlyphVisitor {
    // Determines if a new line needs to be added
    boolean newLine;

    // Keeps internal state of built string
    StringBuilder builder;

    public WordsVisitor(){
        newLine = false;
        builder = new StringBuilder();
    }

    @Override
    public void visit(Character character) {
        if(newLine){
            // Adds one newline if we hit another type of glyph
            builder.append(System.getProperty("line.separator"));

            // Resets newline
            newLine = false;
        }
        // Gets character and adds to string
        char c = character.getCharacter();
        builder.append(c);
    }

    @Override
    // Writes out the state of builder when asked
    public String toString(){
        return builder.toString();
    }

    @Override
    // Adds newline for columns
    public void visit(Column column) {
        newLine = true;
    }

    @Override
    // Adds newline for rows
    public void visit(Row row) {
        newLine = true;
    }

    @Override
    // Adds new line for rectangle
    public void visit(Rectangle rectangle) {
        newLine = true;
    }

    @Override
    // Adds new line for borders
    public void visit(Border border) {
        newLine = true;
    }

    @Override
    // Adds new lines for scrollbar
    public void visit(Scrollbar scrollbar) {
        newLine = true;
    }

    @Override
    // Adds new line for any base glyph (Buttons and Labels)
    // Since they are not visible
    public void visit(Glyph glyph) {
        newLine = true;
    }
}
