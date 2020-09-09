#include <iostream>
#include <atomic>
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <unistd.h>

using namespace std;

int main(void)
{
    lo::ServerThread st(9000);
    if (!st.is_valid()) {
        std::cout << "Nope." << std::endl;
        return EXIT_FAILURE;
    }

    st.set_callbacks([&st](){printf("Thread init: %p.\n",&st);},
                     [](){printf("Thread cleanup.\n");});

    std::cout << "URL: " << st.url() << std::endl;

    std::atomic<int> received(0);

    st.add_method("example", "i",
                [&received](lo_arg **argv, int)
                {std::cout << "example (" << (++received) << "): "
                            << argv[0]->i << std::endl;});

    st.start();

    while(true)
        usleep(10*1000);

    return EXIT_SUCCESS;
}