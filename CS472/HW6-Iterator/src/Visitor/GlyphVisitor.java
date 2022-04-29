package Visitor;

import Glyph.*;
import Glyph.Character;
import Glyph.Embellishment.Border;
import Glyph.Embellishment.Scrollbar;

/**
 * Visitor in Visitor (331)
 */
public interface GlyphVisitor {

    /**
     * Defines visit for concrete Glyph character
     * @param character - Character glyph
     */
    void visit(Character character);

    /**
     * Defines visit for concrete glyph Column
     * @param column - column glyph
     */
    void visit(Column column);

    /**
     * Defines visit for concrete Glyph row
     * @param row - row glyph
     */
    void visit(Row row);

    /**
     * Defines visit for concrete Glyph rectangle
     * @param rectangle - rectangle glyph
     */
    void visit(Rectangle rectangle);

    /**
     * Defines visit for concrete Glyph Border
     * @param border - border glyph
     */
    void visit(Border border);

    /**
     * Defines visit for concrete Glyph scrollbar
     * @param scrollbar - scrollbar glyph
     */
    void visit(Scrollbar scrollbar);

    /**
     * Defines visit for glyphs that don't have an accept method
     * (Buttons and Labels since they are hidden)
     * @param glyph - Base Glyph class
     */
    void visit(Glyph glyph);
}
