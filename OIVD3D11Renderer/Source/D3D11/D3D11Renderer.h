#pragma once

#include <Image.h>
#include "D3D11Device.h"
#include "D3D11Shader.h"
#include "D3D11Buffer.h"
#include "D3D11Texture.h"
#include <API/defs.h>
#include <interfaces/IrendererDefs.h>
#include <map>

namespace OIV
{
    const std::string BLANK_STRING;

#pragma pack(1)
    struct VS_CONSTANT_BUFFER_SELECTIONRECT
    {
        int32_t uvViewportSize [4];
        int32_t uSelectionRect[4]; // p0 (x,y) ,  p1 (z,w)
    };

    

    struct CONSTANT_BUFFER_IMAGE_SIMPLE
    {
        float uvViewportSize[2];
        float uImageSize[2];
        float uImageOffset[2];
        float uScale[2];
    };


    struct VS_CONSTANT_BUFFER
    {
        float uvViewportSize[2];
        float uImageSize[2];
        float uImageOffset[2];
        float uScale[2];
        int32_t uShowGrid;
        float exposure;
        float offset;
        float gamma;
    };
#pragma pack()


    enum ImageDisplayMode
    {
          IDM_Default
        , IDM_ImageView
        , IDM_Overlay
    };


    struct ImageEntry
    {
        D3D11TextureSharedPtr texture;
        ImageProperties properties;
    };

    class D3D11Renderer 
    {
    
    public:
         D3D11Renderer();
    public:
        int Init(std::size_t container);
        int SetViewParams(const ViewParameters& viewParams);
        void UpdateGpuParameters();
        int Redraw();
        int SetFilterLevel(OIV_Filter_type filterType);
        int SetselectionRect(const SelectionRect& selection_rect);
        int SetExposure(const OIV_CMD_ColorExposure_Request& exposure);
        int SetImageBuffer(uint32_t id, const IMCodec::ImageSharedPtr& image);
        int SetImageProperties(uint32_t id, const ImageProperties& properties);
        int RemoveImage(uint32_t id);

#pragma region //**** Private methods*****/
    private: 
        void ResizeBackBuffer(int x, int y);
        void CreateD3D11DefaultSamplerState(D3D11_SAMPLER_DESC& sampler);
        void CreateShaders();

        void CreateD3D11DefaultBlendState(D3D11_BLEND_DESC& blend);
        void CreateBuffers();
        void renderOneFrame();
        void UpdateViewportSize(int x, int y);
        void SetDevicestate();
#pragma endregion
    private:
        D3D11DeviceSharedPtr fDevice;
        
        D3D11ShaderUniquePtr fImageVertexShader;
        D3D11ShaderUniquePtr fImageFragmentShader;
        D3D11ShaderUniquePtr fSelectionFragmentShaer;
        D3D11ShaderUniquePtr fImageSimpleFragmentShader;
        bool fIsParamsDirty = true;
        SelectionRect fSelectionRect;
        using MapImageEntry = std::map<uint16_t,ImageEntry>;
        MapImageEntry fImageEntries;

#pragma region /* Direct3D111 resources*/
        D3D11_VIEWPORT fViewport = {0};
        D3D11BufferBoundUniquePtr<VS_CONSTANT_BUFFER_SELECTIONRECT> fBufferSelection;
        D3D11BufferBoundUniquePtr<CONSTANT_BUFFER_IMAGE_SIMPLE> fBufferSimple;
        
        D3D11BufferBoundUniquePtr<VS_CONSTANT_BUFFER> fConstantBuffer;
        
        ComPtr<ID3D11SamplerState>  fSamplerState;
        ComPtr<ID3D11BlendState> fBlendState;
        ComPtr<ID3D11InputLayout> fInputLayout;
        ComPtr<ID3D11Buffer> fVertexBuffer;
        ComPtr<ID3D11RenderTargetView> fRenderTargetView;
#pragma endregion 
    };
}
