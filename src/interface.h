//
// Created by Ember Lee on 4/23/25.
//

#ifndef RENDER_H
#define RENDER_H

#define WINDOW_BASE_WIDTH 800
#define WINDOW_BASE_HEIGHT 600

namespace opendxf {
    namespace interface {
        struct update_information {
            bool running = true;
        };

        void init();
        update_information update();
        void quit();
    }
}

#endif //RENDER_H
