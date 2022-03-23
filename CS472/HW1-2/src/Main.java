import Glyph.*;
import Glyph.Embellishment.Border;
import Glyph.Embellishment.Embellishment;
import Glyph.Embellishment.Scrollbar;
import Window.*;
import Glyph.Character;

public class Main {
    public static void main(String args[]){
        Window window = new SwingWindow("My app");

        Composition root = new Column();
        Composition row = new Row();

        Character chara = new Character('A');
        Character char3 = new Character('A');
        Character char2 = new Character('B');
        row.insert(window, chara, 0);

        Embellishment bar = new Scrollbar(row, window);
        Embellishment border = new Border(5, bar, window);

        Composition column = new Column();
        column.insert(window, char3, 0);
        column.insert(window, char2, 1);


        Glyph border2 = new Border(5, column, window);

        root.insert(window, border, 0);

        Glyph rect = new Rectangle(5,15);
        row.insert(window, rect, 2);
        border.insert(window, border2, 2);

        Character character = new Character('C');
        border.insert(window, character, 2);

        window.setContents(root);


    }
}
