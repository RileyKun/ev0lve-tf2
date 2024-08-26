//
// Created by panzerfaust on 5/11/2020.
//
#include "renderer.h"
#include "tools_math.h"

void renderer::draw_layer::add_rect_filled(const types::rect& r, const types::color &col) {
    add_quad_filled(types::rect(r.mins, types::vec2(r.maxs.x, r.mins.y)), types::rect(r.maxs, types::vec2(r.mins.x, r.maxs.y)), col);
}

void renderer::draw_layer::add_line(const types::vec2& a, const types::vec2& b, const types::color& col, float thickness) {
    auto dx = b.x - a.x;
    auto dy = b.y - a.y;

    auto inv = 1.f / sqrtf(dx * dx + dy * dy);
    dx *= inv;
    dy *= inv;

    dx *= thickness * 0.5f;
    dy *= thickness * 0.5f;

    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
            types::vec2(a.x + dy, a.y - dx),
            types::vec2(),
            (uint32_t)col.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            types::vec2(b.x + dy, b.y - dx),
            types::vec2(),
            (uint32_t)col.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            types::vec2(b.x - dy, b.y + dx),
            types::vec2(),
            (uint32_t)col.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            types::vec2(a.x - dy, a.y + dx),
            types::vec2(),
            (uint32_t)col.clamp_a(alpha_mod)
    });

    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(1);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(3);

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

void renderer::draw_layer::add_rect(const types::rect& r, const types::color& col, float thickness) {
    add_quad(types::rect(r.mins, types::vec2(r.maxs.x, r.mins.y)), types::rect(r.maxs, types::vec2(r.mins.x + 1.f, r.maxs.y)), col, thickness);
}

void renderer::draw_layer::add_quad_filled(const types::rect& a, const types::rect& b, const types::color& col) {
    auto uv = uvs.empty() ? types::rect(0.f, 0.f, 1.f, 1.f) : uvs.back();

    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
            a.mins,
            uv.tl(),
            (uint32_t)col.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            a.maxs,
            uv.tr(),
            (uint32_t)col.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            b.mins,
            uv.br(),
            (uint32_t)col.clamp_a(alpha_mod)
    });cmd.vertices.emplace_back(vertex {
            b.maxs,
            uv.bl(),
            (uint32_t)col.clamp_a(alpha_mod)
    });

    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(1);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(3);

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

void renderer::draw_layer::add_circle_filled(const types::vec2 &c, float r, const types::color &col, int segments, float fill, float rot) {
    auto uv = uvs.empty() ? types::rect(0.f, 0.f, 1.f, 1.f) : uvs.back();
    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
        c,
        uv.middle(),
        (uint32_t)col.clamp_a(alpha_mod)
    });

    auto delta = ((float)M_PI * fill) * 2.f / (float)segments;
    auto offset = delta * 0.5f + DEG2RAD(rot);

    auto index = 1;
    for (auto i = 0; i < segments; i++) {
        auto pos_a = c + types::vec2(r * cosf(offset - delta * 0.5f), r * sinf(offset - delta * 0.5f));
        auto pos_b = c + types::vec2(r * cosf(offset + delta * 0.5f), r * sinf(offset + delta * 0.5f));

        cmd.vertices.emplace_back(vertex {
                pos_a,
                uv.circular(offset - delta * 0.5f),
                (uint32_t)col.clamp_a(alpha_mod)
        });
        cmd.vertices.emplace_back(vertex {
                pos_b,
                uv.circular(offset + delta * 0.5f),
                (uint32_t)col.clamp_a(alpha_mod)
        });

        cmd.indices.emplace_back(0);
        cmd.indices.emplace_back(index);
        cmd.indices.emplace_back(index + 1);

        offset += delta;
        index += 2;
    }

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

void renderer::draw_layer::add_circle(const types::vec2 &c, float r, const types::color &col, float thickness,
                                      int segments, float fill, float rot) {
    auto delta = ((float)M_PI * fill) * 2.f / (float)segments;
    auto offset = delta * 0.5f + DEG2RAD(rot);

    auto index = 1;
    for (auto i = 0; i < segments; i++) {
        auto pos_a = c + types::vec2(r * cosf(offset - delta * 0.5f), r * sinf(offset - delta * 0.5f));
        auto pos_b = c + types::vec2(r * cosf(offset + delta * 0.5f), r * sinf(offset + delta * 0.5f));

        add_line(pos_a, pos_b, col, thickness);

        offset += delta;
        index += 2;
    }
}

void
renderer::draw_layer::add_rect_multicolor(const types::rect &r, const types::color &lt, const types::color &rt,
                                          const types::color &rb, const types::color &lb, float thickness) {
    add_quad_multicolor(
            types::rect(r.mins, types::vec2(r.maxs.x, r.mins.y)),
            types::rect(r.maxs, types::vec2(r.mins.x + 1.f, r.maxs.y)),
            lt, rt, rb, lb, thickness);
}

void renderer::draw_layer::add_line_multicolor(const types::vec2& a, const types::vec2& b, const types::color& col_a, const types::color& col_b,
                                               float thickness) {
    auto dx = b.x - a.x;
    auto dy = b.y - a.y;

    auto sqr = dx * dx + dy * dy;
    if (sqr > 0.f) {
        auto inv = 1.f / sqrtf(sqr);
        dx *= inv;
        dy *= inv;
    }

    dx *= (thickness * 0.5f);
    dy *= (thickness * 0.5f);

    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
            types::vec2(a.x + dy, a.y - dx),
            types::vec2(),
            (uint32_t)col_a.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            types::vec2(b.x + dy, b.y - dx),
            types::vec2(),
            (uint32_t)col_b.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            types::vec2(b.x - dy, b.y + dx),
            types::vec2(),
            (uint32_t)col_b.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            types::vec2(a.x - dy, a.y + dx),
            types::vec2(),
            (uint32_t)col_a.clamp_a(alpha_mod)
    });

    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(1);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(3);

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

void renderer::draw_layer::add_quad_filled_multicolor(const types::rect &a, const types::rect &b,
                                                      const types::color &lt, const types::color &rt,
                                                      const types::color &rb, const types::color &lb) {
    auto uv = uvs.empty() ? types::rect(0.f, 0.f, 1.f, 1.f) : uvs.back();
    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
            a.mins,
            uv.tl(),
            (uint32_t)lt.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            a.maxs,
            uv.tr(),
            (uint32_t)rt.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            b.mins,
            uv.br(),
            (uint32_t)rb.clamp_a(alpha_mod)
    });cmd.vertices.emplace_back(vertex {
            b.maxs,
            uv.bl(),
            (uint32_t)lb.clamp_a(alpha_mod)
    });

    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(1);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(2);
    cmd.indices.emplace_back(3);

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

void renderer::draw_layer::add_rect_filled_multicolor(const types::rect &r, const types::color &lt,
                                                      const types::color &rt, const types::color &rb,
                                                      const types::color &lb) {
    add_quad_filled_multicolor(types::rect(r.mins, types::vec2(r.maxs.x, r.mins.y)), types::rect(r.maxs, types::vec2(r.mins.x, r.maxs.y)),
            lt, rt, rb, lb);
}

void renderer::draw_layer::add_quad(const types::rect &a, const types::rect &b, const types::color &col,
                                    float thickness) {
    add_line(a.mins, a.maxs, col, thickness);
    add_line(a.maxs, b.mins, col, thickness);
    add_line(b.maxs, b.mins, col, thickness);
    add_line(a.mins, b.maxs, col, thickness);
}

void
renderer::draw_layer::add_quad_multicolor(const types::rect &a, const types::rect &b, const types::color &lt,
                                          const types::color &rt, const types::color &rb,
                                          const types::color &lb, float thickness) {
    add_line_multicolor(a.mins, a.maxs, lt, rt, thickness);
    add_line_multicolor(a.maxs, b.mins, rt, rb, thickness);
    add_line_multicolor(b.maxs, b.mins, lb, rb, thickness);
    add_line_multicolor(a.mins, b.maxs, lt, lb, thickness);
}

void renderer::draw_layer::add_circle_filled_multicolor(const types::vec2 &c, float r, const types::color &col_a,
                                                        const types::color &col_b, int segments, float fill,
                                                        float rot) {
    auto uv = uvs.empty() ? types::rect(0.f, 0.f, 1.f, 1.f) : uvs.back();
    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
            c,
            uv.middle(),
            (uint32_t)col_b.clamp_a(alpha_mod)
    });

    auto delta = ((float)M_PI * fill) * 2.f / (float)segments;
    auto offset = delta * 0.5f + DEG2RAD(rot);

    auto index = 1;
    for (auto i = 0; i < segments; i++) {
        auto pos_a = c + types::vec2(r * cosf(offset - delta * 0.5f), r * sinf(offset - delta * 0.5f));
        auto pos_b = c + types::vec2(r * cosf(offset + delta * 0.5f), r * sinf(offset + delta * 0.5f));

        cmd.vertices.emplace_back(vertex {
                pos_a,
                uv.circular(offset - delta * 0.5f),
                (uint32_t)col_a.clamp_a(alpha_mod)
        });
        cmd.vertices.emplace_back(vertex {
                pos_b,
                uv.circular(offset + delta * 0.5f),
                (uint32_t)col_a.clamp_a(alpha_mod)
        });

        cmd.indices.emplace_back(0);
        cmd.indices.emplace_back(index);
        cmd.indices.emplace_back(index + 1);

        offset += delta;
        index += 2;
    }

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

void renderer::draw_layer::add_rect_rounded(const types::rect &r, const types::color &col, float factor,
        renderer::rounding rnd, float thickness) {
    if (rnd & ROUNDING_TL)
        add_circle(r.mins + factor - 1.f, factor, col, thickness, 12, 0.25f, 180.f);
    if (rnd & ROUNDING_TR)
        add_circle(r.mins + types::vec2(r.width() - factor, factor - 1.f), factor, col, thickness, 12, 0.25f, 270.f);
    if (rnd & ROUNDING_BR)
        add_circle(r.maxs - factor, factor, col, thickness, 12, 0.25f, 0.f);
    if (rnd & ROUNDING_BL)
        add_circle(r.mins + types::vec2(factor - 1.f, r.height() - factor), factor, col, thickness, 12, 0.25f, 90.f);

    add_line(
            r.mins + types::vec2(rnd & ROUNDING_TL ? factor - 1.f : 0.f, 0.f),
            r.mins + types::vec2(r.width() - (rnd & ROUNDING_TR ? factor : 0.f), 0.f),
            col, thickness
            );
    add_line(
            r.mins + types::vec2(r.width(), rnd & ROUNDING_TR ? factor - 1.f : 0.f),
            r.maxs - types::vec2(0.f, rnd & ROUNDING_BR ? factor : 0.f),
            col, thickness
            );
    add_line(
            r.mins + types::vec2(rnd & ROUNDING_BL ? factor - 1.f : 0.f, r.height()),
            r.maxs - types::vec2(rnd & ROUNDING_BR ? factor : 0.f, 0.f),
            col, thickness
            );
    add_line(
            r.mins + types::vec2(0.f, rnd & ROUNDING_TL ? factor - 1.f : 0.f),
            r.mins + types::vec2(0.f, r.height() - (rnd & ROUNDING_BL ? factor : 0.f)),
            col, thickness
            );
}

void renderer::draw_layer::add_rect_filled_rounded(const types::rect &r, const types::color &col, float factor,
                                                   uint32_t rnd) {
    if (rnd & ROUNDING_TL) {
        push_uv_rect(types::rect(0.f, 0.f, (factor / r.width()) * 2.f, (factor / r.height()) * 2.f));
        add_circle_filled(r.mins + factor, factor, col, 12, 0.25f, 180.f);
        pop_uv_rect();
    }

    if (rnd & ROUNDING_TR) {
        push_uv_rect(types::rect(1.f - (factor / r.width()) * 2.f, 0.f, 1.f, (factor / r.height()) * 2.f));
        add_circle_filled(r.mins + types::vec2(r.width() - factor, factor), factor, col, 12, 0.25f, 270.f);
        pop_uv_rect();
    }

    if (rnd & ROUNDING_BR) {
        push_uv_rect(types::rect(1.f - (factor / r.width()) * 2.f, 1.f - (factor / r.height()) * 2.f, 1.f, 1.f));
        add_circle_filled(r.maxs - factor, factor, col, 12, 0.25f, 0.f);
        pop_uv_rect();
    }

    if (rnd & ROUNDING_BL) {
        push_uv_rect(types::rect(0.f, 1.f - (factor / r.height()) * 2.f, (factor / r.width()) * 2.f, 1.f));
        add_circle_filled(r.mins + types::vec2(factor, r.height() - factor), factor, col, 12, 0.25f, 90.f);
        pop_uv_rect();
    }

    push_uv_rect(types::rect(factor / r.width(), factor / r.height(), 1.f - factor / r.width(), 1.f - factor / r.height()));
    add_rect_filled(types::rect(r.mins + factor, r.maxs - factor), col);
    pop_uv_rect();

    push_uv_rect(types::rect(rnd & ROUNDING_TL ? factor / r.width() : 0.f, 0.f, 1.f - (rnd & ROUNDING_TR ? factor / r.width() : 0.f), factor / r.height()));
    add_rect_filled(types::rect(
            r.mins + types::vec2(rnd & ROUNDING_TL ? factor : 0.f, 0.f),
            r.mins + types::vec2(r.width() - (rnd & ROUNDING_TR ? factor : 0.f), factor)
            ), col);
    pop_uv_rect();

    push_uv_rect(types::rect(1.f - (factor / r.width()), factor / r.height(), 1.f, 1.f - (factor / r.height())));
    add_rect_filled(types::rect(
            r.mins + types::vec2(r.width() - factor, factor),
            r.maxs - types::vec2(0.f, factor)
            ), col);
    pop_uv_rect();

    push_uv_rect(types::rect(rnd & ROUNDING_BL ? factor / r.width() : 0.f, 1.f - (factor / r.height()), 1.f - (rnd & ROUNDING_BR ? factor / r.width() : 0.f), 1.f));
    add_rect_filled(types::rect(
            r.mins + types::vec2(rnd & ROUNDING_BL ? factor : 0.f, r.height() - factor),
            r.maxs - types::vec2(rnd & ROUNDING_BR ? factor : 0.f, 0.f)
            ), col);
    pop_uv_rect();

    push_uv_rect(types::rect(0.f, factor / r.height(), factor / r.width(), 1.f - (factor / r.height())));
    add_rect_filled(types::rect(
            r.mins + types::vec2(0.f, factor),
            r.mins + types::vec2(factor, r.height() - factor)
            ), col);
    pop_uv_rect();
}

void renderer::draw_layer::add_text(rage::joaat_t id, const std::string& str, const types::vec2& p, const types::color& col) {
    auto f = &adapter->fonts[id];

    auto offset = types::vec2(0.f, f->glyph_height);
    for (auto i = 0; i < str.length(); ) {
        if (str[i] == ' ') {
            offset += types::vec2(f->font_height * 0.25f, 0.f);
            i++;

            continue;
        }

        uint32_t codepoint = 0;
        i += tools::utf8_decode(str.data() + i, codepoint);

        auto g = &f->glyphs[codepoint];
        if (!g->texture)
            continue;

        push_texture(g->texture);
        add_rect_filled(types::rect((p + g->pos + offset).floor(), (p + g->pos + offset + g->size).floor()), col);
        pop_texture();

        offset += types::vec2(g->size.x + g->advance, 0.f);
    }
}

void renderer::draw_layer::add_triangle(const types::vec2 &a, const types::vec2 &b, const types::vec2 &c,
                                        const types::color &col, float thickness) {
    add_line(c, b, col, thickness);
    add_line(a, b, col, thickness);
    add_line(a, c, col, thickness);
}

void
renderer::draw_layer::add_triangle_multicolor(const types::vec2 &a, const types::vec2 &b, const types::vec2 &c,
                                              const types::color &t, const types::color &r,
                                              const types::color &l, float thickness) {
    add_line_multicolor(c, b, t, r, thickness);
    add_line_multicolor(a, b, l, r, thickness);
    add_line_multicolor(a, c, l, t, thickness);
}

void
renderer::draw_layer::add_triangle_filled(const types::vec2 &a, const types::vec2 &b, const types::vec2 &c,
                                          const types::color &col) {
    auto uv = uvs.empty() ? types::rect(0.f, 0.f, 1.f, 1.f) : uvs.back();
    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
            c,
            types::vec2(uv.width() * 0.5f, uv.mins.y),
            (uint32_t)col.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            a,
            uv.bl(),
            (uint32_t)col.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            b,
            uv.br(),
            (uint32_t)col.clamp_a(alpha_mod)
    });

    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(1);
    cmd.indices.emplace_back(2);

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

void renderer::draw_layer::add_triangle_filled_multicolor(const types::vec2 &a, const types::vec2 &b,
                                                          const types::vec2 &c, const types::color &t,
                                                          const types::color &r, const types::color &l) {
    auto uv = uvs.empty() ? types::rect(0.f, 0.f, 1.f, 1.f) : uvs.back();
    auto alpha_mod = alpha_modifiers.empty() ? 1.f : alpha_modifiers.back();

    gpu_command cmd{};
    cmd.vertices.emplace_back(vertex {
            c,
            types::vec2(uv.width() * 0.5f, uv.mins.y),
            (uint32_t)t.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            a,
            uv.bl(),
            (uint32_t)l.clamp_a(alpha_mod)
    });
    cmd.vertices.emplace_back(vertex {
            b,
            uv.br(),
            (uint32_t)r.clamp_a(alpha_mod)
    });

    cmd.indices.emplace_back(0);
    cmd.indices.emplace_back(1);
    cmd.indices.emplace_back(2);

    cmd.clip = clip_rects.empty() ? types::rect(types::vec2(), adapter->display_size) : clip_rects.back();
    cmd.texture = textures.empty() ? adapter->texture : textures.back();

    commands.emplace_back(cmd);
}

renderer::font::font(const std::string &file, float height, uint16_t _low, uint16_t _high, bool outline) {
    font_height = height;
    low = _low;
    high = _high;
    is_outlined = outline;

    FT_Init_FreeType(&ft);
    open_file(file);
    init();
    FT_Done_FreeType(ft);
}

renderer::font::font(void *arr, uint32_t size, float height, uint16_t _low, uint16_t _high, bool outline) {
    font_height = height;
    low = _low;
    high = _high;
    is_outlined = outline;

    FT_Init_FreeType(&ft);
    open_memory(arr, size);
    init();
    FT_Done_FreeType(ft);
}

void renderer::font::open_file(const std::string &file) {
    FT_New_Face(ft, file.c_str(), 0, &face);
}

void renderer::font::open_memory(void *arr, uint32_t size) {
    FT_New_Memory_Face(ft, reinterpret_cast<FT_Byte*>(arr), size, 0, &face);
}

void renderer::font::init() {
    if (FT_Set_Char_Size(face, 0, font_height * 64.f, 0, 0))
        return;

    FT_Stroker stroker;
    FT_Stroker_New(ft, &stroker);
    FT_Stroker_Set(stroker, 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

    glyph_height = face->height >> 6;
    for (auto i = low; i < high; i++) {
        auto slot = face->glyph;

        auto char_code = 0U;
        if (!(char_code = FT_Get_Char_Index(face, i)))
            continue;
        if (FT_Load_Glyph(face, char_code, 0))
            continue;

        if (!is_outlined) {
            if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL))
                continue;

            auto bmp = slot->bitmap;
            if (bmp.pitch < 0)
                bmp.pitch *= -1;

            if (bmp.pitch == 0)
                continue;

            glyph g(bmp.buffer, bmp.pitch * bmp.rows);
            g.tex_pitch = bmp.pitch;
            g.tex_width = bmp.width;
            g.tex_rows = bmp.rows;
            g.pos.x = slot->bitmap_left;
            g.pos.y = -slot->bitmap_top;
            g.size.x = slot->metrics.width >> 6;
            g.size.y = slot->metrics.height >> 6;
            g.advance = (slot->advance.x >> 6) - g.size.x;

            glyphs[(wchar_t)i] = g;
        } else {
            FT_Glyph ft_glyph;
            FT_Get_Glyph(face->glyph, &ft_glyph);
            FT_Glyph_StrokeBorder(&ft_glyph, stroker, false, true);
            FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, nullptr, true);

            auto bmp = reinterpret_cast<FT_BitmapGlyph>(ft_glyph);
            if (bmp->bitmap.pitch < 0)
                bmp->bitmap.pitch *= -1;

            if (bmp->bitmap.pitch == 0)
                continue;

            glyph g(bmp->bitmap.buffer, bmp->bitmap.pitch * bmp->bitmap.rows);
            g.tex_pitch = bmp->bitmap.pitch;
            g.tex_width = bmp->bitmap.width;
            g.tex_rows = bmp->bitmap.rows;
            g.pos.x = bmp->left;
            g.pos.y = -bmp->top;
            g.size.x = (slot->metrics.width >> 6) + 2.f;
            g.size.y = (slot->metrics.height >> 6) + 2.f;
            g.advance = (slot->advance.x >> 6) - g.size.x;

            glyphs[(wchar_t)i] = g;
        }
    }

    bake();
}

void renderer::font::release() {
    for (auto& g : glyphs)
        g.second.release();
    glyphs.clear();
}

void renderer::font::destroy() {
    for (auto& g : glyphs)
        g.second.destroy();
}

void renderer::font::bake() {
    for (auto& g : glyphs)
        g.second.bake();
}

renderer::glyph::glyph(void *_data, uint32_t _size) {
    data = malloc(_size);
    memcpy(data, _data, _size);
}

void renderer::glyph::release() {
    free(data);
    is_baked = false;
}

void renderer::glyph::bake() {
    auto bmp = (char*)malloc(tex_pitch * tex_rows * 4);
    memset(bmp, 255, tex_pitch * tex_rows * 4);

    for (auto y = 0; y < tex_rows; y++) {
        for (auto x = 0; x < tex_pitch; x++)
            bmp[4 * (x + y * tex_pitch) + 3] = ((char *) data)[x + y * tex_pitch];
    }

    texture = adapter->make_texture_raw(bmp, tex_width, tex_rows, tex_pitch * 4);
    is_baked = true;
}

void renderer::glyph::destroy() {
    adapter->destroy_texture(texture);
    is_baked = false;
}

uint32_t renderer::tools::utf8_decode(const char *str, uint32_t &codepoint) {
    static auto set_octet = [](char _, uint32_t& c, uint32_t o, uint32_t m) {
        for (auto i = 0; i < m; i++) {
            if (get_bit(i, _))
                set_bit(i + o, c);
        }
    };

    codepoint = 0;
    if (!get_bit(7 ,str[0])) {
        codepoint = str[0];
        return 1;
    }

    if (!get_bit(6 ,str[0]))
        return 1;

    if (!get_bit(5 ,str[0])) {
        set_octet(str[1], codepoint, 0, 6);
        set_octet(str[0], codepoint, 6, 5);

        return 2;
    }

    if (!get_bit(4 ,str[0])) {
        set_octet(str[2], codepoint, 0, 6);
        set_octet(str[1], codepoint, 6, 6);
        set_octet(str[0], codepoint, 12, 4);

        return 3;
    }

    if (!get_bit(3 ,str[0])) {
        set_octet(str[3], codepoint, 0, 6);
        set_octet(str[2], codepoint, 6, 6);
        set_octet(str[1], codepoint, 12, 6);
        set_octet(str[0], codepoint, 18, 3);

        return 4;
    }

    return 1;
}

types::vec2 renderer::tools::text_size(rage::joaat_t id, const std::string &str) {
    auto f = &adapter->fonts[id];

    auto offset = types::vec2(0.f, f->glyph_height);
    for (auto i = 0; i < str.size(); ) {
        if (str[i] == ' ') {
            offset += types::vec2(f->font_height * 0.25f, 0.f);
            i++;

            continue;
        }

        uint32_t codepoint = 0;
        i += tools::utf8_decode(str.data() + i, codepoint);

        auto g = &f->glyphs[codepoint];
        if (!g->texture)
            continue;

        offset += types::vec2(g->size.x + g->advance, 0.f);
    }

    return offset;
}

uint32_t renderer::tools::upper_pow2(uint32_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}
