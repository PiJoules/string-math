# String Math
A library that will be made in multiple languages for doing math with large numbers as strings. You wouldn't normally be able to add `723645862349523984563945692836598326459832659832675` and `398756823476592384756893247652346589324659834659365932489` in C or Java. Use this basic library for adding/subtracting/multiplying/dividing these two numbers as strings and get the output as a string.

## C
Be sure to first download [sds](https://github.com/antirez/sds) and place the `sds.c` and `sds.h` in the same directory as your project.
Include
```C
#include "sds.h"
#include "string_math.h"
```
in your project.

To multiply two numbers, you must create them as `sds` objects. Don't forget to free them when you're done using them with the `sdsfree()` function.
```C
sds num1 = sdsnew("37491656398456238945698345");
sds num2 = sdsnew("937598456239645967892354");
sds sum = addNumbers(num1, num2);
printf("%s\n", sum); // prints 38429254854695884913590699
sdsfree(sum);
sdsfree(num1);
sdsfree(num2);
```

## Dependencies
- [sds](https://github.com/antirez/sds)