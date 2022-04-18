package Command;

import Window.Window;

public class Redo extends Command{
    @Override
    public void unExecute(Window window) {

    }

    @Override
    public void execute(Window window) {
        CommandHistory history = CommandHistory.getHistory();
        Command redoCommand = history.redoCommand();
        redoCommand.execute(window);
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
