#include <stdio.h>
#ifdef _ARCH_PPC
#include <altivec.h>
#endif

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))    /* sigma0 */
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))  /* sigma1 */

int main (int argc, char** argv) {
  vector unsigned int s0_input = {88, 88880, 880, 880};
  vector unsigned int s1_input = {1, 2, 3, 4};
  vector unsigned int s0_result = __builtin_crypto_vshasigmaw(s0_input,0,0);
  vector unsigned int s1_result = __builtin_crypto_vshasigmaw(s1_input,0,3);
  vector unsigned int vecempty  = {0,0,0,0};

  printf("SIMD s0: %u, %u, %u, %u\n", s0_result[0], s0_result[1], s0_result[2], s0_result[3]);
  printf("SISD s0: %u, %u, %u, %u\n", SIG0(88), SIG0(88880), SIG0(880), SIG0(880));
  printf("SIMD s1: %u, %u, %u, %u\n", s1_result[0], s1_result[1], s1_result[2], s1_result[3]);
  printf("SISD s1: %u, %u, %u, %u\n", SIG1(1), SIG1(2), SIG1(3), SIG1(4));

  vector unsigned int s1input = {1, 2, 3, 4};
  vector unsigned char pattern = {16,16,16,16,
                                   4, 5, 6, 7,
                                   8, 9, 10,11,
                                   12,13,14,15};
  vector unsigned int result  = vec_perm(s1input, vecempty, pattern);
  printf("resultvec: %i, %i, %i, %i\n", result[0], result[1], result[2], result[3]);
}
