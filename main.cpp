#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION

// Substituir pelo caminho do computador que vai executar o projeto
#include "/home/aluno/Desktop/andrey/stb-master/stb_image.h"
// Caminho do Linux

template <typename T>
inline T extract_bits(const T v, const unsigned bstart, const unsigned blength) {
	const T mask = (1 << blength) - 1;

	return ((v >> bstart) & mask);
}

template <typename T>
inline T set_bits(const T v, const unsigned bstart, const unsigned blength, const T bits) {
	const T mask = ((1 << blength) - 1) << bstart; 

	return (v & mask) | (bits << bstart);
}

int main() {
   const char *filename = "./imagens/indoali.bmp";
   int width, height, channels;

   // Carregar a imagem usando STB Image
   unsigned char *image_data = stbi_load(filename, &width, &height, &channels, 0);

   if (!image_data) {
      printf("Erro ao abrir imagem: %s\n", filename);
      return 1;
   }

   unsigned int red_pixels = 0;
   unsigned int green_pixels = 0;
   unsigned int blue_pixels = 0;

   // Iterar sobre os pixels e contar os que têm componente de cada cor maior ou igual a 111
   for (int i = 0; i < width * height * channels; i += channels) {
      unsigned char red = image_data[i];
      unsigned char green = image_data[i + 1]; // Canal verde
      unsigned char blue = image_data[i + 2];  // Canal azul

      if (red >= 111)
         red_pixels++;

      if (green >= 111)
         green_pixels++;

      if (blue >= 111)
         blue_pixels++;
   }

   // Liberar a memória alocada para a imagem
   stbi_image_free(image_data);

   printf("Numero de pixels vermelhos >= 111: %d\n", red_pixels);
   printf("Numero de pixels verdes >= 111: %d\n", green_pixels);
   printf("Numero de pixels azuis >= 111: %d\n", blue_pixels);

   return 0;
}