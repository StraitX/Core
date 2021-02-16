#include "graphics_pipeline.hpp"

namespace StraitX{

GraphicsPipeline::VTable GraphicsPipeline::s_VTable;

u32 GraphicsPipeline::s_VertexAttributeSizeTable[]={
    1 * sizeof(i32),
    2 * sizeof(i32),
    3 * sizeof(i32),
    4 * sizeof(i32),

    1 * sizeof(u32),
    2 * sizeof(u32),
    3 * sizeof(u32),
    4 * sizeof(u32),

    1 * sizeof(float),
    2 * sizeof(float),
    3 * sizeof(float),
    4 * sizeof(float)
};

}//namespace StraitX::