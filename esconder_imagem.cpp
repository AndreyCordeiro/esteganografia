#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include "/home/aluno/Desktop/Esteganografia/esteganografia/stb-master/stb_image.h" // Caminho do Linux


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "/home/aluno/Desktop/Esteganografia/esteganografia/stb-master/stb_image_write.h" // Caminho do Linux


// Substituir pelo caminho do computador que vai executar o projeto


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


void esconder_imagem(char *texto, int width, int height, int channels, unsigned char *image_data)
{
  int texto_len = strlen(texto);
  int texto_index = 0;


  time_t tempo = time(NULL);
  srand(tempo);
  cout << "Semente: " << tempo << endl;


  for (int i = 0; i < width * height * channels; i += channels)
  {
     unsigned char red = image_data[i];
     unsigned char green = image_data[i + 1];
     unsigned char blue = image_data[i + 2];


     if (texto_index <= texto_len)
     {
     	unsigned char caractere;
     	
     	if (texto_index < texto_len) {
     		caractere = texto[texto_index];
     	} else {
	     	caractere = 0;
     	}
        
        unsigned char metade1 = extract_bits(caractere, 0, 2);
        unsigned char metade2 = extract_bits(caractere, 2, 2);
        unsigned char metade3 = extract_bits(caractere, 4, 4);


        metade1 ^= (rand() % 4);
        metade2 ^= (rand() % 4);
        metade3 ^= (rand() % 16);


        red = set_bits(red, 0, 2, metade1);
        green = set_bits(green, 0, 2, metade2);
        blue = set_bits(blue, 0, 4, metade3);


        image_data[i] = red;
        image_data[i + 1] = green;
        image_data[i + 2] = blue;
        
        if (texto_index == texto_len) break;
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


  char *texto = new char[256];
  cout << "Informe a mensagem que deseja esconder: ";
  cin.getline(texto, 256);


  esconder_imagem(texto, width, height, channels, image_data);


  // Salva a imagem escondida e libera o espaço alocado na memória
  stbi_write_bmp("imagem_com_mensagem.bmp", width, height, channels, image_data);
  stbi_image_free(image_data);


  cout << "Mensagem escondida na imagem!" << endl;


  return 0;
}
