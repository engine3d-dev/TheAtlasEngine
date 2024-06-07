#include <engine3d/engine3dPrecompiledHeader.h>
#include <engine3d/Image/Image.h>
#include <engine3d/Image/stb_image.h>

namespace Engine3D{
    Image::Image(int w, int h) : width(w), height(h) {
        isImageLoadedSuccessfully = true;
    }

    Image::Image(const std::string& file){
        data = stbi_load(file.c_str(), &width, &height, &pixelChannels, 0);

        if(data == nullptr){
            isImageLoadedSuccessfully = false;
        }
        else{
            isImageLoadedSuccessfully = true;
        }
    }

    Image::~Image(){
        stbi_image_free(data); // free image data stored in CPU
    }


    bool Image::IsLoaded() const{
        return isImageLoadedSuccessfully;
    }

    void* Image::GetImageData() { return data; }

    int Image::GetWidth() const { return width; }
    
    int Image::GetHeight() const { return height; }
    
    int Image::GetPixelChannels() { return pixelChannels; }

    void Image::SetVerticalOnLoad(bool enabled){
        stbi_set_flip_vertically_on_load(enabled);
    }
};