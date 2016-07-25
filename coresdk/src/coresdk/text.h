//
//  text.hpp
//  splashkit
//
//  Created by Andrew Cain on 25/07/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#ifndef text_hpp
#define text_hpp

#include "types.h"
#include "drawing_options.h"

#include <string>
using namespace std;

/// Loads a font from file with the specified side. Fonts must be freed using
/// the FreeFont routine once finished with. Once the font is loaded you
/// can set its style using SetFontStyle. Fonts are then used to draw and
/// measure text in your programs.
///
/// @lib
/// @sn loadFontFile:%s size:%s
///
/// @class Font
/// @constructor
/// @csn initWithFontName:%s andSize:%s
font load_font(string name, string filename, int size);

bool has_font(string name);

font font_named(string name);

void draw_text(string text, color clr, font fnt, float x, float y, drawing_options opts);

void draw_text(string text, color clr, font fnt, float x, float y);


#endif /* text_hpp */