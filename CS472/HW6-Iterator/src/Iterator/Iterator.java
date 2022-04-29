package Iterator;

/**
 * Iterator in Iterator(257)
 * @param <T> - Type of item to iterate over
 */
public interface Iterator<T> {
    /**
     * Initializes and sets iterator to first element of container
     */
    void first();

    /**
     * Moves iterator to next item in container
     */
    void next();

    /**
     * Returns true when iterator is at end of container, false otherwise
     * @return
     */
    boolean isDone();

    /**
     * Retrieves current item in the container
     * @return - Item in container
     */
    T currentItem();
}
