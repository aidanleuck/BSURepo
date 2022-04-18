package Command;

import Window.Window;

public class IncrementFontCommand extends Command{
    private final int FONT_SIZE_INCREMENT = 1;
    @Override
    public void unExecute(Window window) {
        int newFontSize = window.getFontSize() - FONT_SIZE_INCREMENT;
        window.setFontSize(newFontSize);
    }

    @Override
    public void execute(Window window) {
        int newFontSize = window.getFontSize() + FONT_SIZE_INCREMENT;
        window.setFontSize(newFontSize);
    }

    @Override
    public boolean isUndoable() {
        return true;
    }

    @Override
    public Command cloneCommand() {
        IncrementFontCommand IncrementCopy = new IncrementFontCommand();
        return IncrementCopy;
    }
}
