#include "MrgRenderingPassShaders.h"
#include "CoreMinimal.h"

IMPLEMENT_SHADER_TYPE(, FMrgLinearizeShaderPS, TEXT("/MRG/Shader/CRT/linearize.usf"), TEXT("MainPS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FMrgBlurHorizontalShaderPS, TEXT("/MRG/Shader/CRT/blur_horiz.usf"), TEXT("MainPS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FMrgBlurVerticalShaderPS, TEXT("/MRG/Shader/CRT/blur_vert.usf"), TEXT("MainPS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FMrgThresholdShaderPS, TEXT("/MRG/Shader/CRT/threshold.usf"), TEXT("MainPS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FMrgBloomShaderPS, TEXT("/MRG/Shader/CRT/bloompass.usf"), TEXT("MainPS"), SF_Pixel);
IMPLEMENT_SHADER_TYPE(, FMrgScanlineShaderPS, TEXT("/MRG/Shader/CRT/scanpass-glow.usf"), TEXT("MainPS"), SF_Pixel);
