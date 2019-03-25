#pragma once
#include <string>
using std::string;
namespace NCL {
	namespace Rendering {
		enum ShaderStages {
			SHADER_VERTEX,
			SHADER_FRAGMENT,
			SHADER_GEOMETRY,
			SHADER_DOMAIN,
			SHADER_HULL,
			SHADER_MAX
		};

		class ShaderBase	{
		public:
			virtual ~ShaderBase();
		protected:
			ShaderBase();
			string shaderFiles[ShaderStages::SHADER_MAX];
		};
	}
}

