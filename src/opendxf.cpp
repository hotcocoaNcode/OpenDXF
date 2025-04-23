//
// Created by Ember Lee on 4/23/25.
//

#include "opendxf.h"
#include "interface.h"

namespace opendxf {
    bool loop() {
        interface::update_information info = interface::update();

        return info.running;
    }

    int run() {
        interface::init();
        while (loop()){}
        interface::quit();
        return 0;
    }
}