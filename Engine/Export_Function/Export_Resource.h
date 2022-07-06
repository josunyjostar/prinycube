#ifndef Export_Resource_h__
#define Export_Resource_h__

#include "ResourceManager.h"
#include "ShaderManager.h"
#include "Texture.h"

BEGIN(Engine)

inline void	Resource_Release();

#include "Export_Resource.inl"

END

#endif // Export_Resource_h__
