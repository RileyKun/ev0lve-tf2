//
// Created by panzerfaust on 5/24/2020.
//

#include "adapter_dx9.h"
#include "tools_dx.h"
#include "data_texture.h"

struct dx9_vertex
{
    float    pos[3];
    D3DCOLOR col;
    float    uv[2];
};

renderer::adapter_dx9::adapter_dx9(HWND _wnd, IDirect3DDevice9 *_device) {
    window = _wnd;
    device = _device;
}

void renderer::adapter_dx9::create_objects() {
    if (did_create_objects)
        return;

    if (FAILED(D3DXCreateTextureFromFileInMemoryEx(
            device, base_texture, 119, 1, 1,
            D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
            0, nullptr, nullptr, &texture_object))) {
        printf("[DX9] Failed to create texture\n");
        return;
    }

    did_create_objects = true;
}

void renderer::adapter_dx9::destroy_objects() {
    adapter_base::destroy_objects();

    DX_RELEASE(texture_object)
    DX_RELEASE(vertex_buffer)
    DX_RELEASE(index_buffer)

    did_create_objects = false;
}

void renderer::adapter_dx9::prepare() {
    adapter_base::prepare();

    create_objects();
    if (!did_create_objects)
        return;

    texture = texture_object;
}

void renderer::adapter_dx9::render() {
    adapter_base::render();

    if (!did_create_objects)
        return;

    IDirect3DStateBlock9* state_block = nullptr;
    if (device->CreateStateBlock(D3DSBT_ALL, &state_block) < 0)
        return;

    D3DMATRIX last_world, last_view, last_projection;
    device->GetTransform(D3DTS_WORLD, &last_world);
    device->GetTransform(D3DTS_VIEW, &last_view);
    device->GetTransform(D3DTS_PROJECTION, &last_projection);

    D3DVIEWPORT9 vp{};
    vp.Width = display_size.x;
    vp.Height = display_size.y;
    vp.MaxZ = 1.f;

    device->SetViewport(&vp);
    device->SetPixelShader(nullptr);
    device->SetVertexShader(nullptr);
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    device->SetRenderState(D3DRS_LIGHTING, false);
    device->SetRenderState(D3DRS_ZENABLE, false);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
    device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
    device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    device->SetRenderState(D3DRS_FOGENABLE, false);
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    D3DMATRIX mat_identity = { {
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        } } };

    D3DMATRIX mat_projection = { { {
            2.f / display_size.x,   0.0f,         0.0f,  0.0f,
            0.0f,         2.f / -display_size.y,   0.0f,  0.0f,
            0.0f,         0.0f,         0.5f,  0.0f,
            -1.f, 1.f, 0.5f, 1.f
        } } };

    device->SetTransform(D3DTS_WORLD, &mat_identity);
    device->SetTransform(D3DTS_VIEW, &mat_identity);
    device->SetTransform(D3DTS_PROJECTION, &mat_projection);
    device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    auto total_vertices = 0;
    for (auto& cmd : background.commands)
        total_vertices += cmd.vertices.size() * sizeof(dx9_vertex);
    for (auto& cmd : foreground.commands)
        total_vertices += cmd.vertices.size() * sizeof(dx9_vertex);

    if (!vertex_buffer || total_vertices > vertex_size) {
        if (total_vertices > vertex_size)
            vertex_size = total_vertices + 5000;

        make_vertex_buffer();
    }

    auto total_indices = 0;
    for (auto& cmd : background.commands)
        total_indices += cmd.indices.size() * sizeof(uint16_t);
    for (auto& cmd : foreground.commands)
        total_indices += cmd.indices.size() * sizeof(uint16_t);

    if (!index_buffer || total_indices > index_size) {
        if (total_indices > index_size)
            index_size = total_indices + 10000;

        make_index_buffer();
    }

    clear_indices();
    clear_vertices();

    std::vector<vertex> _vertices;
    _vertices.reserve(vertex_size);

    std::vector<uint16_t> _indices;
    _indices.reserve(index_size);

    for (auto& cmd : background.commands) {
        for (auto& v : cmd.vertices)
            _vertices.emplace_back(v);
        for (auto& i : cmd.indices)
            _indices.emplace_back(i);
    }

    for (auto& cmd : foreground.commands) {
        for (auto& v : cmd.vertices)
            _vertices.emplace_back(v);
        for (auto& i : cmd.indices)
            _indices.emplace_back(i);
    }

    write_vertices(_vertices);
    write_indices(_indices);

    device->SetStreamSource(0, vertex_buffer, 0, sizeof(dx9_vertex));
    device->SetIndices(index_buffer);

    vertex_off = 0;
    index_off = 0;

    render_layer(background);
    render_layer(foreground);

    device->SetTransform(D3DTS_WORLD, &last_world);
    device->SetTransform(D3DTS_VIEW, &last_view);
    device->SetTransform(D3DTS_PROJECTION, &last_projection);

    state_block->Apply();
    state_block->Release();
}

void renderer::adapter_dx9::render_layer(const renderer::draw_layer &dl) {
    for (auto& cmd : dl.commands) {
        RECT scissors = {
                (LONG)cmd.clip.mins.x, (LONG)cmd.clip.mins.y,
                (LONG)cmd.clip.maxs.x, (LONG)cmd.clip.maxs.y
        };

        device->SetTexture(0, (IDirect3DTexture9*)cmd.texture);
        device->SetScissorRect(&scissors);
        device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                vertex_off, 0, cmd.vertices.size(),
                index_off, cmd.indices.size() / 3);

        vertex_off += cmd.vertices.size();
        index_off += cmd.indices.size();
    }
}

void renderer::adapter_dx9::make_vertex_buffer() {
    DX_RELEASE(vertex_buffer)

    if (FAILED(device->CreateVertexBuffer(
            vertex_size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT,
            &vertex_buffer, nullptr
            ))) {
        printf("[DX9] Failed to create vertex buffer\n");
        return;
    }
}

void renderer::adapter_dx9::make_index_buffer() {
    DX_RELEASE(index_buffer)

    if (FAILED(device->CreateIndexBuffer(
            index_size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            D3DFMT_INDEX16, D3DPOOL_DEFAULT, &index_buffer, nullptr
            ))) {
        printf("[DX9] Failed to create index buffer\n");
        return;
    }
}

void *renderer::adapter_dx9::make_texture_file(const std::string &path) {
    IDirect3DTexture9 *_texture = nullptr;
    D3DXCreateTextureFromFileExA(
            device, path.c_str(), D3DX_DEFAULT, D3DX_DEFAULT,
            1, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT,
            D3DX_DEFAULT, 0, nullptr, nullptr, &_texture);

    return _texture;
}

void *renderer::adapter_dx9::make_texture_memory(void *arr, uint32_t size) {
    IDirect3DTexture9 *_texture = nullptr;
    D3DXCreateTextureFromFileInMemoryEx(
            device, arr, size, D3DX_DEFAULT, D3DX_DEFAULT,
            1, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT,
            D3DX_DEFAULT, 0, nullptr, nullptr, &_texture);

    return _texture;
}

void *renderer::adapter_dx9::make_texture_raw(void *arr, uint32_t width, uint32_t height, uint32_t p) {
    IDirect3DTexture9 *_texture = nullptr;
    if (FAILED(device->CreateTexture(
            width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT, &_texture, nullptr)))
        return nullptr;

    D3DLOCKED_RECT lr{};
    if (FAILED(_texture->LockRect(0, &lr, nullptr, 0)))
        return nullptr;

    for (auto y = 0; y < height; y++)
        memcpy((char*)lr.pBits + lr.Pitch * y, (char*)arr + p * y, p);

    _texture->UnlockRect(0);

    return _texture;
}

void renderer::adapter_dx9::destroy_texture(void *_texture) {
    reinterpret_cast<IDirect3DTexture9*>(_texture)->Release();
}

void renderer::adapter_dx9::write_vertices(const std::vector<vertex> &arr) {
    char *data = nullptr;
    if (FAILED(vertex_buffer->Lock(0, vertex_size, (void**)&data, D3DLOCK_DISCARD))) {
        printf("[DX9] Failed to lock vertex buffer\n");
        return;
    }

    for (auto& v : arr) {
        auto _col = types::color(v.col);

        auto _d = (dx9_vertex*)data;
        _d->pos[0] = v.pos.x - 0.5f;
        _d->pos[1] = v.pos.y - 0.5f;
        _d->pos[2] = 0.f;
        _d->uv[0] = v.uv.x;
        _d->uv[1] = v.uv.y;
        _d->col = D3DCOLOR_ARGB(_col.a(), _col.r(), _col.g(), _col.b());

        data += sizeof(dx9_vertex);
    }

    vertex_buffer->Unlock();
}

void renderer::adapter_dx9::write_indices(const std::vector<uint16_t> &arr) {
    char *data = nullptr;
    if (FAILED(index_buffer->Lock(0, index_size, (void**)&data, D3DLOCK_DISCARD))) {
        printf("[DX9] Failed to lock index buffer\n");
        return;
    }

    memcpy(data, arr.data(), arr.size() * sizeof(uint16_t));
    index_buffer->Unlock();
}

void renderer::adapter_dx9::clear_vertices() {
    dx9_vertex *data = nullptr;
    if (FAILED(vertex_buffer->Lock(0, vertex_size, (void**)&data, D3DLOCK_DISCARD))) {
        printf("[DX9] Failed to lock vertex buffer\n");
        return;
    }

    memset(data, 0, vertex_size);
    vertex_buffer->Unlock();
}

void renderer::adapter_dx9::clear_indices() {
    char *data = nullptr;
    if (FAILED(index_buffer->Lock(0, index_size, (void**)&data, D3DLOCK_DISCARD))) {
        printf("[DX9] Failed to lock index buffer\n");
        return;
    }

    memset(data, 0, index_size);
    index_buffer->Unlock();
}