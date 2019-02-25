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

		class ShaderBase
		{
		public:
			ShaderBase();
			virtual ~ShaderBase();

			virtual void LoadShader(const string& vertex, const string& fragment, const string& geometry = "", const string& domain = "", const string& hull = "") = 0;
		protected:

			string shaderFiles[ShaderStages::SHADER_MAX];
		};
	}
}

