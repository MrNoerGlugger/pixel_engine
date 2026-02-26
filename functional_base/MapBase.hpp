#pragma once
#include <map>

#include <SDL3/SDL.h>

using std::map;
using std::pair;

template <typename T>
class MapBase
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default Constructor
    ///
    /// Creates empty Map
    ///
    ////////////////////////////////////////////////////////////
    MapBase() :
    map_base(),
    bounds() 
    {

    }


    ////////////////////////////////////////////////////////////
    /// \brief Place an object on the map or parts of it
    ///
    ////////////////////////////////////////////////////////////
    void placeObject(T object, SDL_Rect obj_bounds)  {
        //set the bounds to be at least 1 in width & height
        obj_bounds.w = obj_bounds.w < 1 ? 1 : obj_bounds.w;
        obj_bounds.h = obj_bounds.h < 1 ? 1 : obj_bounds.h;

        SDL_Point obj_top_left  = SDL_Point{obj_bounds.x, obj_bounds.y};
        SDL_Point obj_bot_right = SDL_Point{obj_bounds.x + obj_bounds.w, obj_bounds.y + obj_bounds.h};

        //checking for potential resizing
        if (!SDL_PointInRect(&obj_bot_right, &bounds)) {
            bounds.w = bounds.w > obj_bounds.x + obj_bounds.w ? bounds.w : obj_bounds.x + obj_bounds.w;
            bounds.h = bounds.h > obj_bounds.y + obj_bounds.h ? bounds.h : obj_bounds.y + obj_bounds.h;
        }
        if (!SDL_PointInRect(&obj_top_left, &bounds)) {
            bounds.x = bounds.x < obj_bounds.x ? bounds.x : obj_bounds.x;
            bounds.y = bounds.y < obj_bounds.y ? bounds.y : obj_bounds.y;
        }

        //loop for placing the object inside the bounds on the map
        for (int x = 0; x < bounds.w; x++) {
            for (int y = 0; y < bounds.h; y++) {
                map_base[pair<int, int>(bounds.x + x, bounds.y + y)] = object;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    /// \brief Clears the map of its information
    ///
    ////////////////////////////////////////////////////////////
    void clear() {
        map_base.clear();
        bounds = SDL_Rect{0, 0, 0, 0};
    }
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the bounding rectangle of the map
    ///
    /// \return bounding rectangle of the map
    ///
    ////////////////////////////////////////////////////////////
    SDL_Rect getBounds() const {
        return bounds;
    } 

    ////////////////////////////////////////////////////////////
    /// \brief Get the width of the map
    ///
    /// \return width of the map
    ///
    ////////////////////////////////////////////////////////////
    int getWidth() const {
        return bounds.w;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Get the height of the map
    ///
    /// \return height of the map
    ///
    ////////////////////////////////////////////////////////////
    int getHeight() const {
        return bounds.h;
    }  

protected:
    map<pair<int, int>, T> map_base;    ///< map object to save map data to
    SDL_Rect bounds;                    ///< bounding box of the map
};