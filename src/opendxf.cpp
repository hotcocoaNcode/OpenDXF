//
// Created by Ember Lee on 4/23/25.
//

#include "opendxf.h"
#include "interface.h"

#include <vector>

namespace opendxf {
    std::vector<object> document{};

    bool loop() {
        const auto info = interface::update(&document[0], document.size());

        return info.running;
    }

    int run() {
        interface::init();
        document.push_back({LINE, {-1., -1.}, {-1.,  1.}});
        document.push_back({LINE, {-1.,  1.}, { 1.,  1.}});
        document.push_back({LINE, { 1.,  1.}, { 1., -1.}});
        document.push_back({LINE, { 1., -1.}, {-1., -1.}});
        while (loop()){}
        interface::quit();
        return 0;
    }
}