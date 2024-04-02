#include <stdio.h>
int main(void){
float a = 0;
while (a < 1){
printf("Enter number of scores: ");
scanf("%v", &a);
}
float b = 0;
float s = 0;
printf("Enter one value at a time: ");
while (b < a){
scanf("%v", &c);
s = s + c;
b = b + 1;
}
printf("Average: ");
printf(s / a);
return 0;
}