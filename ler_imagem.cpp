#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include "/home/aluno/Desktop/Esteganografia/esteganografia/stb-master/stb_image.h" // Caminho do Linux


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "/home/aluno/Desktop/Esteganografia/esteganografia/stb-master/stb_image_write.h" // Caminho do Linux


using namespace std;


template <typename T>
inline T extract_bits(const T v, const uint32_t bstart, const uint32_t blength)
{
   const T mask = (1 << blength) - 1;
   return (v >> bstart) & mask;
}


char extrair_caractere(unsigned char red, unsigned char green, unsigned char blue)
{
   unsigned char metade1 = extract_bits(red, 0, 2);
   unsigned char metade2 = extract_bits(green, 0, 2);
   unsigned char metade3 = extract_bits(blue, 0, 4);


   return (metade1 & 3) | ((metade2 & 3) << 2) | ((metade3 & 15) << 4);
}


int main()
{
   const char *filename = "imagem_com_mensagem.bmp"; // Aarquivo com a mensagem escondida
   int width, height, channels;


   // Carregar a imagem
   unsigned char *image_data = stbi_load(filename, &width, &height, &channels, 0);


   if (!image_data)
   {
       cout << "Erro ao abrir imagem: " << filename << endl;
       return 1;
   }


   int texto_len = 256;
   char texto[256] = "";


   time_t semente;

   cout << "Informe a semente usada para ocultar a mensagem: ";
   cin >> semente;
   srand(semente);


   int texto_index = 0;
   bool controle = false;

   for (int i = 0; i < width * height * channels; i += channels)
   {
       if (controle) break;
      
       if (texto_index < texto_len) {
           char caractere = extrair_caractere(image_data[i], image_data[i + 1], image_data[i + 2]);


           char metade1 = (caractere & 3) ^ (rand() % 4);
           char metade2 = ((caractere >> 2) & 3) ^ (rand() % 4);
           char metade3 = ((caractere >> 4) & 15) ^ (rand() % 16);


           char caractere_original = metade1 | (metade2 << 2) | (metade3 << 4);

           if (caractere_original == 0) {
               controle = true;
               texto[texto_index] = 0;
               
               break;
           }

           texto[texto_index] = caractere_original;
           texto_index++;
       }
       else
           break;
   }


   cout << "Mensagem recuperada: " << texto << endl;


   stbi_image_free(image_data);


   return 0;
}
