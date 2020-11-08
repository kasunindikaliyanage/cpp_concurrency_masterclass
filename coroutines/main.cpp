//#include <experimental/coroutine>
//
//#include <iostream>
//#include <cassert>
//
//
//class my_resumable {
//public:
//    struct promise_type;
//    using coro_handle = std::experimental::coroutine_handle<promise_type>;
//    my_resumable(coro_handle handle) : handle_(handle) { assert(handle); }
//    my_resumable(my_resumable&) = delete;
//    my_resumable(my_resumable&&) = delete;
//    bool resume() {
//        if (not handle_.done())
//            handle_.resume();
//        return not handle_.done();
//    }
//    ~my_resumable() { handle_.destroy(); }
//private:
//    coro_handle handle_;
//};
//
//struct my_resumable::promise_type {
//    using coro_handle = std::experimental::coroutine_handle<promise_type>;
//    auto get_return_object() noexcept {
//        return coro_handle::from_promise(*this);
//    }
//    auto initial_suspend() noexcept { return std::experimental::suspend_always(); }
//    auto final_suspend() noexcept{ return std::experimental::suspend_always(); }
//    void return_void() noexcept {}
//    void unhandled_exception() noexcept {
//        std::terminate();
//    }
//};
//
//my_resumable foo() {
//    std::cout << "Hello" << std::endl;
//    co_await std::experimental::suspend_always();
//    std::cout << "World" << std::endl;
//}
//
//void alpha()
//{
//    std::cout << "A" << std::endl;
//    std::cout << "B" << std::endl;
//    std::cout << "C" << std::endl;
//}
//
//void numeric()
//{
//    std::cout << "1" << std::endl;
//    std::cout << "2" << std::endl;
//    std::cout << "3" << std::endl;
//}
//
//int main() 
//{
//    alpha();
//    numeric();
//    // resumable res = foo();
//    // foo().resume();
//    // while (res.resume());
//}
//
//
//


#include <experimental/coroutine>
#include <memory>
#include <iostream>

template<typename T>
struct Generator {

    struct promise_type;
    using handle_type = experimental::coroutine_handle<promise_type>;

    Generator(handle_type h) : coro(h) {}                         // (3)
    handle_type coro;

    ~Generator() {
        if (coro) coro.destroy();
    }
    Generator(const Generator&) = delete;
    Generator& operator = (const Generator&) = delete;
    Generator(Generator&& oth) noexcept : coro(oth.coro) {
        oth.coro = nullptr;
    }
    Generator& operator = (Generator&& oth) noexcept {
        coro = oth.coro;
        oth.coro = nullptr;
        return *this;
    }
    T getValue() {
        return coro.promise().current_value;
    }
    bool next() {                                                // (5)
        coro.resume();
        return not coro.done();
    }
    struct promise_type {
        promise_type() = default;                               // (1)

        ~promise_type() = default;

        auto initial_suspend() {                                 // (4)
            return experimental::suspend_always();
        }
        auto  final_suspend() noexcept {
            return experimental::suspend_always();
        }
        auto get_return_object() {                               // (2)
            return Generator{ experimental::handle_type::from_promise(*this) };
        }
        void return_void() {
            experimental::suspend_never();
        }

        auto yield_value(const T value) {                        // (6) 
            current_value = value;
            return experimental::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
        T current_value;
    };

};


Generator<int> getNext(int start = 0, int step = 1) noexcept {
    auto value = start;
    for (int i = 0;; ++i) {
        co_yield value;
        value += step;
    }
}

int main() {

    std::cout << std::endl;

    std::cout << "getNext():";
    auto gen = getNext();
    for (int i = 0; i <= 10; ++i) {
        gen.next();
        std::cout << " " << gen.getValue();                      // (7)
    }

    std::cout << "\n\n";

    std::cout << "getNext(100, -10):";
    auto gen2 = getNext(100, -10);
    for (int i = 0; i <= 20; ++i) {
        gen2.next();
        std::cout << " " << gen2.getValue();
    }

    std::cout << std::endl;

}
