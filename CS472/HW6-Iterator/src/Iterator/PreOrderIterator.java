package Iterator;

import java.util.EmptyStackException;
import java.util.Stack;

/**
 * Concrete Iterator in Iterator (257)
 * Iterates from given item to last item using a pre order traversal
 * @param <T> - Type of item to iterate over
 */
public class PreOrderIterator<T extends Iterable<T>> implements Iterator<T> {
    // Reference to first item to start iteration from
    private T _root;

    // Stack iterator to keep state of iteration
    private Stack<Iterator<T>> iteratorStack;
    public PreOrderIterator(T root){
        _root = root;
        iteratorStack = new Stack<>();
    }
    @Override
    public void first() {
        // Create iterator from the root
        Iterator<T> iterator = _root.createIterator();

        // Calls first on it
        iterator.first();

        // If not done add to stack
        if(!iterator.isDone()){
            iteratorStack.push(iterator);
        }
    }

    @Override
    public void next() {
        try {
            // Gets current item on top of stack and creates an iterator for it
            Iterator<T> nextIterator = iteratorStack.peek().currentItem().createIterator();

            // Initializes new iterator
            nextIterator.first();

            // Push to stack
            iteratorStack.push(nextIterator);

            while(iteratorStack.peek().isDone() && !isDone()){
                // Pop item off stack
                iteratorStack.pop();

                // Calls next on the iterator on top of stack if it is done.
                if(!isDone() && !iteratorStack.peek().isDone()){
                    iteratorStack.peek().next();
                }
            }
        } catch (EmptyStackException ex) {}
    }

    @Override
    // Done when stack is empty
    public boolean isDone() {return iteratorStack.isEmpty();}

    @Override
    public T currentItem() throws RuntimeException {
        try{
            // Gets item on top of stack
            return iteratorStack.peek().currentItem();
        }
        catch(EmptyStackException ex){
            throw new RuntimeException("There is no item in the iterator!");
        }
    }
}
