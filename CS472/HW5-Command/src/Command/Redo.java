package Command;

import Window.Window;

/**
 * ConcreteCommand (Command 233)
 * Concrete Prototype (Prototype 117)
 * Redos a command
 */
public class Redo implements Command{

    @Override
    // Un execute is empty since it can't be undone.
    public void unExecute(Window window) {

    }

    @Override
    public void execute(Window window) {
        CommandHistory history = CommandHistory.getHistory();
        // Redos the command
        history.redoCommand(window);
    }

    @Override
    // Can't be undone
    public boolean isUndoable() {
        return false;
    }


    @Override
    public Command cloneCommand() {
        return this;
    }
}
