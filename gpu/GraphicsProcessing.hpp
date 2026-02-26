#pragma once

#include <BasicRhombus.hpp>
#include <LightSource.hpp>
#include <Rectangle.hpp>
#include <ShaderMap.hpp>
#include <ShaderTexture.hpp>
#include <Texture.hpp>
#include <Text.hpp>
#include <TextureSheet.hpp>

#include <CustomText.hpp> 
#include <Screen.hpp>

// #include "../gui/gui_modules/button_modules/ButtonText.hpp"
// #include "../gui/gui_modules/textbox_modules/TextboxFramed.hpp"

class GraphicsProcessing {
    static map<string, Drawable*> screen_map;
    static string texture_shader_map;
    static string texture_main_target;
    static string texture_overlay_target;
    static Texture texture_main;
    static Texture texture_overlay;
    static ShaderMap shader_map;


public:
//pre-processing steps
    //adds a screen to the screen_map
    static void add_screen(Drawable* screen, string screen_id);

    //translate current window resolution into a scale
    static void interpret_window_scale();

    //resizes the textures currently used as texture targets for the draw process
    static void resize_textures();

    //initialize the needed resources for the draw process
    static void init();

//general processing steps
    //operates all graphics processing steps
    static void graphics_processing();

    //update geometry of all objects prior to using them
    static void update_geometry_all();

    //create the shader map for use in the default phase
    static void draw_shader_map();

    //draw the shadow map for light source calculations
    static void draw_shadow_phase();

    //draw light sources onto the shader map
    static void draw_light_phase();

    //draw the default phase and apply the shader map
    static void draw_default_phase();

    //draw the overlay phase independent from all other phases
    static void draw_overlay_phase();

//camera view
    //set the camera to a specific spot
    static void set_view(float x, float y);

    //move the camera by a specific value
    static void move_view(float x, float y);
};