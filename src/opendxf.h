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

        vec2 operator+(const vec2 & b) const {
            return {x + b.x, y + b.y};
        }

        vec2 operator-(const vec2 & b) const {
            return {x - b.x, y - b.y};
        }

        vec2 operator*(const vec2 & b) const {
            return {x * b.x, y * b.y};
        }

        vec2 operator/(const vec2 & b) const {
            return {x / b.x, y / b.y};
        }
    };

    struct vec3 {
        union {
            float x;
            float r;
        };
        union {
            float y;
            float g;
        };
        union {
            float z;
            float b;
        };
    };

    struct object {
        object_type type;
        vec2 pos_a;
        vec2 pos_b;
    };

    int run();
}