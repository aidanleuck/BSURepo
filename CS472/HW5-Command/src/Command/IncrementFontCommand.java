package Command;

import Window.Window;

/**
 * ConcreteCommand (Command 233)
 * Concrete Prototype (Prototype 117)
 * Increments font size in window
 */
public class IncrementFontCommand implements Command{
    private final int FONT_SIZE_INCREMENT = 1;
    @Override
    public void unExecute(Window window) {
        // Decrements font size of window to undo the increment.
        int newFontSize = window.getFontSize() - FONT_SIZE_INCREMENT;
        window.setFontSize(newFontSize);
    }

    @Override
    public void execute(Window window) {
        // Increments the font size by 1.
        int newFontSize = window.getFontSize() + FONT_SIZE_INCREMENT;
        window.setFontSize(newFontSize);
    }

    @Override
    public boolean isUndoable() {
        return true;
    }

    @Override
    public Command cloneCommand() {
        return this;
    }
}
