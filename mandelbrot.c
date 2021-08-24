#define _XOPEN_SOURCE 700
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BLACK           40
#define RED             41
#define GREEN           42
#define YELLOW          43
#define BLUE            44
#define MAGENTA         45
#define CYAN            46
#define WHITE           47
#define GRAY            100
#define LIGHT_GREEN     102
#define LIGHT_YELLOW    103
#define DARK_BLUE       104
#define LIGHT_MAGENTA   105
#define LIGHT_CYAN      106
#define LIGHT_GRAY      107

static int colors[] = {
   BLACK,
   DARK_BLUE,
   BLUE,
   CYAN,
   LIGHT_CYAN,
   MAGENTA,
   LIGHT_MAGENTA,
   GREEN,
   LIGHT_GREEN,
   RED,
   YELLOW,
   LIGHT_YELLOW,
   GRAY,
   LIGHT_GRAY,
   WHITE,
   BLACK,
};

static int roundi(float f) {
   const float dec = f - (int)f;
   return dec >= 0.5f ? (int)f + 1 : (int)f;
}

static void termsize(int* w, int* h) {
   struct winsize ws;
   if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) != 0) {
      fputs("failed to get terminal size, guessing default (80x25)\n", stderr);
      *w = 80;
      *h = 25;
   } else {
      *w = ws.ws_col;
      *h = ws.ws_row;
   }
}

int main(int argc, char** argv) {
   char* endp;
   long max_it;
   int w, h;
   int option;
   termsize(&w, &h);
   max_it = w / 2;
   fprintf(stderr, "w=%d, h=%d, max_it=%ld\n", w, h, max_it);
   while ((option = getopt(argc, argv, ":hs:m:")) != -1) {
      switch (option) {
      case 'm':
         max_it = strtol(optarg, &endp, 10);
         if (*endp || max_it <= 0) {
            fprintf(stderr, "mandelbrot: invalid number '%s'\n", optarg);
            return 1;
         }
         break;
      case 's':
         w = (int)strtol(optarg, &endp, 10);
         if (*endp != 'x' || w <= 0) {
            fprintf(stderr, "mandelbrot: invalid size '%s'\n", optarg);
            return 1;
         }
         ++endp;
         h = (int)strtol(endp, &endp, 10);
         if (*endp || h <= 0) {
            fprintf(stderr, "mandelbrot: invalid size '%s'\n", optarg);
            return 1;
         }
         break;
      case 'h':
         puts("Usage: mandelbrot [-s <width>x<height>] [-m <max_iterations>] [-h]");
         return 0;
      case '?':
         fprintf(stderr, "mandelbrot: invalid option '-%c'\n", optopt);
         return 1;
      case ':':
         fprintf(stderr, "mandelbrot: missing argument for option '-%c'\n", optopt);
         return 1;
      }
   }

   for (int py = 0; py < h; ++py) {
      for (int px = 0; px < w; ++px) {
         const float x0 = ((float)px / (float)w * 3.5f) - 2.1f;
         const float y0 = ((float)py / (float)h * 2.0f) - 1.0f;

         float x = 0.0f, y = 0.0f;
         int it = 0;
         while (x*x + y*y <= 4 && it < max_it) {
            const float tmp = x*x - y*y + x0;
            y = 2*x*y + y0;
            x = tmp;
            ++it;
         }
         const int ncolors = sizeof(colors) / sizeof(int) - 1;
         const int color = colors[roundi((float)it / (float)max_it * (float)ncolors)];
         printf("\033[%dm ", color);
         fflush(stdout);
      }
      printf("\033[0m\n");
   }

   return 0;
}
