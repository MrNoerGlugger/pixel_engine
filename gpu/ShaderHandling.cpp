#include "ShaderHandling.hpp"


//#ifdef linux
bool game::compile_shader(
    SDL_GPUDevice* device,
    const char* shaderFilename,
	Uint32 gpu_shader_format
) {
	SDL_ShaderCross_ShaderStage stage;
	if (SDL_strstr(shaderFilename, ".vert"))
	{
		stage = SDL_SHADERCROSS_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(shaderFilename, ".frag"))
	{
		stage = SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		Logger::log_error(std::format("Invalid shader stage! {}", shaderFilename));
		return false;
	}
	char srcPath[256];
	char fullPath[256];
    const char *BasePath = SDL_GetBasePath();
    SDL_snprintf(srcPath, sizeof(srcPath), "%s/resources/shaders/Source/%s.hlsl", BasePath, shaderFilename);
	const char *entrypoint;


	size_t codeSize;
	void* code = SDL_LoadFile(srcPath, &codeSize);
	if (code == NULL)
	{
		Logger::log_error(std::format("Failed to load shader from disk! {}", shaderFilename));
		return false;
	}

    

	if (gpu_shader_format & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/resources/shaders/Compiled/SPIRV/%s.spv", BasePath, shaderFilename);
		entrypoint = "main";
	} else if (gpu_shader_format & SDL_GPU_SHADERFORMAT_MSL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/resources/shaders/Compiled/MSL/%s.msl", BasePath, shaderFilename);
		entrypoint = "main";
	} else if (gpu_shader_format & SDL_GPU_SHADERFORMAT_DXIL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%s/resources/shaders/Compiled/DXIL/%s.dxil", BasePath, shaderFilename);
		entrypoint = "main";
	} else {
		Logger::log_error(std::format("Unrecognized backend shader format! {}", shaderFilename));
		return false;
	}

    SDL_ShaderCross_HLSL_Info info(
        (char *)code,
        entrypoint,
        NULL,
        NULL,
        stage,
        0
    );

    size_t buffer_size;
	void* shader_data;

	if (gpu_shader_format & SDL_GPU_SHADERFORMAT_SPIRV) {
		shader_data = SDL_ShaderCross_CompileSPIRVFromHLSL(&info, &buffer_size);

	} else if (gpu_shader_format & SDL_GPU_SHADERFORMAT_MSL) {
		void* shader_data_spirv = SDL_ShaderCross_CompileSPIRVFromHLSL(&info, &buffer_size);
		entrypoint = "main0";

		SDL_ShaderCross_SPIRV_Info info_spirv(
			(Uint8 *)shader_data_spirv,
			buffer_size,
			entrypoint,
			stage,
			0
		);

		shader_data = SDL_ShaderCross_TranspileMSLFromSPIRV(&info_spirv);
		SDL_free(shader_data_spirv);
	} else if (gpu_shader_format & SDL_GPU_SHADERFORMAT_DXIL) {
		shader_data = SDL_ShaderCross_CompileDXILFromHLSL(&info, &buffer_size);
	} else {
		Logger::log_error(std::format("Unrecognized backend shader format! {}", shaderFilename));
		return false;
	}


    std::fstream stream(fullPath, std::ios::binary | std::ios::out);
    if (!stream) {
        Logger::log_error(std::format("failed to open file! {}", fullPath));
        return false;
    }

	if (gpu_shader_format & SDL_GPU_SHADERFORMAT_MSL) {
		stream << (char *)shader_data;
	}
	else {
		stream.write((char *)shader_data, buffer_size);
	}
    stream.close();

    SDL_free(shader_data);
	SDL_free(code);

	return true;
}
//#endif

SDL_GPUShader* game::load_shader(
    SDL_GPUDevice* device,
    const char* shaderFilename,
    Uint32 samplerCount,
    Uint32 uniformBufferCount,
    Uint32 storageBufferCount,
    Uint32 storageTextureCount
) {
	//#ifdef linux
    compile_shader(device, shaderFilename, SDL_GPU_SHADERFORMAT_SPIRV);
    compile_shader(device, shaderFilename, SDL_GPU_SHADERFORMAT_DXIL);
    compile_shader(device, shaderFilename, SDL_GPU_SHADERFORMAT_MSL);
	//#endif

	// Auto-detect the shader stage from the file name for convenience
	SDL_GPUShaderStage stage;
	if (SDL_strstr(shaderFilename, ".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(shaderFilename, ".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		Logger::log_error(std::format("Invalid shader stage! {}", shaderFilename));
		return NULL;
	}

	char fullPath[256];
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char *entrypoint;
    const char *BasePath = SDL_GetBasePath();

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sresources/shaders/Compiled/SPIRV/%s.spv", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	} else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sresources/shaders/Compiled/MSL/%s.msl", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_MSL;
		entrypoint = "main0";
	} else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sresources/shaders/Compiled/DXIL/%s.dxil", BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_DXIL;
		entrypoint = "main";
	} else {
		Logger::log_error(std::format("Unrecognized backend shader format! {}", shaderFilename));
		return NULL;
	}

	size_t codeSize;
	void* code = SDL_LoadFile(fullPath, &codeSize);
	if (code == NULL)
	{
		Logger::log_error(std::format("Failed to load shader from disk! {}", fullPath));
		return NULL;
	}

	SDL_GPUShaderCreateInfo shaderInfo(
        codeSize,
        (Uint8 *)code,
        entrypoint,
        format,
        stage,
        samplerCount,
        storageTextureCount,
        storageBufferCount,
        uniformBufferCount,
		0
    );
	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == NULL)
	{
		Logger::log_error(std::format("Failed to create shader! {}", shaderFilename));
		SDL_free(code);
		return NULL;
	}

	SDL_free(code);
	return shader;
}