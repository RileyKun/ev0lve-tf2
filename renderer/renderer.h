//
// Created by panzerfaust on 5/11/2020.
//

#ifndef EV0LVE_RENDERER_RENDERER_H
#define EV0LVE_RENDERER_RENDERER_H

#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>

#define _USE_MATH_DEFINES
#include <math.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_STROKER_H

#include "../misc/joaat.h"
#include "../types/types.h"

#pragma comment(lib, "FreeType.lib")

namespace renderer {
    struct vertex {
        types::vec2 pos;
        types::vec2 uv;
        uint32_t col = 0;
    };

    struct vertex_constant_buffer {
        float mvp[4][4]{};
    };

    class gpu_command {
    public:
        std::vector<vertex> vertices;
        std::vector<uint16_t> indices;
        void *texture = nullptr;
        types::rect clip;
    };

    enum rounding : uint32_t {
        ROUNDING_TL = 1U,
        ROUNDING_TR = 2U,
        ROUNDING_BR = 4U,
        ROUNDING_BL = 8U,
        ROUNDING_T = ROUNDING_TL | ROUNDING_TR,
        ROUNDING_B = ROUNDING_BL | ROUNDING_BR,
        ROUNDING_L = ROUNDING_TL | ROUNDING_BL,
        ROUNDING_R = ROUNDING_TR | ROUNDING_BR,
        ROUNDING_ALL = ROUNDING_T | ROUNDING_B
    };

    class draw_layer {
    public:
        std::vector<gpu_command> commands;

        void push_clip_rect(const types::rect& r) { clip_rects.emplace_back(clip_rects.empty() ? r : clip_rects.back().intersect(r)); }
        void pop_clip_rect() { clip_rects.erase(clip_rects.end() - 1); }
        void push_uv_rect(const types::rect& r) { uvs.emplace_back(r); }
        void pop_uv_rect() { uvs.erase(uvs.end() - 1); }
        void push_texture(void* t) { textures.emplace_back(t); }
        void pop_texture() { textures.erase(textures.end() - 1); }
        void push_alpha(float a) { alpha_modifiers.emplace_back(a); }
        void pop_alpha() { alpha_modifiers.erase(alpha_modifiers.end() - 1); }

        void add_rect(const types::rect& r, const types::color& col, float thickness = 1.f);
        void add_rect_multicolor(const types::rect& r, const types::color& lt, const types::color& rt, const types::color& rb, const types::color& lb, float thickness = 1.f);
        void add_rect_rounded(const types::rect& r, const types::color& col, float factor, rounding rnd = rounding::ROUNDING_ALL, float thickness = 1.f);
        void add_rect_filled(const types::rect& r, const types::color& col);
        void add_rect_filled_multicolor(const types::rect& r, const types::color& lt, const types::color& rt, const types::color& rb, const types::color& lb);
        void add_rect_filled_rounded(const types::rect& r, const types::color& col, float factor, uint32_t rnd = ROUNDING_ALL);

        void add_quad(const types::rect& a, const types::rect& b, const types::color& col, float thickness = 1.f);
        void add_quad_multicolor(const types::rect& a, const types::rect& b, const types::color& lt, const types::color& rt, const types::color& rb, const types::color& lb, float thickness = 1.f);
        void add_quad_filled(const types::rect& a, const types::rect& b, const types::color& col);
        void add_quad_filled_multicolor(const types::rect& a, const types::rect& b, const types::color& lt, const types::color& rt, const types::color& rb, const types::color& lb);

        void add_circle(const types::vec2& c, float r, const types::color& col, float thickness = 1.f, int segments = 12, float fill = 1.f, float rot = 0.f);
        void add_circle_filled(const types::vec2& c, float r, const types::color& col, int segments = 12, float fill = 1.f, float rot = 0.f);
        void add_circle_filled_multicolor(const types::vec2& c, float r, const types::color& col_a, const types::color& col_b, int segments = 12, float fill = 1.f, float rot = 0.f);

        void add_triangle(const types::vec2& a, const types::vec2& b, const types::vec2& c, const types::color& col, float thickness = 1.f);
        void add_triangle_multicolor(const types::vec2& a, const types::vec2& b, const types::vec2& c, const types::color& t, const types::color& r, const types::color& l, float thickness = 1.f);
        void add_triangle_filled(const types::vec2& a, const types::vec2& b, const types::vec2& c, const types::color& col);
        void add_triangle_filled_multicolor(const types::vec2& a, const types::vec2& b, const types::vec2& c, const types::color& t, const types::color& r, const types::color& l);

        void add_line(const types::vec2& a, const types::vec2& b, const types::color& col, float thickness = 1.f);
        void add_line_multicolor(const types::vec2& a, const types::vec2& b, const types::color& col_a, const types::color& col_b, float thickness = 1.f);

        void add_text(rage::joaat_t id, const std::string& str, const types::vec2& p, const types::color& col);
    private:
        std::vector<types::rect> clip_rects;
        std::vector<types::rect> uvs;
        std::vector<void*> textures;
        std::vector<float> alpha_modifiers;
    };

    class glyph {
    public:
        glyph() = default;
        glyph(void* _data, uint32_t size);
        void release();

        void bake();
        void destroy();

        types::vec2 pos;
        types::vec2 size;
        float advance = 0.f;

        uint32_t tex_pitch = 0;
        uint32_t tex_width = 0;
        uint32_t tex_rows = 0;
        void* texture = nullptr;
    private:
        void* data = nullptr;

        bool is_baked = false;
    };

    class font {
    public:
        font() = default;
        font(const std::string& file, float height, uint16_t low = 0x00, uint16_t high = 0xFF, bool outline = false);
        font(void* arr, uint32_t size, float height, uint16_t low = 0x00, uint16_t high = 0xFF, bool outline = false);
        void release();

        void destroy();
        void bake();

        float font_height = 0.f;
        float glyph_height = 0.f;

        std::unordered_map<uint32_t, glyph> glyphs;
    private:
        void open_file(const std::string& file);
        void open_memory(void* arr, uint32_t size);

        void init();

        uint16_t low = 0;
        uint16_t high = 0;

        bool is_outlined = false;

        FT_Library ft = nullptr;
        FT_Face face = nullptr;
    };

    class tools {
    public:
        static inline bool get_bit(uint32_t N, uint32_t V) {
            return (V >> N) & 1;
        }

        static inline void set_bit(uint32_t N, uint32_t& V) {
            V |= 1 << N;
        }

        static inline void unset_bit(uint32_t N, uint32_t& V) {
            V &= ~(1 << N);
        }

        static uint32_t utf8_decode(const char* str, uint32_t& codepoint);
        static types::vec2 text_size(rage::joaat_t id, const std::string& str);
        static uint32_t upper_pow2(uint32_t v);
    };

    class adapter_base {
    public:
        virtual void create_objects() {}
        virtual void destroy_objects() {
            for (auto& f : fonts)
                f.second.destroy();
        }

        virtual void prepare() {
            refresh();
        }

        virtual void render() {}
        virtual void end() {
            background.commands.clear();
            foreground.commands.clear();
        }

        inline void refresh() {
            RECT client_rect{};
            GetClientRect(window, &client_rect);

            display_size.x = (float)client_rect.right - client_rect.left;
            display_size.y = (float)client_rect.bottom - client_rect.top;
        }

        virtual void* make_texture_file(const std::string& path) { return nullptr; }
        virtual void* make_texture_memory(void* arr, uint32_t size) { return nullptr; }
        virtual void* make_texture_raw(void* arr, uint32_t w, uint32_t h, uint32_t p) { return nullptr; }
        virtual void destroy_texture(void* _texture) {}

        std::unordered_map<rage::joaat_t, font> fonts;

        draw_layer background;
        draw_layer foreground;

        types::vec2 display_size;
        HWND window = nullptr;

        void* texture = nullptr;
    };

    inline adapter_base* adapter = nullptr;
}

#endif //EV0LVE_RENDERER_RENDERER_H
