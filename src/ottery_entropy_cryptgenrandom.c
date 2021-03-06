/* Libottery by Nick Mathewson.

   This software has been dedicated to the public domain under the CC0
   public domain dedication.

   To the extent possible under law, the person who associated CC0 with
   libottery has waived all copyright and related or neighboring rights
   to libottery.

   You should have received a copy of the CC0 legalcode along with this
   work in doc/cc0.txt.  If not, see
      <http://creativecommons.org/publicdomain/zero/1.0/>.
 */
#define OTTERY_INTERNAL
#include "ottery-internal.h"
#include "ottery.h"

#ifdef _WIN32

/** Generate random bytes using the Windows CryptGenRandom operating-system
 * RNG. */
static int
ottery_os_randbytes_cryptgenrandom(const struct ottery_osrng_config *cfg,
                          uint8_t *out, size_t outlen)
{
  /* On Windows, CryptGenRandom is supposed to be a well-seeded
   * cryptographically strong random number generator. */
  HCRYPTPROV provider;
  int retval = 0;
  (void) cfg;

  if (0 == CryptAcquireContext(&provider, NULL, NULL, PROV_RSA_FULL,
                               CRYPT_VERIFYCONTEXT))
    return OTTERY_ERR_INIT_STRONG_RNG;

  if (0 == CryptGenRandom(provider, outlen, out))
    retval = OTTERY_ERR_ACCESS_STRONG_RNG;

  CryptReleaseContext(provider, 0);
  return retval;
}

#define ENTROPY_SOURCE_CRYPTGENRANDOM           \
  { ottery_os_randbytes_cryptgenrandom,         \
      SRC(CRYPTGENRANDOM)|FL(OS)|FL(STRONG) }

#endif


