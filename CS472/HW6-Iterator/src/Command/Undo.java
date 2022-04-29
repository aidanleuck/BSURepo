package Command;

import Window.Window;

/**
 * ConcreteCommand (Command 233)
 * Concrete Prototype (Prototype 117)
 * Undo's an executed command
 */
public class Undo implements Command{
    @Override
    // Can't be undone so does nothing on un execute.
    public void unExecute(Window window) {

    }

    @Override
    public void execute(Window window) {
        // Gets command history and executes previous command
        CommandHistory history = CommandHistory.getHistory();
        history.undoCommand(window);
    }

    @Override
    // Can't be undone.
    public boolean isUndoable() {
        return false;
    }

    @Override
    public Command cloneCommand() {
        return this;
    }
}
