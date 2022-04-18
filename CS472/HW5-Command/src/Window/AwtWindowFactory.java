package Window;

/**
 * Creates AWT window products
 * ConcreteFactory1 in Abstract Factory (87)
 * Singleton (127)
 */
class AwtWindowFactory extends WindowFactory {
    private AwtWindowFactory(){};

    /**
     * Creates an instance of an Awt Window Factory if one does not exist.
     * Singleton (127)
     * @return - Window factory to client
     */
    public static WindowFactory instance(){
        if(_wFactory == null){
            _wFactory = new AwtWindowFactory();
        }
        return _wFactory;
    }

    @Override
    public WindowImp buildWindow(String title, Window window) {
        return new AwtWindow(title, window);
    }
}
