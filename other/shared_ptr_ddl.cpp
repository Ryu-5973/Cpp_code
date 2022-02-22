#include <iostream>
#include <memory>

using namespace std;

class Controller {
public:
    Controller() = default;

    ~Controller() {
        std::cout << "in ~Controller" << std::endl;
    }

    class SubController {
    public:
        SubController() = default;

        ~SubController() {
            std::cout << "in ~SubController" << std::endl;
        }

        std::shared_ptr<Controller> controller_;
    };

    std::shared_ptr<SubController> sub_controller_;
};

void func() {
    Controller p;
}
int main(int argc, char *argv[]) {
    func();
    return 0;
}