package Command;

import Window.Window;

public class SetFontSize extends Command{
    private int fontSize;
    private Integer oldFontSize;
    private Integer newFontSize;

    public SetFontSize(int fontSize){
        this.fontSize = fontSize;
    }
    @Override
    public void unExecute(Window window) {
        if(oldFontSize != null && newFontSize != null){
            window.setFontSize(oldFontSize);
            Integer temp = newFontSize;
            newFontSize = oldFontSize;
            oldFontSize = temp;
        }
    }

    @Override
    public void execute(Window window) {
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
        SetFontSize cloneSize = new SetFontSize(this.fontSize);
        cloneSize.oldFontSize = oldFontSize;
        cloneSize.newFontSize = newFontSize;

        return cloneSize;
    }
}
