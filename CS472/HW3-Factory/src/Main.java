import Glyph.*;
import Glyph.Embellishment.Border;
import Glyph.Embellishment.Embellishment;
import Glyph.Embellishment.Scrollbar;
import Window.*;
import Glyph.Character;
import Factory.*;

public class Main {
    public static void main(String args[]){
        Window window = new SwingWindow("My app");

        try{
            WidgetFactory fact = WidgetFactory.instance();
            WidgetFactory fact2 = WidgetFactory.instance();
            Glyph root = new Column();

            Glyph row = new Row();
            Glyph a = new Character('a');
            Glyph rowRect = new Rectangle(5,15);
            Glyph column = new Column();
            Glyph x = new Character('X');

            Glyph p = new Character('p');
            Glyph q = new Character('q');
            Glyph containingRow = new Row();
            containingRow.insert(window, p);
            containingRow.insert(window, q);
            Glyph label1 = fact.createLabel(containingRow, window);

            Glyph Z = new Character('Z');

            Glyph b = new Character('b');

            row.insert(window, a);
            row.insert(window, rowRect);
            row.insert(window, column);
            row.insert(window, b);

            column.insert(window, x);
            column.insert(window, label1);
            column.insert(window, Z);

            Glyph scrollbar = new Scrollbar(root, window);
            Glyph border = new Border(5, scrollbar, window);

            Glyph middleRow = new Row();
            Glyph x2 = new Character('x');
            Glyph rectangleMiddle = new Rectangle(25, 10);
            Glyph y = new Character('y');

            middleRow.insert(window, x2);
            middleRow.insert(window, rectangleMiddle);
            middleRow.insert(window, y);
            root.insert(window, row);
            root.insert(window, middleRow);

            Glyph buttonRow = new Row();
            Character bigP = new Character('P');
            Character bigQ = new Character('Q');

            buttonRow.insert(window,bigP);
            buttonRow.insert(window, bigQ);

            Button button = fact.createButton(buttonRow, window);

            root.insert(window, button);


            window.setContents(border);
        }
        catch(Exception ex){}



    }
}
