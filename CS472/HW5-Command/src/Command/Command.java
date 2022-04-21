package Command;

import Window.Window;

/**
 * Command Class in Command(233)
 */
public interface Command extends Prototype {
    /**
     * Unexecutes a command (undo)
     * @param window - Window
     */
    void unExecute(Window window);

    /**
     * Executes a command and performs action on receiver (window)
     * @param window
     */
    void execute(Window window);

    /**
     * Determines if a command can be undone after it has executed.
     * @return - true if undoable, false otherwise
     */
    boolean isUndoable();
}
