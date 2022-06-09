#ifndef FONT_HPP_INCLUDED
#define FONT_HPP_INCLUDED

// generated using https://evanw.github.io/font-texture-generator/

typedef struct Character {
  int code_point, x, y, width, height, origin_x, origin_y;
} Character;

typedef struct Font {
  const char *name;
  int size, bold, italic, width, height;
  const int character_count;
  Character *characters;
} Font;

#endif // FONT_HPP_INCLUDED