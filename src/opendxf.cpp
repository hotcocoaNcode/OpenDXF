//
// Created by Ember Lee on 4/23/25.
//

#include "opendxf.h"
#include "interface.h"

namespace opendxf {
    bool loop() {
        object o[4] = {
            {LINE, {-1., -1.}, {-1.,  1.}},
            {LINE, {-1.,  1.}, { 1.,  1.}},
            {LINE, { 1.,  1.}, { 1., -1.}},
            {LINE, { 1., -1.}, {-1., -1.}}
        };
        const auto info = interface::update(&o, 4);

        return info.running;
    }

    int run() {
        interface::init();
        while (loop()){}
        interface::quit();
        return 0;
    }
}