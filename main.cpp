#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION

// Substituir pelo caminho do computador que vai executar o projeto
#include "/home/aluno/Desktop/andrey/stb-master/stb_image.h"
// Caminho do Linux

int main() {
   const char *filename = "./imagens/indoali.bmp";
   int width, height, channels;

   // Carregar a imagem usando STB Image
   unsigned char *image_data = stbi_load(filename, &width, &height, &channels, 0);

   if (!image_data) {
      printf("Erro ao abrir imagem: %s\n", filename);
      return 1;
   }

   unsigned int total_number_of_pixels = 0;

   // Iterar sobre os pixels e contar os que têm componente vermelha maior ou igual a 111
   for (int i = 0; i < width * height * channels; i += channels) {
      unsigned char red = image_data[i];
      if (red >= 111)
         total_number_of_pixels++;
   }

   // Liberar a memória alocada para a imagem
   stbi_image_free(image_data);

   printf("Numero de pixels vermelhos >= 111: %d\n", total_number_of_pixels);

   return 0;
}
