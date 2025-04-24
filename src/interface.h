//
// Created by Ember Lee on 4/23/25.
//

#ifndef RENDER_H
#define RENDER_H

#define _OPENDXF_WINDOW_BASE_WIDTH 800
#define _OPENDXF_WINDOW_BASE_HEIGHT 600

namespace opendxf::interface {
    struct update_information {
        bool running = true;
    };

    void init();
    update_information update(void* objects, unsigned int count);
    void quit();
}

#endif //RENDER_H
