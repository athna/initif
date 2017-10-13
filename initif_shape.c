#include <stdio.h>

int main( int argc, char *argv[] ){
	FILE *fp;
	char *input_fname=argv[1];

	puts("start initial_shape.c");
	puts("This program arrange the shape of input data")
	printf("input file is '%s'\n",input_fname);

	/* file open */
    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "%sのオープンに失敗しました.\n", filename);
        exit(EXIT_FAILURE);
    }

	/*file scan*/
	while()

	return (0);
}