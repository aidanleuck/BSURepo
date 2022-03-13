import Glyph.*;
import Glyph.Embellishment.Border;
import Glyph.Embellishment.Embellishment;
import Glyph.Embellishment.Scrollbar;
import Window.*;
import Glyph.Character;

public class Main {
    public static void main(String args[]){
        Window window = new SwingWindow("My app");
        Compositor compositor = new SimpleCompositor();
        Compositor compositor2 = new SimpleCompositor();
        Compositor rootCompositor = new SimpleCompositor();
        Compositor compositor3 = new SimpleCompositor();
        Compositor compositor4 = new SimpleCompositor();


        Composition root = new Column();
        root.setCompositor(rootCompositor);

        Embellishment border = new Border(5);
        border.setCompositor(compositor3);

        Embellishment bar = new Scrollbar();
        bar.setCompositor(compositor4);

        Composition row = new Row();
        row.setCompositor(compositor2);
        Character chara = new Character('A');
        Character char3 = new Character('A');
        Character char2 = new Character('B');
        row.insert(window, chara, 0);

        Composition column = new Column();
        column.setCompositor(compositor);
        column.insert(window, char3, 0);
        column.insert(window, char2, 1);

        row.insert(window, column, 1);
        bar.insert(window, row, 0);
        border.insert(window, bar, 0);


        root.insert(window, border, 0);


        Glyph rect = new Rectangle(5,15);
        row.insert(window, rect, 2);


        window.setContents(root);


    }
}
