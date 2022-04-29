package Iterator;

/**
 * Interface that says an object can be iterated over
 * Aggregate in Iterator(257)
 * @param <T> - Item to iterate over
 */
public interface Iterable<T> {
    // Create a new iterator
    Iterator<T> createIterator();
}
