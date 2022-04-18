package Command;

import Window.Window;

public class Undo extends Command{
    @Override
    public void unExecute(Window window) {

    }

    @Override
    public void execute(Window window) {
        CommandHistory history = CommandHistory.getHistory();
        Command undoCommand = history.undoCommand();
        undoCommand.unExecute(window);
    }

    @Override
    public boolean isUndoable() {
        return false;
    }

    @Override
    public Command cloneCommand() {
        return null;
    }
}
