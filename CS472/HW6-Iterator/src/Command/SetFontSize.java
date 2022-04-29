package Command;

import Window.Window;

/**
 * ConcreteCommand (Command 233)
 * Concrete Prototype (Prototype 117)
 * Sets the font size of the window to specified size
 */
public class SetFontSize implements Command{
    private int fontSize;

    // Keeps track of state for undo/redo
    private Integer oldFontSize;
    private Integer newFontSize;

    public SetFontSize(int fontSize){
        this.fontSize = fontSize;
    }
    @Override

    public void unExecute(Window window) {
        if(oldFontSize != null && newFontSize != null){
            // Uses state to undo execute and swaps the new/old state
            window.setFontSize(oldFontSize);
            Integer temp = newFontSize;
            newFontSize = oldFontSize;
            oldFontSize = temp;
        }
    }

    @Override
    public void execute(Window window) {
        // Saves the state and sets window to the font size
        oldFontSize = window.getFontSize();
        newFontSize = this.fontSize;
        window.setFontSize(this.fontSize);
    }

    @Override
    public boolean isUndoable() {
        return true;
    }

    @Override
    public Command cloneCommand() {
        // Clones the data by using saved state and pushing to new object.
        SetFontSize cloneSize = new SetFontSize(this.fontSize);
        cloneSize.oldFontSize = oldFontSize;
        cloneSize.newFontSize = newFontSize;

        return cloneSize;
    }
}
