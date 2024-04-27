#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Lenora Tairova B00865751
//Sources:
// [1] Narang, P. (2022, November 21). Playfair cipher program in C. Scaler Topics. https://www.scaler.com/topics/playfair-cipher-program-in-c/ 
//Method 1: accept secret key (string) as argument and gen/return a 2D key matrix
//Method 2: accept plaintext (string) as argument and gen/return ciphertext
//Method 3: accept ciphertext and key matrix as argument and gen/return plaintext


//Function to remove spaces of plaintext - with help of scaler.com [1]
int removeSpace(char* plaintext){
	int count = 0;
	int i;
	//printf("HERE!");
	//check for spaces around entire plaintext
	//changed from i<plaintextLen
	for(i = 0; plaintext[i] != '\0'; i++){
		//if no space, then set next plaintext symbol to be the current
		if(plaintext[i]!=' '){
			//printf("HERE2");
			plaintext[count++]=plaintext[i];
		}
	
	}
	plaintext[count]='\0';
	return count;
}

//Method 1: Generate the 5x5 2d Matrix Key:
void genKeyTable(char key[], char keyTable[5][5]){
	char alphabet[26] = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
	int keyLen = strlen(key);
	//index of alphabet
	int index = 0;
	//initialize all usedLetter elements to 0
	int usedLetter[26] = {0};

	//Make 2D Matrix with non-repeating key letters
	for(int i = 0; i < keyLen; i++){
		//ASCCI 'A'- subtracting it will convert to a value btwn 0 and 25
		//if the subtraction evaluates to 0, it is not used, otherwise it is used
		if(!usedLetter[key[i] - 'A']){
			//align current letter to next available position in the table
			keyTable[index/5][index%5] = key[i];
			//mark letter as used
			usedLetter[key[i] - 'A'] = 1;
			//go to next letter in key
			index++;

		}
	}

	//Fill up remaining letters into matrix
	for(int i = 0; i<26; i++){
		if(!usedLetter[alphabet[i] - 'A']){
			keyTable[index/5][index%5]=alphabet[i];
			index++;
		}
	} 
}

//Method 2: Encrypt plaintext to ciphertext
void encrypt(char plaintext[], char keyTable[5][5]){
	int plaintextLen = strlen(plaintext);
	//add two to i as we increment in PAIRS
	for(int i = 0; i < plaintextLen; i+=2){
		//two rows + cols to form "Square"
		int row1, row2, col1, col2;
		//loop through 2D Key table
		for(int j = 0; j <5; j++){
			for(int k = 0; k <5; k++){
				//if it matches current letter (in one pair)
				if(keyTable[j][k] == plaintext[i]){
					row1 = j;
					col1 = k;
				}
				//if it matches next letter in the same pair
				if(keyTable[j][k] == plaintext[i+1]){
					row2= j;
					col2 = k;
				}
			}
		}

		//Now that we have our row and col numbers, we can apply the Playfair
		//Cipher rules discussed in lectures. Keep in mind mod 5 to account for
		//2D array limited size.

		//if column and row are different, proceed with "square" rule
		if((row1!= row2) && (col1 != col2)){
			plaintext[i] = keyTable[row1][col2];
			plaintext[i+1] = keyTable[row2][col1];
		}
		//if columns match, than we must go to next letter 
		else if(col1==col2){
			plaintext[i+1] = keyTable[(row1+1)%5][col1];
			plaintext[i] = keyTable[(row2+1)%5][col2];
		}
		//otherwise if rows match, we must go to the next letter as well
		else{
			plaintext[i] = keyTable[row1][(col1+1)%5];
			plaintext[i+1] = keyTable[row2][(col2+1)%5];

		}
	}
}

//Method 3: Decrypt
void decrypt(char ciphertext[], char keyTable[5][5]){
	int ciphertextLen = strlen(ciphertext);
	//add two to i as we increment in PAIRS
	for(int i = 0; i < ciphertextLen; i+=2){
		//two rows + cols to form "Square"
		int row1, row2, col1, col2;
		//loop through 2D Key table
		for(int j = 0; j <5; j++){
			for(int k = 0; k <5; k++){
				//if it matches current letter (in one pair)
				if(keyTable[j][k] == ciphertext[i]){
					row1 = j;
					col1 = k;
				}
				//if it matches next letter in the same pair
				if(keyTable[j][k] == ciphertext[i+1]){
					row2= j;
					col2 = k;
				}
			}
		}

		//Now that we have our row and col numbers, we can apply the Playfair
		//Cipher rules discussed in lectures in REVERSE. Keep in mind mod 5 to account for
		//2D array limited size.

		//if column and row are different, proceed with "square" rule
		if((row1!= row2) && (col1 != col2)){
			ciphertext[i] = keyTable[row1][col2];
			ciphertext[i+1] = keyTable[row2][col1];
		}
		//if columns match, than we must go to next letter 
		else if(col1==col2){
			ciphertext[i] = keyTable[(row1-1+5)%5][col1];
			ciphertext[i+1] = keyTable[(row2-1+5)%5][col2];
		}
		//otherwise if rows match, we must go to the next letter as well
		else{
			ciphertext[i] = keyTable[row1][(col1-1+5)%5];
			ciphertext[i+1] = keyTable[row2][(col2-1+5)%5];

		}
	}
}

int main(){
	//plaintext is max 600 chars (based off provided sample i/o)
	char key[30], keyTable[5][5], plaintext[600];

	printf("Enter key:\n");
	//scanf("%s", key);
	fgets(key, sizeof(key), stdin);
	key[strcspn(key, "\n")] = '\0';
	//strcpy(key, "RAYQUAZA");
	//printf("Key given: %s\n", key);

	//strcpy(plaintext, "PLAIN TEXT TO BE ENCRYPTED");
	
	printf("Enter plaintext to be encrypted\n");
	//scanf(" %[^\n]s", plaintext);
	fgets(plaintext, sizeof(plaintext), stdin);
	plaintext[strcspn(plaintext, "\n")] = '\0';
	//remove spaces from plaintext
	int plaintextLen = removeSpace(plaintext);

	printf("Plain text: %s\n", plaintext);
	genKeyTable(key, keyTable);
	encrypt(plaintext, keyTable);

	printf("Cipher text: %s\n", plaintext);

    //Decrypt back
    decrypt(plaintext, keyTable);
    printf("Decrypted: %s\n", plaintext);
    
	return 0; 

}
