#pragma once
#include "../API/defs.h"
#include <Image.h>
#include "IRendererDefs.h"

namespace OIV
{
    class IRenderer
    {
    public:
        virtual int Init(std::size_t container) = 0;
        virtual int SetViewParams(const ViewParameters& viewParams) = 0;
        virtual int Redraw() = 0;
        virtual int SetFilterLevel(OIV_Filter_type filterType) = 0;
        virtual int SetExposure(const OIV_CMD_ColorExposure_Request& exposure) = 0;
        virtual int SetSelectionRect(SelectionRect selectionRect) = 0;


        //Multi image API
        virtual int SetImageBuffer(uint32_t id, const IMCodec::ImageSharedPtr image) = 0;
        virtual int SetImageProperties(uint32_t id, const ImageProperties& properties) = 0;
        virtual int RemoveImage(uint32_t id) = 0;

        virtual ~IRenderer() {}
    };

    typedef std::shared_ptr<IRenderer> IRendererSharedPtr;
}    