package Command;

import Window.Window;

import java.util.ArrayList;
import java.util.List;

/**
 * Singleton (127)
 * Keeps track of commands that can be undone/redone
 */
public class CommandHistory{
    private static CommandHistory cHistory;
    private List<Command> commandList;
    private int cursor;
    private CommandHistory(){
        commandList = new ArrayList<>();
        cursor = 0;
    };

    /**
     * Singleton method (127)
     * @return - Instance of command history
     */
    public static CommandHistory getHistory(){
        if(cHistory == null){
            cHistory = new CommandHistory();
        }
        return cHistory;
    }

    /**
     * Pushes a command to command history if undoable and executes command
     * @param window - window (receiver reference)
     * @param command - Command to clone/execute
     */
    public void push(Window window, Command command){
        command.execute(window);
        if(command.isUndoable()){
            Command commandCopy = command.cloneCommand();
            List<Command> newList = commandList.subList(0, cursor);
            newList.add(commandCopy);
            cursor += 1;
            commandList = newList;
        }
    }

    /**
     * Undos the most recent command in the history and decrements history pointer
     * @param window - Window (receive reference)
     */
    public void undoCommand(Window window) throws IndexOutOfBoundsException{
        Command commandToUndo = commandList.get(cursor - 1);
        cursor -= 1;
        commandToUndo.unExecute(window);
    }

    /**
     * Redos the most recently undone command in the history
     * @param window - Window (receiver reference).
     */
    public void redoCommand(Window window) throws IndexOutOfBoundsException{
        Command commandToRedo = commandList.get(cursor);
        cursor += 1;
        commandToRedo.execute(window);
    }
}
