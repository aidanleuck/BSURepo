package Window;

/**
 * Creates Swing window products
 * ConcreteFactory2 in Abstract Factory (87)
 * Singleton (127)
 */
class SwingWindowFactory extends WindowFactory {
    private SwingWindowFactory(){};

    /**
     * Creates an instance of a Swing Window Factory if one does not exist.
     * Singleton (127)
     * @return - Window factory that generates swing windows.
     */
    public static WindowFactory instance(){
        if(_wFactory == null){
            _wFactory = new SwingWindowFactory();
        }
        return _wFactory;
    }

    @Override
    protected WindowImp buildWindow(String title, Window window) {
        return new SwingWindow(title, window);
    }
}
