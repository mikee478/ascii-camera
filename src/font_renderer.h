#pragma once

#include <string>
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "shader.h"
#include <vector>
#include <array>

class FontRenderer
{
public:
    FontRenderer(const std::string &font_path, unsigned int font_size);
    void RenderText(const std::vector<char> &text, float left, float top);
    unsigned long GetLineSpacing() const;

private:
    unsigned long line_spacing_;

    unsigned int id_;
    static const char MIN_ASCII_CODE = 32; // space
    static const char MAX_ASCII_CODE = 126; // tilde

    struct CharacterInfo 
    {
        float advance_x; // advance.x
        float advance_y; // advance.y
        
        float bitmap_width; // bitmap.width;
        float bitmap_rows; // bitmap.rows;
        
        float bitmap_left; // horizontal distance from the glyph origin (0,0) to the left-most pixel of the glyph bitmap;
        float bitmap_top; // vertical distance from the glyph origin (0,0) to the top-most pixel of the glyph bitmap
        
        float texture_x; // x offset of glyph in texture coordinates
        float texture_width; // bitmap_width divided by atlas_width
        float texture_height; // bitmap_rows divided by atlas_height
    } char_info_[MAX_ASCII_CODE+1];

    struct Vertex
    {
        float position[2];
        float tex_coor[2];
    };
    static const int MAX_CHARS = 200*200;
    static const int VERTICES_PER_CHAR = 6;
    std::array<Vertex, VERTICES_PER_CHAR * MAX_CHARS> data_;

    VertexArray va_;
    VertexBuffer vb_;
    Shader shader_;
};