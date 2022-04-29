package Iterator;

/**
 * Concrete Iterator in Iterator(257)
 * Defines iterator for containers with no children
 * @param <T> - Type of the container
 */
public class NullIterator<T> implements Iterator<T>{
    @Override
    public void first() {}

    @Override
    public void next() {}

    @Override
    // Always returns true since no children
    public boolean isDone() {
        return true;
    }

    @Override
    public T currentItem() {
        return null;
    }
}
