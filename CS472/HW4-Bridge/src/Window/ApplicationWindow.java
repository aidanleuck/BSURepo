package Window;

/**
 * Main Application window for Lexi
 * Refined Abstraction in Bridge (151)
 */
public class ApplicationWindow extends Window {
    public ApplicationWindow(String title){
        super(title);
    }

    @Override
    public void draw() {
        _windowImp.setContents();
        if (glyph != null) {
            glyph.draw(this);
        }
    }
}
