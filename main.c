/*
 * File:   main.c
 * Compiled executable name: param
 * Compiled using: gcc
 * Author: mahel.berthod & francesc.saurina
 *
 * Created on 15 de febrero de 2024, 10:16
 */

 /*
 WARNING: When counting number of apparitions of a letter, we do not count the first apparition.
 Ex- yoyoyo will return 2 (not 3)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /*Library that contains the write function. Does not work if not called*/

/*
 * Prototyping functions
 */
int main(int argc, char **argv);
int StringCompare(char *sr1, char *sr2);
int StringLength(char *sr);
int AsciiToInteger(char *sr1);
ssize_t write(int fildes, const void *buf, size_t nbyte);

/*
* Main function
*/
int main(int argc, char **argv)
{
  /* Variable declaration */
  int length;
  int i = 0, num = 0, rm = 0;
  char hexa, firstCar, firstCarBis;
  int temp=0, lgthNum=0;
  if (argc == 1)
  {
    /*No params were given. Show error*/
    write(1, "params action param1 [param2]\n", 30);
    return EXIT_FAILURE;
  }
  if (StringCompare(argv[1], "compare") == 0)
  {
    /*action = compare*/
    switch (StringCompare(argv[2], argv[3]))
    {
    case 0:
      /*param1 = param2*/
      write(1, "Param 1 and Param 2 are the same\n", 33);
      break;
    case 1:
      /*param1 > param2*/
      write(1, "Param 1 > Param 2\n", 18);
      break;
    case -1:
      /*param1 < param2*/
      write(1, "Param 1 < Param 2\n", 18);
      break;
    default:
      /*Throw error*/
      write(1, "An error happened\n", 18);
      return EXIT_FAILURE;
      break;
    }
  }
  else if (StringCompare(argv[1], "number") == 0)
  {
    /*if action = number*/
    length = StringLength(argv[2]);
    /* Get string length to convert number in 
    string format to Number format */
    char nbHex[length+1];
    for (i = 0; i < length; i++)
    {
      num = num * 10;
      num += AsciiToInteger(&argv[2][i]);
    }
    if (num > 0)
    {
      /*If number obtained is > 0,
      return dec number to hex */
      i = 0;
      while (num > 0)
      {
        int remainder = num % 16; /* 16 for HEX */

        if (remainder < 10)
          hexa = remainder + '0';
        else
          hexa = remainder + 'A' - 10;
        num /= 16;
        nbHex[length-i]=hexa;
        i++;
      }
      nbHex[length+1]='\n';
      write(1, nbHex, length+2);
    }
  }
  else
  {
    /*Action is neither compare nor num*/
    firstCar = argv[1][0]; /*Get the first character*/
    /* FirstCarBis will contain the charater in minuscule (if firstCar is majuscule)
    and vice versa. */
   if (firstCar > 'A' && firstCar < 'Z')
      firstCarBis = firstCar + ('a' - 'A');
    else if (firstCar > 'a' && firstCar < 'z')
      firstCarBis = firstCar - ('a' - 'A');
    else
    {
      /* firstCar is neither a majuscule nor a minuscule */
      write(1, "Error\n", 6);
      return EXIT_FAILURE;
    }
    i = 0;
    num = 0; /* To count number of apparitions*/
    length = StringLength(argv[1]);
    for (i = 1; i < length; i++)
    {
      if (argv[1][i] == firstCar || argv[1][i] == firstCarBis){
        /*Letter matches (minuscule or majuscule)*/
        num++;
      }
    }
    /*Conversion of type number to type string*/
    temp=num; /* Stores the number */
    lgthNum=0;
    /* Counts the length of the resulting string
    ex: 42 -> length=2; 123 -> length=3 ... */
    while(temp>0){
      temp = temp/10;
      lgthNum++;
    }
    char nbTemp[lgthNum+1]; /*Will store the number in str format*/
    temp = num;
    for(i=lgthNum-1; i>=0; i--){
      nbTemp[i] = (temp % 10) + '0'; /*Convert number to str (using ascii table) */
      temp = temp / 10;
    }
    nbTemp[lgthNum] = '\n'; /* For purely esthetical purposes*/
    write(1, nbTemp, lgthNum+1); /*Shows result*/
  }
  return (EXIT_SUCCESS);
}

int StringCompare(char *sr1, char *sr2)
{
  int i = 0, brk = 0, isEq=1;
  while (sr1[i] != '\0' && sr2[i] != '\0')
  {
    char carBis;
    if(sr2[i]>='a'){
      /* Is minuscule -> carBis is majuscule*/
      carBis = sr2[i]-('a' - 'A');
    }
    else if(sr2[i]>='A'){
      /* Is majuscule -> carBis is minuscule*/
      carBis = sr2[i]+('a' - 'A');
    }
    if(sr1[i] != sr2[i] && sr1[i] != carBis && brk==0){
      brk=-2;
    }
    i++;
  }
  if (sr1[i] == '\0' && sr2[i] != '\0')
    brk = -1;
  else if (sr2[i] == '\0' && sr1[i] != '\0')
    brk = 1;
  return brk;
}

int AsciiToInteger(char *sr1)
{
  if (*sr1 < '0' || *sr1 > '9'){
    //str representation is not a number
    return -1;
  }
  return (*sr1) - '0'; //Return single digit in Number form
}

int StringLength(char *sr)
{
  int i = 0;
  while (sr[i] != '\0')
  {
    i++;
  }
  return i;
}