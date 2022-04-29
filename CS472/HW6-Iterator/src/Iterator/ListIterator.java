package Iterator;

import java.util.List;

/**
 * Concrete Iterator in Iterator(257)
 * Iterates over items conforming to list interface
 * @param <T> - Type of item to iterate over
 */
public class ListIterator<T> implements Iterator<T> {
    // Use Java list interface
    private List<T> _list;
    int index;

    public ListIterator(List<T> list){
        _list = list;
    }
    @Override
    public void first() {
        // Resets index when first is called
        index = 0;
    }

    @Override
    public void next() {
        // Increments index when next is called
        index++;
    }

    @Override
    public boolean isDone() {
        // Done when index is equal to list size since index starts at zero
        return index == _list.size();
    }

    @Override
    public T currentItem() {
        // Retrieves item at current index
        T item = null;
        if(!isDone()){
            item = _list.get(index);
        }
        return item;
    }
}
