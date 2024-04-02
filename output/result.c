#include <stdio.h>
int main(void){
auto a = 0;
while (a < 1){
printf("Enternumberofscores:");
scanf("%v",&a);
}
auto b = 0;
auto s = 0;
printf("Enteronevalueatatime:");
while (b < a){
scanf("%v",&c);
auto s = s + c;
auto b = b + 1;
}
printf("Average:");
printf(s / a);
return 0;
}