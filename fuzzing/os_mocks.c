#include <openssl/evp.h>
#include <string.h>

#include "errors.h"
#include "os.h"
#include "cx.h"

void os_longjmp(unsigned int exception) {
    longjmp(try_context_get()->jmp_buf, exception);
}

try_context_t* current_context = NULL;
try_context_t* try_context_get(void) {
    return current_context;
}

try_context_t* try_context_set(try_context_t* ctx) {
    try_context_t* previous_ctx = current_context;
    current_context = ctx;
    return previous_ctx;
}

// cx_err_t cx_ripemd160_init_no_throw(cx_ripemd160_t *hash) { return CX_OK; }

// size_t cx_hash_get_size(const cx_hash_t *ctx) { return 32; }

// cx_err_t cx_sha256_init_no_throw(cx_sha256_t *hash) { return CX_OK; }

// cx_err_t cx_hash_no_throw(cx_hash_t *hash, uint32_t mode, const uint8_t *in,
//                           size_t len, uint8_t *out, size_t out_len) {
//     return CX_OK;
// }

size_t cx_hash_sha256(const uint8_t* in, size_t len, uint8_t* out, size_t out_len) {
    EVP_MD_CTX* mdctx;
    const EVP_MD* md;
    unsigned int hash_len;

    uint8_t tmpBuf[EVP_MAX_MD_SIZE];

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, in, len);
    EVP_DigestFinal_ex(mdctx, tmpBuf, &hash_len);
    EVP_MD_CTX_free(mdctx);

    if (out_len < hash_len)
        hash_len = ERR_INVALID_DATA;
    else
        memcpy(out, tmpBuf, hash_len);

    return hash_len;
}
