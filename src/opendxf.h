//
// Created by Ember Lee on 4/23/25.
//

namespace opendxf {
    enum object_type {
        POINT,
        LINE,
        OBJECT_TYPE_MAX_VALUE
    };

    struct vec2 {
        float x;
        float y;
    };

    struct object {
        object_type type;
        vec2 pos_a;
        vec2 pos_b;
    };

    int run();
}