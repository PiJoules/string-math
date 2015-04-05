/**
 * For testing string_math.c
 */


#include <stdio.h>
#include "sds.h"
#include "string_math.h"

void testAddNumers();
void testSubtractNumbers();
void testMultiplyNumbers();


int main(){
	testAddNumers();
	testSubtractNumbers();
	testMultiplyNumbers();

	return 0;
}


void testMultiplyNumbers(){
	printf("\nmultiplyNumbers test\n");
	int j;
	for (j = 0; j < 1; j++){
		sds product = sdsnew("1");
		unsigned int i;
		for (i = 0; i < 1000; i++){
			sds num1 = sdsnew("2");
			sds tempProduct = multiplyNumbers(product, num1);
			
			// overwrite old value of sum
			sdstrim(product, product);
			product = sdscat(product, tempProduct);

			sdsfree(tempProduct);
			sdsfree(num1);
		}
	    printf("test %d: \t%s\n", j+1, product);
	    sdsfree(product);
	}
}


void testSubtractNumbers(){
	printf("\nsubtractNumbers test\n");
	int j;
	for (j = 0; j < 10; j++){
		sds diff = sdsnew("0");
		unsigned int i;
		for (i = 0; i < 1000; i++){
			sds num1 = sdsnew("398481923674987123649237469127364912639481263478623497296781263894");
			sds tempDiff = subtractNumbers(diff, num1);
			
			// overwrite old value of sum
			sdstrim(diff, diff);
			diff = sdscat(diff, tempDiff);

			sdsfree(tempDiff);
			sdsfree(num1);
		}
	    printf("test %d: \t%s\n", j+1, diff);
	    sdsfree(diff);
	}
}


void testAddNumers(){
	printf("\naddNumbers test\n");
	int j;
	for (j = 0; j < 10; j++){
		sds sum = sdsnew("0");
		unsigned int i;
		for (i = 0; i < 1000; i++){
			sds num1 = sdsnew("398481923674987123649237469127364912639481263478623497296781263894");
			sds tempSum = addNumbers(sum, num1);
			
			// overwrite old value of sum
			sdstrim(sum, sum);
			sum = sdscat(sum, tempSum);

			sdsfree(tempSum);
			sdsfree(num1);
		}
	    printf("test %d: \t%s\n", j+1, sum);
	    sdsfree(sum);
	}
}


