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
#include <stdarg.h>
#include "mycrypt.h"

/* will read multiple DER INTEGER encoded mp_ints from src
 * of upto [inlen] bytes.  It will store the number of bytes
 * read back into [inlen].
 */
int der_get_multi_integer(const unsigned char *src, unsigned long *inlen, 
                  mp_int *num, ...)
{
   va_list        args;
   mp_int        *next;
   unsigned long  wrote, len;
   int            err;

   _ARGCHK(src    != NULL);
   _ARGCHK(inlen  != NULL);

   /* setup va list */
   next  = num;
   len   = *inlen;
   wrote = 0;
   va_start(args, num);

   while (next != NULL) {
       if ((err = der_decode_integer(src, inlen, next)) != CRYPT_OK) {
          va_end(args);
          return err;
       }
       wrote += *inlen;
       src   += *inlen;
       len   -= *inlen;
       *inlen = len;
        next     = va_arg(args, mp_int*);
   }
   va_end(args);
   *inlen = wrote;
   return CRYPT_OK;
}

