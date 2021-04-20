#include <iostream>
#include <functional>
#include <memory>

#include <boost/asio/coroutine.hpp>
#include <boost/asio/yield.hpp>

class Foo
{
public:
    Foo() = default;

    template<typename HandlerCoro>
    void operator()(HandlerCoro && handler)
    {
        int value;
        std::cin >> value;
        std::cout << "Foo next: " << value << std::endl;

        handler();
    }
};

class connection :
        public std::enable_shared_from_this<connection>,
        private boost::asio::coroutine
{
public:
    connection()
    {
        std::cout << "ctor" << std::endl;
    }

    void start()
    {
        std::cout << "enter " << std::endl;
        auto self = shared_from_this();
        auto funcThis = [self] { self->start(); };
        Foo foo;
        reenter(this)
        {
            yield
            {
                std::cout << "yield 1" << std::endl;
                foo(funcThis);
            }
            yield
            {
                std::cout << "yield 2" << std::endl;
                foo(funcThis);
            }
        }

        if (self->is_complete())
        {
            std::cout << "is complete" << std::endl;
        }
    }

    void generate()
    {
        int num = 0;
        auto self = shared_from_this();
        auto func = [self]() {
            static int value = 0;
            std::cout << value << std::endl;
            ++value;
            if (value < 10) self->generate();
        };
        reenter(this) for(;;)
        {
            yield func();
        }
    }

};


int main()
{
    auto connect = std::make_shared<connection>();
    connect->start();
//    connect->generate();

    return 0;
}
