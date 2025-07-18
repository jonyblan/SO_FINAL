#include <stdint.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

uint64_t strlen(const char *string) {
  uint64_t i = 0;
  while (string[i] != '\0') {
    i++;
  }
  return i;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

char *safe_strncpy(char *dest, const char *src, uint8_t max) {
    uint8_t i = 0;
    while (i < max - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; // Ensure null termination
    return dest;
}

char *my_strcat(char *dest, const char *src) {
    char *ptr = dest;

    // Avanzar hasta el final de dest
    while (*ptr != '\0') {
        ptr++;
    }

    // Copiar src al final de dest
    while (*src != '\0') {
        *ptr = *src;
        ptr++;
        src++;
    }

    // Agregar el null terminator final
    *ptr = '\0';

    return dest;
}

void itoa(int value, char *str) {
    char buffer[12]; // suficiente para int de 32 bits (-2147483648 a 2147483647)
    int i = 0;
    int isNegative = 0;

    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if (value < 0) {
        isNegative = 1;
        value = -value;
    }

    while (value > 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (isNegative)
        buffer[i++] = '-';

    // Revertir el buffer en str
    int j = 0;
    while (i > 0)
        str[j++] = buffer[--i];
    
    str[j] = '\0';
}