#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		return 1;
	}

	FILE *file = fopen(argv[1], "r");
	unsigned char bytes[3];

	fread(bytes, 3, 1, file);

	if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff)
	{
		printf("The file %s is a .jpg file", argv[1]);
	}
	else
	{
		printf("The file %s is not a .jpg file", argv[1]);
	}
}
