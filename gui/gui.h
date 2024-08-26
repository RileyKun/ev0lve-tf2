//
// Created by panzerfaust on 5/2/2020.
//

#ifndef RENDERER_TEST_GUI_H
#define RENDERER_TEST_GUI_H

#include <string>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <functional>

#include "../renderer/renderer.h"
#include "../renderer/adapter_dx9.h"

#include "../input/input.h"
#include "../misc/joaat.h"
#include "../types/types.h"

#include "data/game.h"
#include "data/product.h"

namespace gui {
    class anim;
    class anim_color;
    class anim_val;
    class element;
    class layout;
    class tab;
    class title_tab;
    class content_tab;
    class large_window;
    class small_window;
    class window;
    class context;
    class group;
    class checkbox;
    class button;
    class text_label;
    class progressbar;
    class slider;
    class combobox;
    class color_picker;
    class hotkey;
    class text_input;

    enum class gui_type {
        BASE,
        TAB_TITLE,
        TAB_CONTENT,
        WINDOW_LARGE,
        WINDOW_SMALL,
        LAYOUT,
        LAYOUT_STACK,
        LAYOUT_SCROLL,
        CONTROL_GROUP,
        CONTROL_CHECKBOX,
        CONTROL_LABEL,
        CONTROL_LISTBOX,
        CONTROL_LISTBOX_ENTRY,
        CONTROL_BUTTON,
        CONTROL_PROGRESSBAR,
        CONTROL_SLIDER,
        CONTROL_COMBOBOX,
        CONTROL_COLORPICKER,
        CONTROL_HOTKEY,
        CONTROL_TEXT_INPUT,
    };

    enum class stack_direction {
        VERTICAL,
        HORIZONTAL
    };

    /// Internal use only
    class anim {
    public:
        virtual void reset() = 0;
        virtual void animate() = 0;

        static float interpolate(float a, float b, float u) {
            return (1.f - u) * a + u * b;
        }

        float duration = 0.f;
        float progress = 0.f;
    };

    /// Internal use only
    class anim_color : public anim {
    public:
        void reset() override;
        void animate() override;

        inline void init(const _COLOR& v, float d) { begin = end = last = v; duration = d; progress = 1.f; }

        _COLOR begin;
        _COLOR end;
        _COLOR last;
    };

    /// Internal use only
    class anim_val : public anim {
    public:
        void reset() override;
        void animate() override;

        inline void init(float v, float d) { begin = end = last = v; duration = d; progress = 1.f; }

        float begin = 0.f;
        float end = 0.f;
        float last = 0.f;
    };

    /// Internal use only
    class element {
    public:
        virtual void update() {};
        virtual void post_update() {};
        virtual void draw() {};

        [[nodiscard]] bool is_tab() const {
            return
                    type == gui_type::TAB_TITLE ||
                    type == gui_type::TAB_CONTENT;
        }
        [[nodiscard]] bool is_window() const {
            return
                    type == gui_type::WINDOW_LARGE ||
                    type == gui_type::WINDOW_SMALL;
        }
        [[nodiscard]] bool is_layout() const {
            return
                    type == gui_type::LAYOUT ||
                    type == gui_type::LAYOUT_STACK ||
                    type == gui_type::LAYOUT_SCROLL;
        }

        template<typename T>
        inline T* as() { return reinterpret_cast<T*>(this); }

        _RECT rect;

        bool is_active = false;
        bool is_visible = false;
        bool is_in_bounds = false;

        rage::joaat_t id = 0;
        gui_type type = gui_type::BASE;
    };

    class layout : public element {
    public:
        /// Constructor
        /// \param pos Position
        /// \param size Size
        layout(rage::joaat_t _id, const _VEC2& pos, const _VEC2& size);

        void draw() override;

        element* parent = nullptr;
        std::vector<element *> elements;
    };

    class stack_layout : public layout {
    public:
        /// Stacks elements vertically/horizontally
        /// \param _id ID
        /// \param pos Position
        /// \param size Size
        /// \param dir Direction
        stack_layout(rage::joaat_t _id, const _VEC2& pos, const _VEC2& size, gui::stack_direction dir = stack_direction::VERTICAL);

        void update() override;
        void draw() override;

        stack_direction direction = stack_direction::VERTICAL;
    };

    class scroll_layout : public layout {
    public:
        /// Allows user to scroll through whole layout page
        /// \param _id ID
        /// \param pos Position
        /// \param size Size
        scroll_layout(rage::joaat_t _id, const _VEC2& pos, const _VEC2& size);

        void update() override;
        void draw() override;

        anim_val scroller;

        bool need_scroll_bar = false;
        float scroll_size = 0.f;
        float scroll = 0.f;
        float temp_size = 0.f;
    };

    /// Internal use only
    class tab : public element {
    public:
        std::string label;
        bool is_selected = false;

        std::vector<element *> elements;
    };

    /// Internal use only
    class window : public element {
    public:
        bool is_movable = true;

        bool was_visible = false;
        anim_val visible_anim;

        std::vector<element *> elements;
    };

    class title_tab : public tab {
    public:
        /// Constructor
        /// \param _id ID
        /// \param text Label
        /// \param ico Icon
        title_tab(rage::joaat_t _id, const std::string &text, const std::string &ico);

        void update() override;
        void post_update() override;
        void draw() override;

        anim_color fill;
        int old_state = 0;
        std::string icon;

        large_window *current_window;
    };

    class content_tab : public tab {
    public:
        content_tab(rage::joaat_t _id, const std::string& text);

        void update() override;
        void post_update() override;
        void draw() override;

        _VEC2 size;

        anim_color fill;
        anim_val offset;
        int old_state = 0;

        element* parent = nullptr;
    };

    class small_window : public window {
    public:
        /// Constructor
        /// \param _id ID
        /// \param title Title
        /// \param pos Position
        /// \param sz Size
        small_window(rage::joaat_t _id, const std::string& title, const _VEC2& pos, const _VEC2& sz);

        void update() override;
        void post_update() override;
        void draw() override;

        std::string label;

        anim_color cross_color;
        bool is_hovered = false;

        std::function<bool()> callback = nullptr;
    };

    class large_window : public window {
    public:
        /// Constructor
        /// \param _id Element ID
        /// \param pos Position
        /// \param sz Size
        large_window(rage::joaat_t _id, const _VEC2& pos, const _VEC2& sz);

        void update() override;
        void post_update() override;
        void draw() override;

        std::vector<title_tab *> tabs;

        anim_color cross_color;
        bool is_hovered = false;

        std::function<bool()> callback = nullptr;
    };

    class group : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param name Title
        /// \param size Groupbox size
        group(rage::joaat_t _id, const std::string &name, const _VEC2& size);

        void draw() override;

        std::string label;
    };

    class checkbox : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param name Label
        /// \param vptr Value pointer (optional)
        checkbox(rage::joaat_t _id, const std::string &name, bool *vptr = nullptr);

        void update() override;
        void draw() override;

        std::string label;
        bool *value = nullptr;

        std::function<void(bool)> callback = nullptr;

        anim_color outline;
        bool was_hovered = false;
        bool set_rect = false;
    };

    class text_label : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param text Text
        /// \param bold Should text be bold
        /// \param color Text color
        text_label(rage::joaat_t _id, const std::string &text, bool bold = false, const _COLOR& color = 0);

        void update() override;
        void draw() override;

        _COLOR color;
        std::string label;
        bool set_rect = false;
        bool is_bold = false;
    };

    class listbox : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param size Size
        listbox(rage::joaat_t _id, const _VEC2& size);

        void update() override;
        void post_update() override;
        void draw() override;

        rage::joaat_t selected_id = 0;
        std::function<void(rage::joaat_t)> callback = nullptr;

        scroll_layout *background = nullptr;
        stack_layout *elements = nullptr;
    };

    class listbox_entry : public element {
    public:
        void update() override;

        listbox *parent = nullptr;
        bool is_even = true;

        anim_color color;
        anim_val offset;
        int old_state = 0;
    };

    class listbox_text_entry : public listbox_entry {
    public:
        /// Constructor
        /// \param _id ID
        /// \param name Label
        listbox_text_entry(rage::joaat_t _id, const std::string &name);

        void draw() override;

        std::string label;
    };

    class listbox_product_entry : public listbox_entry {
    public:
        /// Constructor
        /// \param _id ID
        /// \param product Product object
        listbox_product_entry(rage::joaat_t _id, const data::product &product);

        void draw() override;

        data::product info;
    };

    class listbox_game_entry : public listbox_entry {
    public:
        /// Constructor
        /// \param _id ID
        /// \param game Game object
        listbox_game_entry(rage::joaat_t _id, const data::game &game);

        void draw() override;

        data::game info;
        void *texture = nullptr;
    };

    class button : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param name Label
        /// \param size Size
        button(rage::joaat_t _id, const std::string &name, const _VEC2& size);

        void update() override;
        void draw() override;

        std::function<void()> callback = nullptr;

        std::string label;
        anim_color color;

        bool is_hovered = false;
        bool is_pressed = false;
    };

    class progressbar : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param from Minimal value
        /// \param to Maximal value
        /// \param size Size
        progressbar(rage::joaat_t _id, float from, float to, const _VEC2& size = _VEC2(164.f, 15.f));

        void draw() override;
        void set(float v);

        anim_val value;
        float mins = 0.f;
        float maxs = 100.f;
    };

    class slider : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param min Minimal value
        /// \param max Maximal value
        /// \param v Value pointer
        /// \param _step Step
        /// \param size Size
        slider(rage::joaat_t _id, float min, float max, float *v = nullptr, float _step = 1.f, const std::string& fmt = "%.0f", const _VEC2& size = _VEC2(164.f, 15.f));

        void update() override;
        void draw() override;

        float* value = nullptr;
        std::function<void(float)> callback = nullptr;

        float mins = 0.f;
        float maxs = 100.f;
        float step = 1.f;

        std::string format = "%.0f";

        anim_color hover_col;
        anim_color grip_col;
        anim_val val_anim;
        bool is_hovered = false;
    };

    class combobox : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param e Elements array
        /// \param v Value
        /// \param ms Is multi selectable
        /// \param size Size
        combobox(rage::joaat_t _id, const std::vector<std::string>& e, _BITS* v = nullptr, bool ms = false, const _VEC2& size = _VEC2(164.f, 25.f));

        void update() override;
        void draw() override;

        _BITS* value = nullptr;
        std::function<void(const _BITS&)> callback = nullptr;

        bool is_multiselect = false;

        std::vector<std::string> elements;
        std::unordered_map<uint32_t, anim_color> color_anim;
        std::unordered_map<uint32_t, anim_val> select_anim;
        std::unordered_map<uint32_t, bool> is_hovered;

        _RECT dropdown;
        anim_val open_anim;

        anim_color hover_col;
        bool hovered = false;
    };

    class color_picker : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param v Value
        /// \param alpha Show alpha bar
        color_picker(rage::joaat_t _id, _COLOR* v = nullptr, bool alpha = true);

        void update() override;
        void draw() override;

        _COLOR old_value = 0;
        _COLOR* value = nullptr;
        std::function<void(const _COLOR&)> callback = nullptr;

        bool enable_alpha = true;
        uint8_t changing = 0;

        _RECT dropdown;
        anim_val open_anim;

        float _h = 0.f;
        float _s = 0.f;
        float _v = 0.f;
    };

    class hotkey : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param v Value
        /// \param b Behavior
        hotkey(rage::joaat_t _id, uint32_t* v = nullptr, uint32_t* b = nullptr);

        void update() override;
        void post_update() override;
        void draw() override;

        uint32_t* value = nullptr;
        uint32_t* behavior = nullptr;

        std::function<void()> callback = nullptr;
        std::function<void()> released = nullptr;

        anim_color hover_col;
        bool is_hovered = false;

        _RECT dropdown;
        anim_val open_anim;

        std::unordered_map<uint32_t, anim_color> color_anim;
        std::unordered_map<uint32_t, bool> hovered;
    };

    class text_input : public element {
    public:
        /// Constructor
        /// \param _id ID
        /// \param v Value
        /// \param size Size
        text_input(rage::joaat_t _id, std::string* v = nullptr, const _VEC2& size = _VEC2(164.f, 25.f));

        void update() override;
        void draw() override;

        std::string* value = nullptr;
        std::function<void(const std::string&)> callback = nullptr;

        anim_color hover_col;
        bool is_hovered = false;

        uint32_t caret = 0;
        uint32_t offset = 0;
    };

    template<typename T>
    inline T *_gui_find_recursive(layout *container, rage::joaat_t id) {
        for (auto e : container->elements) {
            if (e->id == id)
                return (T *) e;

            if (e->is_layout())
            {
                auto t =  _gui_find_recursive<T>((layout *) e, id);
                if (t)
                    return t;
            }

            if (e->type == gui_type::CONTROL_LISTBOX)
            {
                auto t = _gui_find_recursive<T>(((listbox *) e)->elements, id);
                if (t)
                    return t;
            }
        }

        return nullptr;
    }

    class context {
    public:
        void init();
        void draw();

        [[nodiscard]] bool is_hovered(const _RECT& r) const;
        [[nodiscard]] bool is_held() const;
        [[nodiscard]] bool is_released() const;
        [[nodiscard]] bool is_clicked() const;
        [[nodiscard]] bool is_unclicked() const;
        [[nodiscard]] bool is_right_clicked() const;

        template<typename T>
        inline T *find_element(rage::joaat_t id) {
            for (auto w : windows) {
                if (w->id == id)
                    return (T *) w;

                if (w->type == gui_type::WINDOW_LARGE) {
                    auto wl = (large_window *) w;
                    for (auto t : wl->tabs) {
                        if (t->id == id)
                            return (T *) t;

                        for (auto e : t->elements) {
                            if (e->id == id)
                                return (T *) e;

                            if (e->is_layout()) {
                                auto f = _gui_find_recursive<T>((layout *) e, id);
                                if (f)
                                    return f;
                            }

                            if (e->type == gui_type::CONTROL_LISTBOX) {
                                auto f = _gui_find_recursive<T>(((listbox *) e)->elements, id);
                                if (f)
                                    return f;
                            }
                        }
                    }
                }

                for (auto e : w->elements) {
                    if (e->id == id)
                        return (T *) e;

                    if (e->is_layout()) {
                        auto f = _gui_find_recursive<T>((layout *) e, id);
                        if (f)
                            return f;
                    }

                    if (e->type == gui_type::CONTROL_LISTBOX) {
                        auto f = _gui_find_recursive<T>(((listbox *) e)->elements, id);
                        if (f)
                            return f;
                    }
                }
            }

            return nullptr;
        }

        struct {
            _COLOR background;
            _COLOR bottom;
            _COLOR outline;
            _COLOR accent;
            _COLOR text_normal;
            _COLOR text_disabled;
            _COLOR text_alt_1;
            _COLOR text_alt_2;
            _COLOR odd;
            _COLOR even;
            _COLOR green;
            _COLOR red;
            _COLOR yellow;
        } colors;

        void *texture = nullptr;

        rage::joaat_t active_id = 0;
        rage::joaat_t current_id = 0;

        std::vector<window *> windows;
        window *current_window = nullptr;

        bool debug_mode = false;
        float frame_time = 1.f / 60.f;
    };

    extern context *ctx;

    class debug {
    public:
        static void area(const _RECT& r) {
            if (!ctx->debug_mode)
                return;

            renderer::adapter->foreground.add_rect_filled(r, _COLOR(255, 255, 255, 25));
        }
    };
}

#endif //RENDERER_TEST_GUI_H
