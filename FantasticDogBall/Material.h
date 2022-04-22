#include "UncheckedUniformBuffer.h"
#include "Shaders.h"

namespace Material
{

	class Material
	{
	public:

		UncheckedUniformBuffer buffer;
		Shaders::Program program;
		
		Material(Shaders::Program);

		void bind();
	};

	class Static : public Material
	{
	};

	class Texture : public Material
	{
		
	};

	class Procedural : public Material
	{
		
	};

}
