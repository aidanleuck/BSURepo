package Command;

import Window.Window;

/**
 * ConcreteCommand (Command 233)
 * Concrete Prototype (Prototype 117)
 * Decrements font size in window
 */
public class DecrementFontCommand implements Command{
    private final int FONT_SIZE_INCREMENT = 1;

    @Override
    public void unExecute(Window window) {
        // Increments font size to undo decrement
        int newFontSize = window.getFontSize() + FONT_SIZE_INCREMENT;
        window.setFontSize(newFontSize);
    }

    @Override
    public void execute(Window window) {
        // Decrements font size by 1
        int newFontSize = window.getFontSize() - FONT_SIZE_INCREMENT;
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
