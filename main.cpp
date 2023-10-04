#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION
#include "/home/aluno/Desktop/andrey/esteganografia/stb-master/stb_image.h" // Caminho do Linux

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "/home/aluno/Desktop/andrey/esteganografia/stb-master/stb_image_write.h" // Caminho do Linux

// Substituir pelo caminho do computador que vai executar o projeto

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

template <typename T>
inline T extract_bits(const T v, const uint32_t bstart, const uint32_t blength)
{
   const T mask = (1 << blength) - 1;

   return (v >> bstart) & mask;
}

template <typename T>
inline T set_bits(const T v, const uint32_t bstart, const uint32_t blength, const T bits)
{
   const T mask = ((1 << blength) - 1) << bstart;

   return (v & ~mask) | (bits << bstart);
}

void esconder_imagem(const char *texto, int width, int height, int channels, unsigned char *image_data)
{
   int texto_len = strlen(texto);
   int texto_index = 0;

   time_t tempo = time(NULL);
   srand(tempo);
   cout << "Semente: " << tempo << endl;

   // Iterar sobre os pixels e esconder o texto nos canais vermelho, verde e azul
   for (int i = 0; i < width * height * channels && texto_index < texto_len; i += channels)
   {
      unsigned char red = image_data[i];
      unsigned char green = image_data[i + 1];
      unsigned char blue = image_data[i + 2];

      if (texto_index < texto_len)
      {
         char caractere = texto[texto_index];
         unsigned char metade1 = extract_bits(caractere, 0, 2);
         unsigned char metade2 = extract_bits(caractere, 2, 2);
         unsigned char metade3 = extract_bits(caractere, 4, 4);

         // Adicionando pequena aleatoriedade aos bits modificados
         metade1 ^= (rand() % 4);
         metade2 ^= (rand() % 4);
         metade3 ^= (rand() % 16);

         // Definir metade1 nos bits do canal vermelho
         red = set_bits(red, 0, 2, metade1);

         // Definir metade2 nos bits do canal verde
         green = set_bits(green, 0, 2, metade2);

         // Definir metade3 nos bits do canal azul
         blue = set_bits(blue, 0, 4, metade3);

         // Atualizar os valores dos canais vermelho, verde e azul no pixel
         image_data[i] = red;
         image_data[i + 1] = green;
         image_data[i + 2] = blue;
      }
      else
         break;

      texto_index++;
   }
}

int main()
{
   const char *filename = "./imagens/indoali.bmp";
   int width, height, channels;

   // Carregar a imagem
   unsigned char *image_data = stbi_load(filename, &width, &height, &channels, 0);

   if (!image_data)
   {
      cout << "Erro ao abrir imagem: " << filename << endl;
      return 1;
   }

   // Texto que você deseja esconder na imagem
   const char *texto = "Mensagem secreta";

   esconder_imagem(texto, width, height, channels, image_data);

   // Salvar a imagem criptografada e liberar o espaço alocado na memória
   stbi_write_bmp("imagem_com_mensagem.bmp", width, height, channels, image_data);
   stbi_image_free(image_data);

   cout << "Mensagem escondida na imagem" << endl;

   return 0;
}