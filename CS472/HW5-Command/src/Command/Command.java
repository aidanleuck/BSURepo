package Command;

import Window.Window;

public abstract class Command implements Prototype {
    public abstract void unExecute(Window window);
    public abstract void execute(Window window);
    public abstract boolean isUndoable();
}
