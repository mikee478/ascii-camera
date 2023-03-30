#include "font_renderer.h"

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include "vertex_buffer_layout.h"
#include "renderer.h"

FontRenderer::FontRenderer(const std::string &font_path, unsigned int font_size)
    : vb_(nullptr, VERTICES_PER_CHAR * MAX_CHARS * sizeof(Vertex), GL_DYNAMIC_DRAW),
    shader_("/Users/michael/Documents/projects/ascii-camera/res/shaders/vertex.shader",
        "/Users/michael/Documents/projects/ascii-camera/res/shaders/fragment.shader")
{
    VertexBufferLayout layout;
    layout.Push<float>(2); // position
    layout.Push<float>(2); // text_coor

    va_.AddBuffer(vb_, layout);

    shader_.Bind();
    shader_.SetUniformMatrix4f("proj_mat", Shader::projection_mat);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        throw std::runtime_error("ERROR - Failed to init FreeType Library");
    }

    FT_Face face;
    if (FT_New_Face(ft, font_path.c_str(), 0, &face))
    {
        throw std::runtime_error("ERROR - Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, font_size, font_size);

    // text height in 26.6 frac. pixels
    line_spacing_ = face->size->metrics.height / 64;

    FT_GlyphSlot g = face->glyph;

    int atlas_width = 0;
    int atlas_height = 0;

    for(int i = MIN_ASCII_CODE; i <= MAX_ASCII_CODE; i++) {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            std::cout << "Loading character " << (char)i << " failed!" << std::endl;
            continue;
        }

        atlas_width += g->bitmap.width;
        atlas_height = std::max(atlas_height, (int)g->bitmap.rows);
    }

    GLCALL(glActiveTexture(GL_TEXTURE0));
    GLCALL(glGenTextures(1, &id_));
    GLCALL(glBindTexture(GL_TEXTURE_2D, id_));
    GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0));

    int atlas_x = 0;
    for(int i = MIN_ASCII_CODE; i <= MAX_ASCII_CODE; i++)
    {
        if(FT_Load_Char(face, i, FT_LOAD_RENDER))
            continue;

        GLCALL(glTexSubImage2D(
            GL_TEXTURE_2D, 0, atlas_x, 0, g->bitmap.width, g->bitmap.rows,
            GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer));

        // 26.6 fixed float format used to define fractional pixel coordinates (1 unit = 1/64 pixel)
        // char_info_[i].advance_x = g->metrics.horiAdvance >> 6;
        // char_info_[i].advance_y = g->metrics.vertAdvance >> 6;
        
        char_info_[i].advance_x = g->advance.x >> 6;
        char_info_[i].advance_y = g->advance.y >> 6;        

        char_info_[i].bitmap_width = g->bitmap.width;
        char_info_[i].bitmap_rows = g->bitmap.rows;

        char_info_[i].bitmap_left = g->bitmap_left;
        char_info_[i].bitmap_top = g->bitmap_top;

        char_info_[i].texture_x = (float)atlas_x / atlas_width;
        char_info_[i].texture_width = (float)g->bitmap.width / atlas_width;
        char_info_[i].texture_height = (float)g->bitmap.rows / atlas_height;

        atlas_x += g->bitmap.width;
    }
}

void FontRenderer::RenderText(const std::vector<char> &text, float x, float y)
{
    float origin_x = x;
    float origin_y = y;
    int i = 0;
    for(const char &c : text) 
    {
        if(c == '\n')
        {
            origin_x = x;
            origin_y -= line_spacing_;
        }

        float left = origin_x + char_info_[c].bitmap_left;
        float top = origin_y + char_info_[c].bitmap_top;
        float w = char_info_[c].bitmap_width;
        float h = char_info_[c].bitmap_rows;
        float right = left + w;
        float bottom = top - h;

        // Advance the cursor to the start of the next character
        origin_x += char_info_[c].advance_x;
        origin_y += char_info_[c].advance_y;

        // Skip glyphs that have no pixels
        if(!w || !h)
            continue;
        
        // bottom left
        data_[i].position[0] = left;
        data_[i].position[1] = bottom;
        data_[i].tex_coor[0] = char_info_[c].texture_x;
        data_[i].tex_coor[1] = char_info_[c].texture_height;
        i++;

        // bottom right
        data_[i].position[0] = right;
        data_[i].position[1] = bottom;
        data_[i].tex_coor[0] = char_info_[c].texture_x + char_info_[c].texture_width;
        data_[i].tex_coor[1] = char_info_[c].texture_height;
        i++;

        // top left
        data_[i].position[0] = left;
        data_[i].position[1] = top;
        data_[i].tex_coor[0] = char_info_[c].texture_x;
        data_[i].tex_coor[1] = 0.0f;
        i++;

        // bottom right
        data_[i].position[0] = right;
        data_[i].position[1] = bottom;
        data_[i].tex_coor[0] = char_info_[c].texture_x + char_info_[c].texture_width;
        data_[i].tex_coor[1] = char_info_[c].texture_height;
        i++;

        // top left
        data_[i].position[0] = left;
        data_[i].position[1] = top;
        data_[i].tex_coor[0] = char_info_[c].texture_x;
        data_[i].tex_coor[1] = 0.0f;
        i++;

        // top right
        data_[i].position[0] = right;
        data_[i].position[1] = top;
        data_[i].tex_coor[0] = char_info_[c].texture_x + char_info_[c].texture_width;
        data_[i].tex_coor[1] = 0.0f;
        i++;
    }

    va_.Bind();
    vb_.UpdateBuffer(&data_[0], i * sizeof(Vertex));

    shader_.Bind();
    glDrawArrays(GL_TRIANGLES, 0, i);
}

unsigned long FontRenderer::GetLineSpacing() const
{
    return line_spacing_;
}
