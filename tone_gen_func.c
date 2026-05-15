#include "tone_gen_func.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define AMPLITUDE 30000

static void write_wav_header(FILE *file, int data_size) {
    int file_size = 36 + data_size;
    short audio_format = 1;
    short num_channels = 1;
    int byte_rate = SAMPLE_RATE * num_channels * 16 / 8;
    short block_align = num_channels * 16 / 8;
    short bits_per_sample = 16;

    fwrite("RIFF", 1, 4, file);
    fwrite(&file_size, 4, 1, file);
    fwrite("WAVE", 1, 4, file);

    fwrite("fmt ", 1, 4, file);

    int subchunk1_size = 16;
    fwrite(&subchunk1_size, 4, 1, file);
    fwrite(&audio_format, 2, 1, file);
    fwrite(&num_channels, 2, 1, file);

    int sample_rate = SAMPLE_RATE;
    fwrite(&sample_rate, 4, 1, file);

    fwrite(&byte_rate, 4, 1, file);
    fwrite(&block_align, 2, 1, file);
    fwrite(&bits_per_sample, 2, 1, file);

    fwrite("data", 1, 4, file);
    fwrite(&data_size, 4, 1, file);
}

void generate_tone(
    const char *filename,
    double frequency,
    int duration_seconds
) {
    FILE *file = fopen(filename, "wb");

    if (file == NULL) {
        perror("Could not create WAV file");
        return;
    }

    int total_samples = SAMPLE_RATE * duration_seconds;
    int data_size = total_samples * sizeof(int16_t);

    write_wav_header(file, data_size);

    for (int i = 0; i < total_samples; i++) {
        double t = (double)i / SAMPLE_RATE;
        int16_t sample =
            (int16_t)(AMPLITUDE * sin(2.0 * M_PI * frequency * t));

        fwrite(&sample, sizeof(int16_t), 1, file);
    }

    fclose(file);
}
