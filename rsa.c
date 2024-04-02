#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	printf("Enter two prime numbers (p,q)\n");
	int p,q;
	scanf("%d", &p);
	scanf("%d", &q);
	printf("p and q: %d %d\n",p ,q);
	
	//product will be second key value for both public and private RSA values
	int product = p*q;
	//printf("product: %d\n", product);
	
	printf("Calculating RSA values...\n");
	//printf("e is %d\n", calcPublicKey(p,q));
	int e = calcPublicKey(p,q);
	printf("Public RSA key is (%d, %d)\n", e, product);
	int d = calcPrivateKey(p,q,e);
	printf("Private RSA key is (%d, %d)\n", d, product);
	//message
	int m;
	printf("Enter plaintext message (an integer):\n");
	scanf("%d", &m);
	printf("Encrypting m...\n");
	//ciphertext
	int c = modExp(m, e, product);
	printf("Ciphertext is: %d\n", c);
	//decrypted message
	int m1 = modExp(c, d, product);
	printf("Decrypting c...\n");
	printf("The plaintext is: %d\n", m1);
}
//SOURCE
int modExp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        //If exp is odd, multiply base with result
        if (exp % 2 == 1)
            result = (result * base) % mod;
        //exp must be even now
        exp = exp >> 1; //exp = exp/2
        base = (base * base) % mod;
    }
    return result;
}
//greatest common denominator to be used to find coprimes
int gcd(int a, int b){
	if(b==0){
		return a;
	}
	return gcd(b, a%b);
}

/* Calculate Public Key 
 * looking for phi/number of coprimes
 * coprime: no shared primes with product (p*q)
 */
int calcPublicKey(int p, int q){
	//get the phi function value
	int phi = (p-1)*(q-1);
	//printf("Phi: %d\n", phi);	
	
	//choose number e such that it is 1<e<phi
	//that is e and phi must not have common factors
	int e = 2; 
	while (e < phi){
		if(gcd(e,phi)==1){
			break;
		}
		e++;
	}
	return e;
}
/* Calculate Private Key
 * look for number d such that e*d(mod phi) = 1;
 * d & e must NOT be the same number
 */
int calcPrivateKey(int p, int q, int e){
	//get phi function value
	int phi = (p-1)*(q-1);
	int d = modInverse(e,phi);
	return d;
}

//Via: [1]
int gcdExt(int a, int b, int *x, int *y){
	if(a==0){
		*x = 0;
		*y = 1;
		return b;
	}

	int x1,y1;
	int gcd = gcdExt(b%a, a, &x1, &y1);

	*x = y1-(b/a)*x1;
	*y = x1;

	return gcd;
}

//Via: [1]
int modInverse(int e, int phi){
	int x,y;
	int gcd = gcdExt(e, phi, &x, &y);

	if(gcd!=1){
		return -1;
	}

	return (x%phi+phi)%phi;
}
