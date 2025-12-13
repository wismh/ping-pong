namespace engine {

class ApplicationState {
    friend class Loop;

    bool _running = true;
public:
    void Quit() {
        _running = false;
    }
};

}