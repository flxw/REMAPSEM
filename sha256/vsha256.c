/*************************** HEADER FILES ***************************/
#include "vsha256.h"
#include <stdlib.h>
#include <memory.h>

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22)) /* SIGMA0 */
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25)) /* SIGMA1 */
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))    /* sigma0 */
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))  /* sigma1 */

/**************************** VARIABLES *****************************/
static const WORD k[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const BYTE data[])
{
  WORD i, j, t1,t2,m[64];

  /* unsigned int is a WORD - macro expansion didn't work here */
  vector unsigned int s0_result;
  vector unsigned int s1_result;

  // copy chunk into first 16 words w[0..15] of the message schedule array
  for (i = 0, j = 0; i < 16; ++i, j += 4) {
    m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
  }

  // Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
  for ( ; i < 64; i+=2) {
    // There is a also an interdependency right here
    // Using previous values to build the result of the current
    // Maybe rotate vectors around?
    vector unsigned int s1_input = { m[i-2 ], m[i-1 ], 0, 0 };
    vector unsigned int s0_input = { m[i-15], m[i-14], 0, 0 };

    s0_result = __builtin_crypto_vshasigmaw(s0_input,0,0);
    // or 3 to only fill in the first 2 values correctly
    s1_result = __builtin_crypto_vshasigmaw(s1_input,0,15); 

    m[i]   = s1_result[0] + m[i-7] + s0_result[0] + m[i-16];
    m[i+1] = s1_result[1] + m[i-6] + s0_result[1] + m[i-15];
  }

  /* The below needs to be ported into SIMD thinking...somehow
   * 
   * What about putting a...h into a vector before the loop
   * Inside the loop:
   * Execute EP1 and EP0 on a single piece of data and leave the rest blank
   *  ...
   *  do vec_perm to manage reassignments and do the two additions
   */
  vector unsigned int v_abcd  = ctx->state[0];
  vector unsigned int v_efgh  = ctx->state[1];
  vector unsigned int v_empty = {0,0,0,0};
  vector unsigned char shuffle_pattern = {16,16,16,16, 0,1,2,3, 4,5,6,7, 8,9,10,11};

  for (i = 0; i < 64; ++i) {
    t1 = v_efgh[3] + EP1(v_efgh[0]) + CH(v_efgh[0],v_efgh[1],v_efgh[2]) + k[i] + m[i];
    t2 = EP0(v_abcd[0]) + MAJ(v_abcd[0],v_abcd[1],v_abcd[2]);

    v_efgh = vec_perm(v_efgh, v_empty, shuffle_pattern);
    v_efgh[0] = v_abcd[3] + t1;
    v_abcd = vec_perm(v_abcd, v_empty, shuffle_pattern);
    v_abcd[0] = t1 + t2;
  }

  ctx->state[0] = vec_add(ctx->state[0], v_abcd);
  ctx->state[1] = vec_add(ctx->state[1], v_efgh);
}

void sha256_init(SHA256_CTX *ctx)
{
  ctx->datalen = 0;
  ctx->bitlen = 0;
  ctx->state[0][0] = 0x6a09e667;
  ctx->state[0][1] = 0xbb67ae85;
  ctx->state[0][2] = 0x3c6ef372;
  ctx->state[0][3] = 0xa54ff53a;
  ctx->state[1][0] = 0x510e527f;
  ctx->state[1][1] = 0x9b05688c;
  ctx->state[1][2] = 0x1f83d9ab;
  ctx->state[1][3] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len)
{
  WORD i;

  for (i = 0; i < len; ++i) {
    ctx->data[ctx->datalen] = data[i];
    ctx->datalen++;
    if (ctx->datalen == 64) {
      sha256_transform(ctx, ctx->data);
      ctx->bitlen += 512;
      ctx->datalen = 0;
    }
  }
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[])
{
  WORD i;

  i = ctx->datalen;

  // Pad whatever data is left in the buffer.
  if (ctx->datalen < 56) {
    ctx->data[i++] = 0x80;
    while (i < 56) ctx->data[i++] = 0x00;
  } else {
    ctx->data[i++] = 0x80;
    while (i < 64) ctx->data[i++] = 0x00;
    sha256_transform(ctx, ctx->data);
    memset(ctx->data, 0, 56);
  }

  // Append to the padding the total message's length in bits and transform.
  ctx->bitlen += ctx->datalen * 8;
  ctx->data[63] = ctx->bitlen;
  ctx->data[62] = ctx->bitlen >> 8;
  ctx->data[61] = ctx->bitlen >> 16;
  ctx->data[60] = ctx->bitlen >> 24;
  ctx->data[59] = ctx->bitlen >> 32;
  ctx->data[58] = ctx->bitlen >> 40;
  ctx->data[57] = ctx->bitlen >> 48;
  ctx->data[56] = ctx->bitlen >> 56;
  sha256_transform(ctx, ctx->data);

  // Since this implementation uses little endian byte ordering and SHA uses big endian,
  // reverse all the bytes when copying the final state to the output hash.
  for (i = 0; i < 4; ++i) {
    hash[i]      = (ctx->state[0][0] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 4]  = (ctx->state[0][1] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 8]  = (ctx->state[0][2] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 12] = (ctx->state[0][3] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 16] = (ctx->state[1][0] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 20] = (ctx->state[1][1] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 24] = (ctx->state[1][2] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 28] = (ctx->state[1][3] >> (24 - i * 8)) & 0x000000ff;
  }
}