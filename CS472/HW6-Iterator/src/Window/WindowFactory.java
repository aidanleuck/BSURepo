package Window;

/**
 * Abstract Factory (87)
 * Singleton (127)
 * Produces different types of window factories.
 */
abstract class WindowFactory {
    // Concrete factory that the window factory is set to.
    protected static WindowFactory _wFactory;

    /**
     * Singleton (127)
     * Method to get instance of window factory
     * @return - Instance of window factory depending on environment variable.
     */
    public static WindowFactory instance(){
        if(_wFactory == null){
            // Uses environment variable to determine Look and Feel.
            String winType = System.getenv("Window");

            // By default use SwingWindowFactory
            // use AWTWindowFactory if environment variable is equal to Awt.
            if(winType != null && winType.equalsIgnoreCase("Awt")){
                _wFactory = AwtWindowFactory.instance();
            }
            else{
                _wFactory = SwingWindowFactory.instance();
            }
        }
        return _wFactory;
    }

    /**
     * Factory method for creating windows
     * @param title - Title of the window
     * @param window - Window interface to use
     * @return - Window implementation to client.
     */
    protected abstract WindowImp buildWindow(String title, Window window);

    /**
     * Creator method for Windows via indirection.
     * @param title - Title of the window
     * @param window - Window interface to use
     * @return - Window implementation.
     */
    public final WindowImp createWindow(String title, Window window){
        return buildWindow(title, window);
    }


}
