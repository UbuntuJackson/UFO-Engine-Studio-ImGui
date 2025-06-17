#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace UFOEngineStudio{

class TextureWrapper{
public:
    SDL_Texture* texture;
    std::string alias;
    bool edit_mode = false;
    TextureWrapper(SDL_Texture* _texture, std::string _name) : texture{_texture}, alias{_name}{}
};

class AssetManager{
public:
    std::vector<std::unique_ptr<TextureWrapper>> textures;
    SDL_Renderer* renderer = nullptr;

    AssetManager() = default;

    AssetManager(SDL_Renderer* _renderer){
        renderer = _renderer;
    }

    void AddTexture(std::string _alias, std::string _path){
        SDL_Surface* example_surface = IMG_Load(_path.c_str());
        assert(example_surface != nullptr);
        SDL_Texture* example_texture = SDL_CreateTextureFromSurface(renderer ,example_surface);
        assert(example_texture != nullptr);
        SDL_DestroySurface(example_surface);

        textures.push_back(std::make_unique<TextureWrapper>(example_texture,_alias));
    }

    void DeleteTexture(std::string _alias){

        for(int i = textures.size() -1; i >= 0; i--){
            if(textures[i]->alias == _alias){
                SDL_DestroyTexture(textures[i]->texture);
                textures.erase(textures.begin()+i);
                
            }
        }

    }

    ~AssetManager(){
        for(auto && v : textures) SDL_DestroyTexture(v->texture);
        textures.clear();
    }

};

}