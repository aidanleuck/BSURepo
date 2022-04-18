import Command.*;
import Glyph.*;
import Glyph.Embellishment.Border;
import Glyph.Embellishment.Embellishment;
import Glyph.Embellishment.Scrollbar;
import Window.*;
import Glyph.Character;
import Factory.*;

public class Main {
    public static void main(String args[]){
        KeyMap map = new KeyMap();
        map.put('i', new IncrementFontCommand());
        map.put('u', new Undo());
        map.put('r', new Redo());
        map.put('d', new DecrementFontCommand());
        Window window = new ApplicationWindow("My app");
        window.setKeyMap(map);

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
            Character plus = new Character('+');
            Command plusCommand = new IncrementFontCommand();


            Character minus = new Character('-');

            Glyph fourteenRow = new Row();
            Character one = new Character('1');
            Character four = new Character('4');
            Character whiteSpace = new Character(' ');
            fourteenRow.insert(window, one);
            fourteenRow.insert(window, four);
            fourteenRow.insert(window, whiteSpace);

            Glyph twentyRow = new Row();
            Character two = new Character('2');
            Character zero = new Character('0');

            twentyRow.insert(window, two);
            twentyRow.insert(window, zero);

            Glyph button = fact.createButton(plus, window);
            button.setCommand(plusCommand);
            Glyph button2 = fact.createButton(minus, window);
            button2.insert(window, minus);
            Glyph button3 = fact.createButton(fourteenRow, window);
            Command set14 = new SetFontSize(14);
            button3.setCommand(set14);

            Glyph button4 = fact.createButton(twentyRow, window);
            Command set20 = new SetFontSize(20);
            button4.setCommand(set20);

            buttonRow.insert(window, button);
            buttonRow.insert(window, button2);
            buttonRow.insert(window, button3);
            buttonRow.insert(window,button4);
            Command command = new DecrementFontCommand();
            button2.setCommand(command);

            root.insert(window, buttonRow);

            window.setContents(border);
        }
        catch(Exception ex){}

    }
}
