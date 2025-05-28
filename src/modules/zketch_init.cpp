#include <iostream>
#include "zketch/zketch_init.h"

namespace zketch {
    static bool initialized = false;

    void init() {
        if (!initialized) {
            initialized = true;
            std::cout << "[zketch] init\n";
            // lakukan semua init yang diperlukan
        }
    }

    void shutdown() {
        if (initialized) {
            initialized = false;
            std::cout << "[zketch] shutdown\n";
            // bersihkan resource
        }
    }

    struct AutoInit {
        AutoInit()  { zketch::init(); }
        ~AutoInit() { zketch::shutdown(); }
    };

    // instance global -> dipanggil otomatis saat lib dimuat
    static AutoInit _autoinit;
}