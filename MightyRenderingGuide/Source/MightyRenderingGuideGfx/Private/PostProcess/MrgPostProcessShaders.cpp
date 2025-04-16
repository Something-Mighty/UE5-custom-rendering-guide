#include "MrgPostProcessShaders.h"
#include "CoreMinimal.h"

IMPLEMENT_GLOBAL_SHADER(FMrgShadePostProcessShaderCS, "/MRG/Shader/PostProcessCS.usf", "MainCS", SF_Compute);
