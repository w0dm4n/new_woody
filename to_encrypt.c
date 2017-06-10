#include <stdio.h>

void call_patate(char *patate)
{
	printf("%s\n", patate);
}

int	main(int argc, char **argv)
{
	char bonjour[10] = "Hey !\0";
	char petite_patate[256] = "\nDe nos jours la vie est dur, quand on est une petite patate\0";
	char petite_patate_suite[256] = "Alors mon p'tit gars, si t'es une petite patate comme moi\0";
	char casserole[256] = "Retiens bien ses paroles, si tu veux pas finir a la casserole\0";
	printf("%s", bonjour);
	call_patate(petite_patate);
	call_patate(petite_patate_suite);
	call_patate(casserole);
}
