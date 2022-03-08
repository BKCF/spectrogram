#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "plplot.h"

#include <fftw3.h>
#include <math.h>

#include "include/libbmp.h"

#include "include/pa_linux_alsa.h"

#include "audiofuncs.h"



#define ASSERT assert

float PI = 3.1415926535;

int NUM_COLORS = 256;
int colors[] = {
    0x000000, 0x800000, 0x008000, 0x808000, 0x000080, 0x800080, 0x008080,
    0xC0C0C0, 0xC0DCC0, 0xA6CAF0, 0x402000, 0x602000, 0x802000, 0xA02000,
    0xC02000, 0xE02000, 0x004000, 0x204000, 0x404000, 0x604000, 0x804000,
    0xA04000, 0xC04000, 0xE04000, 0x006000, 0x206000, 0x406000, 0x606000,
    0x806000, 0xA06000, 0xC06000, 0xE06000, 0x008000, 0x208000, 0x408000,
    0x608000, 0x808000, 0xA08000, 0xC08000, 0xE08000, 0x00A000, 0x20A000,
    0x40A000, 0x60A000, 0x80A000, 0xA0A000, 0xC0A000, 0xE0A000, 0x00C000,
    0x20C000, 0x40C000, 0x60C000, 0x80C000, 0xA0C000, 0xC0C000, 0xE0C000,
    0x00E000, 0x20E000, 0x40E000, 0x60E000, 0x80E000, 0xA0E000, 0xC0E000,
    0xE0E000, 0x000040, 0x200040, 0x400040, 0x600040, 0x800040, 0xA00040,
    0xC00040, 0xE00040, 0x002040, 0x202040, 0x402040, 0x602040, 0x802040,
    0xA02040, 0xC02040, 0xE02040, 0x004040, 0x204040, 0x404040, 0x604040,
    0x804040, 0xA04040, 0xC04040, 0xE04040, 0x006040, 0x206040, 0x406040,
    0x606040, 0x806040, 0xA06040, 0xC06040, 0xE06040, 0x008040, 0x208040,
    0x408040, 0x608040, 0x808040, 0xA08040, 0xC08040, 0xE08040, 0x00A040,
    0x20A040, 0x40A040, 0x60A040, 0x80A040, 0xA0A040, 0xC0A040, 0xE0A040,
    0x00C040, 0x20C040, 0x40C040, 0x60C040, 0x80C040, 0xA0C040, 0xC0C040,
    0xE0C040, 0x00E040, 0x20E040, 0x40E040, 0x60E040, 0x80E040, 0xA0E040,
    0xC0E040, 0xE0E040, 0x000080, 0x200080, 0x400080, 0x600080, 0x800080,
    0xA00080, 0xC00080, 0xE00080, 0x002080, 0x202080, 0x402080, 0x602080,
    0x802080, 0xA02080, 0xC02080, 0xE02080, 0x004080, 0x204080, 0x404080,
    0x604080, 0x804080, 0xA04080, 0xC04080, 0xE04080, 0x006080, 0x206080,
    0x406080, 0x606080, 0x806080, 0xA06080, 0xC06080, 0xE06080, 0x008080,
    0x208080, 0x408080, 0x608080, 0x808080, 0xA08080, 0xC08080, 0xE08080,
    0x00A080, 0x20A080, 0x40A080, 0x60A080, 0x80A080, 0xA0A080, 0xC0A080,
    0xE0A080, 0x00C080, 0x20C080, 0x40C080, 0x60C080, 0x80C080, 0xA0C080,
    0xC0C080, 0xE0C080, 0x00E080, 0x20E080, 0x40E080, 0x60E080, 0x80E080,
    0xA0E080, 0xC0E080, 0xE0E080, 0x0000C0, 0x2000C0, 0x4000C0, 0x6000C0,
    0x8000C0, 0xA000C0, 0xC000C0, 0xE000C0, 0x0020C0, 0x2020C0, 0x4020C0,
    0x6020C0, 0x8020C0, 0xA020C0, 0xC020C0, 0xE020C0, 0x0040C0, 0x2040C0,
    0x4040C0, 0x6040C0, 0x8040C0, 0xA040C0, 0xC040C0, 0xE040C0, 0x0060C0,
    0x2060C0, 0x4060C0, 0x6060C0, 0x8060C0, 0xA060C0, 0xC060C0, 0xE060C0,
    0x0080C0, 0x2080C0, 0x4080C0, 0x6080C0, 0x8080C0, 0xA080C0, 0xC080C0,
    0xE080C0, 0x00A0C0, 0x20A0C0, 0x40A0C0, 0x60A0C0, 0x80A0C0, 0xA0A0C0,
    0xC0A0C0, 0xE0A0C0, 0x00C0C0, 0x20C0C0, 0x40C0C0, 0x60C0C0, 0x80C0C0,
    0xA0C0C0, 0xFFFBF0, 0xA0A0A4, 0x808080, 0xFF0000, 0x00FF00, 0xFFFF00,
    0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFFFF
};

struct SoundData{

    int format;
    int channels;
    int sample_rate;
    int byte_rate;
    int block_align;
    int bits_per_sample;

    int16_t * samples;
    u_int32_t num_samples;

};

float gaussian_window(float i, float N);
float sigmoid(float i, float N);
float window(float i , float N);
float minf(float a, float b);
float maxf(float a, float b);
int cmpfunc (const void * a, const void * b);
int decodeWAVE(char filename[], struct SoundData * sd);


/**
 * Graciously taken from plplot examples library, examples of 3d plots
 */
static void
cmap1_init( void )
{
      PLFLT i[2], h[2], l[2], s[2];

    i[0] = 0.0;         // left boundary
    i[1] = 1.0;         // right boundary

    h[0] = 240;         // blue -> green -> yellow ->
    h[1] = 40;           // -> red

    l[0] = 0.6;
    l[1] = 0.6;

    s[0] = 0.99;
    s[1] = 0.99;

    plscmap1n( 256 );
    c_plscmap1l( 0, 2, i, h, l, s, NULL );
}

int main(int argc, char * argv[]){

    //------------------------------------
    // Args
    //------------------------------------

    printf("\n\nARGS: ");
    for(int i = 0 ; i < argc ; i ++)
        printf("%s ", argv[i]);

    char * filename = "sweep.wav";

    printf("%d", argc);

    if(argc > 1){
        int len = strlen(argv[1]);
        //if ends in .wav
        if(len > 4 && strncmp(argv[1]+(len-4) , ".wav" , 4) == 0)
            filename = argv[1];
        //else we append .wav
        else
            filename = strcat(argv[1], ".wav");
    }
    printf("\nFILENAME: %s\n", filename);

    //------------------------------------
    // Read in WAVE file
    //------------------------------------
    
    struct SoundData * sd = (struct SoundData *) calloc(1, sizeof(struct SoundData));

    int status = decodeWAVE(filename, sd);

    if(status == -1){
        printf("decoding failed!");
        return 1;
    }

    //------------------------------------
    // FFT config variables
    //------------------------------------

    int WINDOW = 441 * 3 / 2;

    int N = 441 * 2;
    N += WINDOW;

    int MAG_DIV_SCALE = 4;

    int S = sd->num_samples/sd->channels;

    int NUM_CHUNKS = (S-N) / (N - WINDOW); //subtract N to keep OOB issue for later...
    NUM_CHUNKS *=  sd->channels;



    //------------------------------------
    // plplot config variables
    //------------------------------------

    plsdev("xwin"); 
    plinit(); 

    PLINT min_x = 0;
    PLINT max_x = NUM_CHUNKS;//(int)(SECONDS_PER_CHUNK * NUM_CHUNKS);///SAMPLES_PER_FFT;
    PLINT min_y = 0;
    PLINT max_y = (sd->sample_rate / 2); //max frequency bin is Fs / 2

    PLFLT * y = malloc(sizeof(PLFLT)*(N/2+1));
    PLFLT * x = malloc(sizeof(PLFLT)*(N/2+1));

    plenv(min_x, max_x, min_y, max_y, 0,0);

    char title[100]; 
    sprintf(title, "%s N=%d SAMPLES PER FFT=%d", filename, N-WINDOW,N);
    pllab("window #", "frequency (x10 kHz)", title);

    //setup color space for plplot
    cmap1_init();

    //------------------------------------
    // bitmap init
    //------------------------------------
    bmp_img img;
    bmp_img_init_df(&img, NUM_CHUNKS, N/2+1);


    //------------------------------------
    // FFT begin
    //------------------------------------

    double * in;
    fftw_complex * out;
    fftw_plan p;

    in = (double*)fftw_alloc_real(N);
    out = (fftw_complex*)fftw_alloc_complex(N/2+1);

    p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

    //do some work beforehand to speed up loops
    float window_precompute[N];
    for(int i = 0 ; i < N ; i++)
        window_precompute[i] = gaussian_window(i, N);

    float bin_size = (sd->sample_rate/(N));
    float half_bin_size = bin_size / 2;

    float max = 0;

    int loop = -1;
    while(++loop < NUM_CHUNKS){ 
    

        //go through the samples listed and put in array
        for(int i = 0 ; i < N; i++)
        {   
            in[i] = window_precompute[i] * sd->samples[(loop * (N-WINDOW)) + (i * sd->channels)];
        }

        fftw_execute_dft(p, in, out);

        
        //plot points
        for(int i = 0 ; i < (N / 2 + 1) ; i++)
        {

            float mag = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1])/MAG_DIV_SCALE;
            if(mag > max)
                max = mag;

            //---------------------------------
            x[i] = (PLFLT) (loop); //((sd->sample_rate / SAMPLES_PER_FFT));

            //TODO dividing by 4 cleans up image a lot, we need a better color mapping function than sigmoid/linear
        
            y[i] = i * bin_size + half_bin_size;

            float color_f = (mag) / (sd->sample_rate);

            color_f = minf(color_f, 1);
            //float color = sigmoid(mag, sd->sample_rate);
            plcol1( color_f);
            plpoin(1, x+i,y+i, -1);
            //----------------------------------------
    
            // int color_index = 64 * mag / sd->sample_rate;
            // color_index *= (128 / 64);

            // color_index = minf(128/2, color_index) + 64;

            // int color = colors[(int)color_index];

            // int r = color / 0x010000;
            // int g = (color % 0x010000) / 0x00100;
            // int b = color % 0x000100;

            // bmp_pixel_init(&img.img_pixels[(img.img_header.biHeight - i - 1)][loop], r , g , b);
        //   //  (img).img_pixels[loop/2 - 1][i/2] = BMP_PIXEL(0,0,0);

        }
    }
    plend();

    printf("MAX: %f", max);

    bmp_img_write (&img, "test.bmp");
	bmp_img_free (&img);


    free(in);
    free(out);

    free(x);
    free(y);

    free(sd->samples);
    free(sd);

    fftw_destroy_plan(p);
    fftw_cleanup();


    exit(0);

}

float gaussian_window(float i, float N){
    float sigma = .45;
    return exp(-.1*pow((i-N/2)/(sigma * N/2),2));
}

float sigmoid(float i, float N){
    return 1 / (1 + exp(-.5 * (i - N/2)));
}

float window(float i , float N){
    return .5 * ( 1 + cos(2 * PI * i / N ));
}

float minf(float a, float b)
{
    return a < b ? a : b;
}

float maxf(float a, float b)
{
    return a > b ? a : b;
}

int decodeWAVE(char filename[], struct SoundData * sd){
    FILE *file = fopen(filename, "r");


    char buffer[20];
    
    int s; //to avoid warnings

    //check header info is valid
    s = fread(&buffer, sizeof(char), 4, file);
    if(strncmp(buffer, "RIFF", 4) != 0){
        printf("AAAAAAAAA");
        return -1;
    }

    u_int32_t filesize;
    s = fread(&filesize, sizeof(int), 1, file);
    printf("File size: %u\n", filesize);

    s = fread(&buffer, sizeof(char), 8, file);
    if(strncmp(buffer, "WAVEfmt", 7) != 0){
        printf("BBBBBBB");
        return -1;
    }

    //read wav format information
    
    //fread(&format_size, 4, 1, file);
    fseek(file, 4, 1); //seek past format chunk size since we don't need it

    s = fread(&sd->format, 2, 1, file);
    s = fread(&sd->channels, 2, 1, file);
    s = fread(&sd->sample_rate, 4, 1, file);
    s = fread(&sd->byte_rate, 4, 1, file);
    s = fread(&sd->block_align, 2, 1, file);
    s = fread(&sd->bits_per_sample, 2, 1, file);

    

    //read in next header
    s = fread(&buffer, sizeof(char), 4, file);

    int size;  //generalize to a method for each header type? not rly needed for my use case
    while(strncmp(buffer, "data", 4) != 0) //we only care about data
    {
        s = fread(&size, sizeof(int), 1, file);
        fseek(file, size, 1);
        s = fread(&buffer, sizeof(char), 4, file);
    }

    //now let's read in the audio data, rn we're assuming 16 bit WAV - need to update that later for 24 / 32 bit
    //each sample is a signed 16 bit num relating to intensity from 0 -> 2^16
    int data_size;
    s = fread(&data_size, sizeof(int), 1, file);
    printf("datasize: %d\n", data_size);

    // bytes / [(bits/s) / (bits/byte)] => bytes / [bytes / s] => s
    u_int32_t num_samples = data_size / (sd->bits_per_sample / 8);

    printf("num_samples: %d\n", num_samples);

    
    int16_t * samples = malloc(num_samples * sizeof(int16_t));
    s = fread(samples, sizeof(int16_t), num_samples, file);
    sd->samples = samples;
    sd->num_samples = num_samples;

    printf("\n------------------\nformat: %d \nchannels: %d\nsample_rate: %d\nbyte_rate: %d\nblock_align: %d\nbits_per_sample: %d\n",sd->format,sd->channels,sd->sample_rate,sd->byte_rate,sd->block_align,sd->bits_per_sample);
    return 0;

}



