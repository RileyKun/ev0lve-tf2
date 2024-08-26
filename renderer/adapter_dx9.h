//
// Created by panzerfaust on 5/24/2020.
//

#ifndef EV0LVE_RENDERER_ADAPTER_DX9_H
#define EV0LVE_RENDERER_ADAPTER_DX9_H
#define WIN32_LEAN_AND_MEAN

#ifdef RENDERER
#error You can include only one renderer at a time!
#else
#define RENDERER
#endif

#include "renderer.h"

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace renderer {
    class adapter_dx9 : public adapter_base {
    public:
        adapter_dx9(HWND _wnd, IDirect3DDevice9* _device);

        void create_objects() override;
        void destroy_objects() override;

        void prepare() override;
        void render() override;

        void* make_texture_file(const std::string& path) override;
        void* make_texture_memory(void* arr, uint32_t size) override;
        void* make_texture_raw(void* arr, uint32_t width, uint32_t height, uint32_t p) override;
        void destroy_texture(void* _texture) override;

    private:
        void render_layer(const draw_layer& dl);
        void make_vertex_buffer();
        void make_index_buffer();

        void clear_vertices();
        void clear_indices();

        void write_vertices(const std::vector<vertex>& v);
        void write_indices(const std::vector<uint16_t>& v);

        bool did_create_objects = false;

        IDirect3DDevice9 *device = nullptr;
        IDirect3DVertexBuffer9 *vertex_buffer = nullptr;
        IDirect3DIndexBuffer9 *index_buffer = nullptr;
        IDirect3DTexture9 *texture_object = nullptr;

        uint32_t vertex_size = 5000;
        uint32_t index_size = 10000;

        uint32_t vertex_off = 0;
        uint32_t index_off = 0;
    };
}

#endif //EV0LVE_RENDERER_ADAPTER_DX9_H
