#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/OpenGL/OpenGLShader.h>

namespace engine3d{

    static GLenum ShaderTypeFromString(const std::string& type){
        if(type == "vertex") return GL_VERTEX_SHADER;
        if(type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

        CoreLogWarn("ShaderTypeToString returns no enum type");
        assert(false);
        return 0;
    }

    static std::string ShaderTypeToString(GLenum type){
        if(type == GL_VERTEX_SHADER) return "GL_VERTEX_SHADER";
        else if(type == GL_FRAGMENT_SHADER) return "GL_FRAGMENT_SHADER";
        return "No Shader Type was found (make sure it is either vertex or fragment shader!)";
    }

    static std::string GetShaderFileName(const std::string& shaderSource){
        // Examples Filepath: asets/shaders/Texture.glsl
        // Essentially how we will extract Texture.glsl from the filepath.
        auto lastSlash = shaderSource.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

        auto lastDot = shaderSource.rfind('.');
        // If no dot found, then the count is the singleShaderSource.size() - lastSlash
        // Though if there is a dot (.), then we go backwards until we hit our back slash and that'll be our count
        auto count = lastDot == std::string::npos ? shaderSource.size() - lastSlash : lastDot - lastSlash;

        // This is how we get our name, when every time we load in our shader
        // The name of the file is our key, and the actual shader, is our value in unordered map
        // In Short: Name will be used to get the specific shader, that we're storing
        return shaderSource.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::filesystem::path& vertex, const std::filesystem::path& fragment){
        std::unordered_map<GLenum, std::string> shaderSources;

        //! @note In the cases users pass in a raw string, we just add that into shader source
        //! @note If user passes in the two shaders separately then we execute them separately
        if(vertex.extension().string() == ".glsl" and fragment.extension().string() == ".glsl"){
            auto vert = LoadFile(vertex.string());
            auto frag = LoadFile(fragment.string());

            shaderSources[GL_VERTEX_SHADER] = vert;
            shaderSources[GL_FRAGMENT_SHADER] = frag;
        }
        else{
            shaderSources[GL_VERTEX_SHADER] = vertex.string();
            shaderSources[GL_FRAGMENT_SHADER] = fragment.string();
        }

        m_Name = GetShaderFileName(vertex.string());
        CoreLogInfo("Shader Name == {} being loaded", m_Name);

        //! @note Indicating that we want to use our shader the moment we load them
        CompileShaders(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::filesystem::path& singleShaderSource){
        std::string generateSource = LoadFile(singleShaderSource.string());
        auto shaderSources = LoadFromSingleFile(generateSource);

        m_Name = GetShaderFileName(singleShaderSource.string());
        CoreLogInfo("Shader Name == {} being loaded", m_Name);

        CompileShaders(shaderSources);
    }

    OpenGLShader::~OpenGLShader(){
        ReleaseCurrentBoundState();
    }



    std::string OpenGLShader::LoadFile(const std::string& filename){
        std::string result = "";
        std::ifstream ins(filename, std::ios::in | std::ios::binary);

        if(!ins){
            CoreLogError("Could not load shader named {}", filename);
            assert(false);
        }

        ins.seekg(0, std::ios::end);
        result.resize(ins.tellg());
        ins.seekg(0, std::ios::beg);
        ins.read(&result[0], result.size());
        ins.close();

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::LoadFromSingleFile(const std::string& src){
        std::unordered_map<GLenum, std::string> sources;
        const char* shaderTypeToken = "#type";
        size_t tokenLen = strlen(shaderTypeToken);

        size_t pos = src.find(shaderTypeToken, 0);

        while(pos != std::string::npos){
            size_t eol = src.find_first_of("\r\n", pos);
            assert(eol != std::string::npos);

            size_t begin = pos + tokenLen + 1;
            std::string type = src.substr(begin, eol - begin);
            // assert(ShaderTypeFromString(type));

            size_t nextLinePosOffset = src.find_first_not_of("\r\n", eol);
            pos = src.find(shaderTypeToken, nextLinePosOffset);
            sources[ShaderTypeFromString(type)] = src.substr(nextLinePosOffset, pos - (nextLinePosOffset == std::string::npos ? src.size()-1 : nextLinePosOffset));
        }
        return sources;
    }

    void OpenGLShader::CompileShaders(std::unordered_map<GLenum, std::string>& sources){
        GLint program = glCreateProgram();

		std::array<GLenum, 3> shaderIDs;
		int shaderIndex = 0; //  Keeping track of shaders

		for(auto& [t, value] : sources){
			GLenum type = t;
			const std::string& src = value;
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = src.c_str();

			glShaderSource(shader, 1, &sourceCStr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if(isCompiled == GL_FALSE){
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
                
                glDeleteShader(shader);

                CoreLogError("{} Shader compilation failure! (In Shader.cpp)", ShaderTypeToString(type));
                CoreLogError("{}", infoLog.data());
				// assert(false);
                break;
            }

			glAttachShader(program, shader);
            shaderIDs[shaderIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked =0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

		if(isLinked == GL_FALSE){
			GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the program anymore.
            glDeleteProgram(program);
            
            for(GLenum shaderID : shaderIDs){
                glDeleteShader(shaderID);
            }

            CoreLogError("Shader link failure!");
            CoreLogError("{}", infoLog.data());
			// assert(false);
            return;
		}

		for(GLenum shaderID : shaderIDs){
			glDetachShader(program, shaderID);
		}

        m_ProgramId = program;
    }

    // void OpenGLShader::CompileVertexShader(const std::string& vertex){
    // }

    // void OpenGLShader::CompileFragmentShader(const std::string& fragment){
    // }

    std::string OpenGLShader::GetShaderNameID() const{
        return m_Name;
    }

    uint32_t OpenGLShader::Get(const std::string& name){
        return glGetUniformLocation(m_ProgramId, name.c_str());
    }

    void OpenGLShader::ApplyCurrentBoundState() {
        glUseProgram(m_ProgramId);
    }

    void OpenGLShader::ReleaseCurrentBoundState() {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value) {
        ApplyCurrentBoundState();
        uint32_t location = Get(name);
        glUniform1i(location, value);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value) {
        ApplyCurrentBoundState();
        uint32_t location = Get(name);
        glUniform1f(location, value);
    }

    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& values) {
        ApplyCurrentBoundState();
        uint32_t location = Get(name);
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& values) {
        ApplyCurrentBoundState();
        uint32_t location = Get(name);
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& values) {
        ApplyCurrentBoundState();
        uint32_t location = Get(name);
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& values) {
        ApplyCurrentBoundState();
        uint32_t location = Get(name);
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(values));
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& values) {
        ApplyCurrentBoundState();
        uint32_t location = Get(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(values));
    }
};