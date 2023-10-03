#include <cstdio>
#define STB_IMAGE_IMPLEMENTATION
#include "/home/aluno/Desktop/andrey/esteganografia/stb-master/stb_image.h" // Caminho do Linux

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "/home/aluno/Desktop/andrey/esteganografia/stb-master/stb_image_write.h" // Caminho do Linux

// Substituir pelo caminho do computador que vai executar o projeto

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

    // Carregar a imagem
    unsigned char *image_data = stbi_load(filename, &width, &height, &channels, 0);

    if (!image_data) {
        printf("Erro ao abrir imagem: %s\n", filename);
        return 1;
    }

    // Texto que você deseja esconder na imagem
    const char *texto = "Mensagem secreta";

    int texto_len = strlen(texto);
    int texto_index = 0;

    // Iterar sobre os pixels e esconder o texto nos canais vermelho e verde
    for (int i = 0; i < width * height * channels && texto_index < texto_len; i += channels) {
        unsigned char red = image_data[i];
        unsigned char green = image_data[i + 1];
        //unsigned char blue = image_data[i + 2];

        if (texto_index < texto_len) {
            char caractere = texto[texto_index];
            unsigned char metade1 = extract_bits(caractere, 0, 4);
            unsigned char metade2 = extract_bits(caractere, 4, 4);

            // Definir metade1 nos bits menos significativos do canal vermelho
            red = set_bits(red, 0, 4, metade1);

            // Definir metade2 nos bits menos significativos do canal verde
            green = set_bits(green, 0, 4, metade2);

            // Atualizar os valores dos canais vermelho e verde no pixel
            image_data[i] = red;
            image_data[i + 1] = green;
        }

        texto_index++;
    }

    // Salvar a imagem criptografada e liberar o espaço alocado na memória
    stbi_write_bmp("imagem_com_mensagem.bmp", width, height, channels, image_data);
    stbi_image_free(image_data);

    printf("Mensagem escondida na imagem.\n");

    return 0;
}