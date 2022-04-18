package Command;

import java.util.ArrayList;
import java.util.List;

public class CommandHistory{
    private static CommandHistory cHistory;
    private List<Command> commandList;
    private int cursor;
    private CommandHistory(){
        commandList = new ArrayList<>();
        cursor = 0;
    };

    public static CommandHistory getHistory(){
        if(cHistory == null){
            cHistory = new CommandHistory();
        }
        return cHistory;
    }
    public void push(Command command){
        List<Command> newList = commandList.subList(0, cursor);
        newList.add(command);
        cursor += 1;
        commandList = newList;
    }
    public Command undoCommand(){
        Command commandToUndo = commandList.get(cursor - 1);
        cursor -= 1;
        return commandToUndo;
    }
    public Command redoCommand(){
        Command commandToRedo = commandList.get(cursor);
        cursor += 1;
        return commandToRedo;
    }
}
