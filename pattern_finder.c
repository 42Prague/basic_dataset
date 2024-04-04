#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>

void process_image(const char *filename, struct jpeg_decompress_struct *cinfo, unsigned char **buffer)
{
    FILE * infile = fopen(filename, "rb");
    if (infile == NULL) {
        fprintf(stderr, "Cannot open %s\n", filename);
        return;
    }

    jpeg_stdio_src(cinfo, infile);

    (void) jpeg_read_header(cinfo, TRUE);

    (void) jpeg_start_decompress(cinfo);
	
    *buffer = malloc(cinfo->output_width * cinfo->output_height * cinfo->output_components);

    while (cinfo->output_scanline < cinfo->output_height) {
        unsigned char *buffer_array[1];
        buffer_array[0] = *buffer + cinfo->output_scanline * cinfo->output_width * cinfo->output_components;
        jpeg_read_scanlines(cinfo, buffer_array, 1);
    }

    (void) jpeg_finish_decompress(cinfo);

    fclose(infile);
}

int is_match(unsigned char *pattern_buffer, unsigned char *image_buffer, int image_row_size, int pattern_width, int pattern_row_size, int pattern_height)
{
	int tolerance = 5;
	
	for (int y = 0; y < pattern_height; y++)
    {
		for (int x = 0; x < pattern_width; x++)
        {
			for (int c = 0; c < 3; c++)  // Loop over the three color components
            {
                char px_pattern = pattern_buffer[y * pattern_row_size + x * 3 + c];
                char px_image = image_buffer[y * image_row_size + x * 3 + c];
                if (abs(px_pattern - px_image) > tolerance)
                    return 0;
            }
        }
    }
    return (1);
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pattern.jpg> <image.jpg>\n", argv[0]);
        return (-1);
    }

    struct jpeg_decompress_struct cinfo1, cinfo2;
    struct jpeg_error_mgr jerr1, jerr2;

    cinfo1.err = jpeg_std_error(&jerr1);
    jpeg_create_decompress(&cinfo1);

    cinfo2.err = jpeg_std_error(&jerr2);
    jpeg_create_decompress(&cinfo2);

    unsigned char *buffer1, *buffer2;

    process_image(argv[1], &cinfo1, &buffer1);  // Process the pattern image
    process_image(argv[2], &cinfo2, &buffer2);  // Process the search image

	unsigned int matches_found = 0;

	printf("Pattern size: %d x %d pixels;\n", cinfo1.output_width, cinfo1.output_height);
	printf("Image size: %d x %d pixels;\n", cinfo2.output_width, cinfo2.output_height);
	printf("---------\n");

	for (unsigned int y = 0; y <= cinfo2.output_height; y++)
	{
		for (unsigned int x = 0; x <= cinfo2.output_width; x++)
	    {
			if (is_match(buffer1,
				buffer2 + ((y * cinfo2.output_width) + x) * cinfo2.output_components,
				cinfo2.output_width * cinfo2.output_components,
				cinfo1.output_width,
				cinfo1.output_width * cinfo1.output_components,
				cinfo1.output_height))
				{
					matches_found++;
					printf("Match found at position (x: %d,y: %d);\n", x, y);
				}
	    }
	}

	if (matches_found > 0)
		printf("Matches found: %d.\n", matches_found);
	else
		printf("No matches found.\n");

    free(buffer1);
    free(buffer2);

    jpeg_destroy_decompress(&cinfo1);
    jpeg_destroy_decompress(&cinfo2);

    return 0;
}