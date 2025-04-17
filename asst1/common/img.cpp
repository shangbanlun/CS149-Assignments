#include <png.h>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <algorithm>


void writePNGImage(int* data, int width, int height, const char* filename, int maxIterations) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("File opening failed");
        return;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        fprintf(stderr, "png_create_write_struct failed\n");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        fprintf(stderr, "png_create_info_struct failed\n");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        fprintf(stderr, "Error during png creation\n");
        return;
    }

    png_init_io(png_ptr, fp);

    // Set image attributes
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        8, // Bit depth
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for row pointers
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    if (!row_pointers) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)malloc(width);
        if (!row_pointers[y]) {
            for (int k = 0; k < y; k++) free(row_pointers[k]);
            free(row_pointers);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            fclose(fp);
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }
        for (int x = 0; x < width; x++) {
            int value = data[y * width + x];
            // Clamp the value to [0, 255]
            if (value < 0) value = 0;
            if (value > 255) value = 255;

            float mapped = pow(std::min(static_cast<float>(maxIterations), static_cast<float>(value)) / 256.f, .5f);

            // convert back into 0-255 range, 8-bit channels
            unsigned char result = static_cast<unsigned char>(255.f * mapped);

            row_pointers[y][x] = static_cast<png_byte>(result);
        }
    }

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, nullptr);

    // Free memory
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}
