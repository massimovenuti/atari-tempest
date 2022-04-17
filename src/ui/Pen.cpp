#include "Pen.hpp"
#include "../utils/Errors.hpp"

int
Pen::index_of_ascii(const char c)
{
  return static_cast<int>(c) - 32;
}

void
Pen::draw_edge(SDL_Renderer* const r,
               const float x1,
               const float y1,
               const float x2,
               const float y2,
               const int xoffset,
               const int yoffset)
{
  SDL_SetRenderDrawColor(r, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLineF(
    r, x1 + xoffset, -y1 + yoffset, x2 + xoffset, -y2 + yoffset);
}

int
Pen::draw_character(SDL_Renderer* const r,
                    const char c,
                    const int xoffset,
                    const int yoffset,
                    const float multiplier)
{
  int char_index = Pen::index_of_ascii(c);
  if (char_index < 0 || char_index >= 95) {
    throw errors::non_valid_character_requested();
  }

  int i = 2;
  while ((i + 3) < 112) {
    if (Pen::hershey_table[char_index][i] == -1 ||
        Pen::hershey_table[char_index][i + 2] == -1) {
      i += 2;
    } else {
      draw_edge(r,
                multiplier * Pen::hershey_table[char_index][i],
                multiplier * Pen::hershey_table[char_index][i + 1],
                multiplier * Pen::hershey_table[char_index][i + 2],
                multiplier * Pen::hershey_table[char_index][i + 3],
                xoffset,
                yoffset);
      i += 2;
    }
  }
  return multiplier * Pen::hershey_table[char_index][1];
}

// Public

void
Pen::draw_string(std::string const& s,
                 int xpos,
                 int ypos,
                 SDL_Renderer* const r)
{
  for (auto& c : s) {
    xpos += Pen::draw_character(r, c, xpos, ypos, 1) + Pen::space_size;
  }
}

void
Pen::draw_string_centered_x(std::string const& s,
                            int ypos,
                            SDL_Renderer* const r)
{
  // TODO: remove hardcoded WINDOWS_WIDTH
  draw_string(s, 960 / 2 - get_string_width(s) / 2, ypos, r);
}

void
Pen::draw_string(std::string const& s,
                 int xpos,
                 int ypos,
                 SDL_Renderer* const r,
                 float size)
{
  for (auto& c : s) {
    xpos += Pen::draw_character(r, c, xpos, ypos, size) + Pen::space_size;
  }
}

void
Pen::draw_string_centered_x(std::string const& s,
                            int ypos,
                            SDL_Renderer* const r,
                            float size)
{
  draw_string(s, 960 / 2 - get_string_width(s, size) / 2, ypos, r, size);
}

int
Pen::get_string_width(std::string const& s)
{
  int total_width = 0;
  for (auto& c : s) {
    total_width += Pen::hershey_table[Pen::index_of_ascii(c)][1];
  }
  return total_width + (s.size() - 1) * Pen::space_size;
}

int
Pen::get_string_width(std::string const& s, float size)
{
  int total_width = 0;
  for (auto& c : s) {
    total_width += size * Pen::hershey_table[Pen::index_of_ascii(c)][1];
  }
  return total_width + (s.size() - 1) * Pen::space_size;
}