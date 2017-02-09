#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"


void* GridTexture(int& width, int& height, uint32_t c1, uint32_t c2) {
	uint32_t* buffer = (uint32_t*)malloc(sizeof(uint32_t) * 256 * 256);

	int i, j;
	for (j = 0; j < 256; j++) {
		for (i = 0; i < 256; i++) {
			int x = i / 32, y = j / 32;
			buffer[j * 256 + i] = ((x + y) & 1) ? c1 : c2;
		}
	}

	width = 256;
	height = 256;

	return buffer;
}


void* LoadTexture(std::string filepath, bool isrelpath, int& width, int& height) {
	if (isrelpath) {
		filepath = _FSPFX absolute(filepath, ResourceDir()).string();
	}

	int comp = 0;
	stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &comp, 4);
	if (!data) {
		return nullptr;
	}

	uint8_t* buffer = (uint8_t*)malloc(sizeof(uint32_t) * width * height);

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			const unsigned char* pp_src = data + 4 * (j * width + i);
			unsigned char* pp_dst = buffer + 4 * (j * width + i);
			pp_dst[0] = pp_src[0];
			pp_dst[1] = pp_src[1];
			pp_dst[2] = pp_src[2];
			pp_dst[3] = pp_src[3];
		}
	}

	stbi_image_free(data);

	return buffer;
}