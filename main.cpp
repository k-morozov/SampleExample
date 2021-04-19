#include <iostream>
#include <functional>
#include <memory>
#include <boost/asio/coroutine.hpp>

template<typename HandlerCoro>
void foo(HandlerCoro && handler, int num)
{
    std::cout << "Hello, World! " << num << std::endl;
    handler();
}

class _connection :
        public std::enable_shared_from_this<_connection>,
        private boost::asio::coroutine
{
public:
    _connection()
    {
        std::cout << "ctor" << std::endl;
    }

    void start()
    {
        std::cout << "enter " << std::endl;
        BOOST_ASIO_CORO_REENTER(this)
        {
            BOOST_ASIO_CORO_YIELD
            {
                std::cout << "yield 1" << std::endl;
                auto self = shared_from_this();
                auto funcThis = std::bind(&_connection::start, self);
                foo(funcThis, 1);
            }
            BOOST_ASIO_CORO_YIELD
            {
                std::cout << "yield 2" << std::endl;
                auto self = shared_from_this();
                auto funcThis = std::bind(&_connection::start, self);
                foo(funcThis, 2);
            }
        }
    }

};


int main()
{
    auto connect = std::make_shared<_connection>();
    connect->start();
    return 0;
}
