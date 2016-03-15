#include "mesh.hpp"
#include <cstdio>
#include <cctype>
#include <cstring>

char const* skip_blank(const char* b)
{
   while (isspace(*b))
   {
      b++;
   }
   return b;
}

char const* skip_number(const char* b)
{
   while (isdigit(*b) || *b == '.' || *b == '-' || *b == '+')
   {
      ++b;
   }
   return b;
}

bool is_token(const char* b, const char* token)
{
   const char* t = token;
   while (*t != '\0')
   {
      if (*(b++) != *(t++))
      {
         return false;
      }
   }
   return true;
}

namespace potato
{

const char* read_face_tuple(const char* buffer, int& idx, int& n_idx, int& uv_idx)
{
   idx = atoi(buffer);
   buffer = skip_number(buffer);
   if (*buffer == '/')
   {
      // normal
      ++buffer;
      n_idx = atoi(buffer);
      buffer = skip_number(buffer);
   }
   if (*buffer == '/')
   {
      // uv
      buffer++;
      uv_idx = atoi(buffer);
      buffer = skip_number(buffer);
   }
   const char* ret = skip_blank(buffer);
   assert(ret != buffer);
   return ret;
}

Mesh::ptr Mesh::load(char const* path)
{
   FILE* file = fopen(path, "rb");
   if (file == nullptr)
   {
      printf("fail to open %s", path);
      return nullptr;
   }

   std::vector<vec3> pos;
   std::vector<Face> faces;
   char buffer[1024];

   int i = 0;
   while (fgets(buffer, 1024, file) != nullptr)
   {
      char const* b = skip_blank(buffer);
      switch (*b)
      {
      case '#':// comment
      case 'g':// start new group
      case 's':// enable smooth blending
         break;
      case 'v':
      {
         if (b[1] == ' ')
         {
            const char* v = skip_blank(b + 2);
            const f32 x = atof(v);
            v = skip_number(v) + 1;
            const f32 y = atof(v);
            v = skip_number(v) + 1;
            const f32 z = atof(v);
            pos.push_back(vec3(x, y, z));
         }
         else if (b[1] == 'n')
         {

         }
         else if (b[1] == 'p')
         {

         }
         break;
      }
      case 'f':// face
      {
         const char* v = skip_blank(b + 2);
         int a = 0, b = 0, c = 0;
         int na = 0, nb = 0, nc = 0;
         int ta = 0, tb = 0, tc = 0;
         v = read_face_tuple(v, a, na, ta);
         v = read_face_tuple(v, b, nb, tb);
         read_face_tuple(v, c, nc, tc);

         assert(a > 0);
         assert(b > 0);
         assert(c > 0);
         uint idx_a = static_cast<uint>(a + 1);
         uint idx_b = static_cast<uint>(b + 1);
         uint idx_c = static_cast<uint>(c + 1);
         Face f = {idx_a, idx_b, idx_c};
         faces.push_back(f);
         break;
      }
      case 'u':// usemtl
         if (is_token(b, "usemtl"))
         {
            // continue;
         }
         break;
      case 'm':// mtllib
         if (is_token(b, "mtllib"))
         {
            // continue;
         }
         break;
      default:
         printf("line %d : %s\n", i++, b);
         break;
      }
   }
   fclose(file);

   return std::make_shared<Mesh>(faces, pos);
}

Mesh::Mesh(std::vector<Face> tri, std::vector<vec3> pos)
   : positions(pos)
   , triangles(tri)
{
}

uint Mesh::intersect_triangle(Ray const& ray)
{

}

}
