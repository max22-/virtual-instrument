#include <iostream>
#include <lo/lo.h>
#include <lo/lo_cpp.h>
#include <unistd.h>
#include <rtmidi/RtMidi.h>

using namespace std;

int main(void)
{
    lo::ServerThread st(9000);
    if (!st.is_valid()) {
        std::cout << "Nope." << std::endl;
        return EXIT_FAILURE;
    }

    RtMidiOut *midiout = new RtMidiOut();
    std::vector<unsigned char> midiMessage;
    midiout->openVirtualPort();

    st.set_callbacks([&st](){printf("Thread init: %p.\n",&st);},
                     [](){printf("Thread cleanup.\n");});

    std::cout << "URL: " << st.url() << std::endl;

    st.add_method("example", "i",
                [&midiMessage, midiout](lo_arg **argv, int)
                {
                    std::cout << "example : "
                            << argv[0]->i << std::endl;
                    midiMessage.clear();
                    if(argv[0]->i == 1) {
                        std::cout << "note on" << endl;
                        midiMessage.push_back(144);
                    }
                    else {
                        std::cout << "note off" << endl;
                        midiMessage.push_back(128);
                    }
                    midiMessage.push_back(82);
                    midiMessage.push_back(127);
                    std::cout << "sending message" << endl;
                    midiout->sendMessage( &midiMessage );
                });

    st.start();

    while(true)
        usleep(10*1000);

    delete midiout;

    return EXIT_SUCCESS;
}