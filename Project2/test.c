#include <stdio.h>

int main(void)
{
  int phoneNumPrint[10];
  int count = 9;
  int i;
  
  while(phoneNumber >= 1)
    {
      phoneNumPrint[count] = phoneNumber % 10;
      phoneNumber = phoneNumber / 10;
      count = count -1;
    }

  printf("\n\n(");
  for(i=0;i<10;i++)
    {
      if(i==3)
      printf(")");
      else if(i==6)
      printf("-");
      printf("%d",phoneNumPrint[i]);
    }

    return 0;
}
