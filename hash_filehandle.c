/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * Tom St Denis, tomstdenis@iahu.ca, http://libtomcrypt.org
 */
#include "mycrypt.h"

int hash_filehandle(int hash, FILE *in, unsigned char *dst, unsigned long *outlen)
{
#ifdef NO_FILE
    return CRYPT_NOP;
#else
    hash_state md;
    unsigned char buf[512];
    size_t x;
    int err;

    _ARGCHK(dst    != NULL);
    _ARGCHK(outlen != NULL);
    _ARGCHK(in     != NULL);

    if ((err = hash_is_valid(hash)) != CRYPT_OK) {
        return err;
    }

    if (*outlen < hash_descriptor[hash].hashsize) {
       return CRYPT_BUFFER_OVERFLOW;
    }
    if ((err = hash_descriptor[hash].init(&md)) != CRYPT_OK) {
       return err;
    }

    *outlen = hash_descriptor[hash].hashsize;
    do {
        x = fread(buf, 1, sizeof(buf), in);
        if ((err = hash_descriptor[hash].process(&md, buf, x)) != CRYPT_OK) {
           return err;
        }
    } while (x == sizeof(buf));
    err = hash_descriptor[hash].done(&md, dst);

#ifdef CLEAN_STACK
    zeromem(buf, sizeof(buf));
#endif
    return err;
#endif
}

