
#include <string.h>
#include "sds.h"


int charToInt(char);
int compare(char*, char*);
void reverse(sds);
sds intToString(int);
sds prepend(char*, sds);
sds substr(sds, int, int);
sds padZeroes(char*, char*);
sds addNumbers(sds, sds);
sds subtractNumbers(sds, sds);
sds multiplyNumbers(sds, sds);


int charToInt(char c){
	return c - '0';
}

/**
 * Add zeroes in front of num1 (which should be positive) until it is equal to the length of num2
 */
sds padZeroes(char* num1, char* num2){
	sds dup = sdsdup(num1);
	int len = strlen(num2);
	while (sdslen(dup) < len){
		dup = prepend("0", dup);
	}
	return dup;
}

sds intToString(int num){
	if (num == 0){
		sds str = sdsnew("0");
		return str;
	}

	sds str = sdsempty();
 	int i, rem, len = 0, n = num;
 
    while (n != 0){
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++){
        rem = num % 10;
        num /= 10;

        char temp[2];
        temp[0] = rem + '0';
        temp[1] = '\0';
        str = prepend(temp, str);
    }
    return str;
}

void reverse(sds s){
	struct sdshdr *sh = (void*)(s-sizeof *sh);
	sds temp = sdsdup(s);
	int i, lastIndex = (int)sdslen(s)-1;
	for (i = lastIndex; i >= 0; i--){
		sh->buf[lastIndex-i] = temp[i];
	}
	sdsfree(temp);
}

sds prepend(char* t, sds s){
	sds result = sdscat(s, t);
	struct sdshdr *sh = (void*)(result-sizeof *sh);
	sds temp = sdsdup(result);

	int i, len = (int)sdslen(result), tLen = strlen(t);
	for (i = 0; i < len; i++){
		if (i < tLen){
			sh->buf[i] = t[i];
		}
		else {
			sh->buf[i] = temp[i-tLen];
		}
	}

	sdsfree(temp);
	return result;
}

/**
 * Compares two string numbers.
 *
 * @return number 1 if num1 > num2, 0 if num1 == num2, -1 if num1 < num2.
 */
int compare(char* n1, char* n2){
	sds num1 = sdsnew(n1);
	sds num2 = sdsnew(n2);

	sdstrim(num1, "0");
	sdstrim(num2, "0");

	if (strcmp(num1, num2) == 0){
		sdsfree(num1);
		sdsfree(num2);
		return 1; // numbers are equal
	}

	int num1IsNegative = (num1[0] == '-');
	int num2IsNegative = (num2[0] == '-');

	int sign = 1;
	if (!num1IsNegative && num2IsNegative){
		sdsfree(num1);
		sdsfree(num2);
		return 1;
	}
	else if (num1IsNegative && !num2IsNegative){
		sdsfree(num1);
		sdsfree(num2);
		return -1;
	}
	else if (num1IsNegative && num2IsNegative){
		sds sub1 = substr(num1, 1, sdslen(num1));
		sds sub2 = substr(num2, 1, sdslen(num2));
		sdstrim(num1, num1);
		sdstrim(num2, num2);
		num1 = sdscat(num1, sub1);
		num2 = sdscat(num2, sub2);
		sdsfree(sub1);
		sdsfree(sub2);
		
		sign = -1;
	}

	sds paddedNum1 = padZeroes(num1, num2);
	sdstrim(num1, num1);
	num1 = sdscat(num1, paddedNum1);
	sdsfree(paddedNum1);
	sds paddedNum2 = padZeroes(num2, num1);
	sdstrim(num2, num2);
	num2 = sdscat(num2, paddedNum2);
	sdsfree(paddedNum2);

	int len = sdslen(num1);
	int i;
	for (i = 0; i < len; i++){
		int digit1 = charToInt(num1[i]);
		int digit2 = charToInt(num2[i]);

		if (digit1 > digit2){
			sdsfree(num1);
			sdsfree(num2);
			return sign;
		}
		else if (digit1 < digit2){
			sdsfree(num1);
			sdsfree(num2);
			return sign * -1;
		}
	}

	sdsfree(num1);
	sdsfree(num2);
	return 0;
}

sds substr(sds s, int start, int end){
	sds sub = sdsempty();

	int i;
	for (i = start; i < end; i++){
		char temp[2];
		temp[0] = s[i];
		temp[1] = '\0';
		sub = sdscat(sub, temp);
	}

	return sub;
}

sds addNumbers(sds num1, sds num2){
	sds result = sdsempty();

	int num1IsNegative = (num1[0] == '-');
	int num2IsNegative = (num2[0] == '-');

	if (num1IsNegative && num2IsNegative){
		sds num1Sub = substr(num1, 1, sdslen(num1));
		sds num2Sub = substr(num2, 1, sdslen(num2));
		
		sds sum = addNumbers(num1Sub, num2Sub);
		result = sdscat(result, sum);
		sdsfree(sum);
		result = prepend("-", result);

		sdsfree(num1Sub);
		sdsfree(num2Sub);
		return result;
	}
	else if (num1IsNegative && !num2IsNegative){
		//return subtractNumbers( num2, substr(num1, 1, strlen(num1)-1) );
		sds sub = substr(num1, 1, sdslen(num1));
		sds diff = subtractNumbers(num2, sub);
		result = sdscat(result, diff);
		sdsfree(diff);
		sdsfree(sub);

		return result;
	}
	else if (!num1IsNegative && num2IsNegative){
		//return subtractNumbers( num1, substr(num2, 1, strlen(num2)-1) );
		sds sub = substr(num2, 1, sdslen(num2));
		sds diff = subtractNumbers(num1, sub);
		result = sdscat(result, diff);
		sdsfree(diff);
		sdsfree(sub);

		return result;
	}

	int i, j, carry;
	for (i = sdslen(num1)-1, j = sdslen(num2)-1, carry = 0; i >= 0 || j >= 0 || carry != 0; i--, j--){
		int digit1;
		if (i < 0)
			digit1 = 0;
		else
			digit1 = charToInt(num1[i]);

		int digit2;
		if (j < 0)
			digit2 = 0;
		else
			digit2 = charToInt(num2[j]);

		int digit = digit1 + digit2 + carry;
		if (digit > 9){
			carry = 1;
			digit -= 10;
		}
		else {
			carry = 0;
		}

		sds digitStr = intToString(digit);
		result = sdscat(result, digitStr);
		sdsfree(digitStr);
	}

	reverse(result);
	return result;
}

sds subtractNumbers(sds num1, sds num2){
	sds result = sdsempty();

	int num1IsNegative = (num1[0] == '-');
	int num2IsNegative = (num2[0] == '-');

	if (num2IsNegative){
		sds sub = substr(num2, 1, sdslen(num2));
		sds sum = addNumbers(num1, sub);
		result = sdscat(result, sum);
		sdsfree(sub);
		sdsfree(sum);

		return result;
	}
	else if (num1IsNegative){
		sds sub = substr(num1, 1, sdslen(num1));
		sds sum = addNumbers(sub, num2);
		result = sdscat(result, sum);
		sdsfree(sum);
		sdsfree(sub);

		result = prepend("-", result);

		return result;
	}
	else if (compare(num1, num2) == -1){
		sds diff = subtractNumbers(num2, num1);
		result = sdscat(result, diff);
		sdsfree(diff);

		result = prepend("-", result);

		return result;
	}

	// After this point, num1 > num2 

	int i, j, carry;
	for (i = sdslen(num1)-1, j = sdslen(num2)-1, carry = 0; i >= 0 || j >= 0 || carry != 0; i--, j--){
		int digit1;
		if (i < 0)
			digit1 = 0;
		else
			digit1 = charToInt(num1[i]);

		int digit2;
		if (j < 0)
			digit2 = 0;
		else
			digit2 = charToInt(num2[j]);

		int digit = digit1 - digit2 + carry;
		if (digit < 0){
			digit += 10;
			carry = -1;
		}
		else {
			carry = 0;
		}

		sds digitStr = intToString(digit);
		result = sdscat(result, digitStr);
		sdsfree(digitStr);
	}

	reverse(result);
	return result;
}

sds multiplyNumbers(sds num1, sds num2){
	sds result = sdsempty();

	int num1IsNegative = (num1[0] == '-');
	int num2IsNegative = (num2[0] == '-');

	if (num1IsNegative && num2IsNegative){
		sds sub1 = substr(num1, 1, sdslen(num1));
		sds sub2 = substr(num2, 1, sdslen(num2));
		sds product = multiplyNumbers(sub1, sub2);
		result = sdscat(result, product);
		sdsfree(product);
		sdsfree(sub1);
		sdsfree(sub2);

		return result;
	}
	else if (num1IsNegative && !num2IsNegative){
		sds sub1 = substr(num1, 1, sdslen(num1));
		sds product = multiplyNumbers(sub1, num2);
		result = sdscat(result, product);
		sdsfree(product);
		sdsfree(sub1);

		result = prepend("-", result);
		return result;
	}
	else if (!num1IsNegative && num2IsNegative){
		sds sub2 = substr(num2, 1, sdslen(num2));
		sds product = multiplyNumbers(num1, sub2);
		result = sdscat(result, product);
		sdsfree(product);
		sdsfree(sub2);

		result = prepend("-", result);
		return result;
	}

	int i, j, len1 = (int)sdslen(num1), len2 = (int)sdslen(num2);
	for (i = 0; i < len1; i++){
		int digit1 = charToInt(num1[i]);

		if (digit1 == 0)
			continue;

		int num1Place = len1-1-i;
		if (num1Place < 0)
			num1Place++;

		for (j = 0; j < len2; j++){
			int digit2 = charToInt(num2[j]);

			if (digit2 == 0)
				continue;

			int num2Place = len2-1-j;
			if (num2Place < 0)
				num2Place++;

			sds subProduct = intToString(digit1*digit2);
			int subProductPlace = num1Place + num2Place;

			int k;
			for (k = 0; k < subProductPlace; k++){
				char temp[2];
				temp[0] = '0';
				temp[1] = '\0';
				subProduct = sdscat(subProduct, temp);
			}

			sds sum = addNumbers(result, subProduct);
			sdstrim(result, result);
			result = sdscat(result, sum);
			sdsfree(sum);
			sdsfree(subProduct);
		}
	}

	return result;
}