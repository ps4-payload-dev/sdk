
asm(".global CA_MGMT_extractKeyBlobEx\n"
    ".type CA_MGMT_extractKeyBlobEx @function\n"
    "CA_MGMT_extractKeyBlobEx:\n");

asm(".global CA_MGMT_extractPublicKeyInfo\n"
    ".type CA_MGMT_extractPublicKeyInfo @function\n"
    "CA_MGMT_extractPublicKeyInfo:\n");

asm(".global CA_MGMT_freeKeyBlob\n"
    ".type CA_MGMT_freeKeyBlob @function\n"
    "CA_MGMT_freeKeyBlob:\n");

asm(".global CRYPTO_initAsymmetricKey\n"
    ".type CRYPTO_initAsymmetricKey @function\n"
    "CRYPTO_initAsymmetricKey:\n");

asm(".global CRYPTO_uninitAsymmetricKey\n"
    ".type CRYPTO_uninitAsymmetricKey @function\n"
    "CRYPTO_uninitAsymmetricKey:\n");

asm(".global RSA_verifySignature\n"
    ".type RSA_verifySignature @function\n"
    "RSA_verifySignature:\n");

asm(".global VLONG_freeVlongQueue\n"
    ".type VLONG_freeVlongQueue @function\n"
    "VLONG_freeVlongQueue:\n");

asm(".global sceSslCheckRecvPending\n"
    ".type sceSslCheckRecvPending @function\n"
    "sceSslCheckRecvPending:\n");

asm(".global sceSslClose\n"
    ".type sceSslClose @function\n"
    "sceSslClose:\n");

asm(".global sceSslConnect\n"
    ".type sceSslConnect @function\n"
    "sceSslConnect:\n");

asm(".global sceSslCreateConnection\n"
    ".type sceSslCreateConnection @function\n"
    "sceSslCreateConnection:\n");

asm(".global sceSslCreateSslConnection\n"
    ".type sceSslCreateSslConnection @function\n"
    "sceSslCreateSslConnection:\n");

asm(".global sceSslDeleteConnection\n"
    ".type sceSslDeleteConnection @function\n"
    "sceSslDeleteConnection:\n");

asm(".global sceSslDeleteSslConnection\n"
    ".type sceSslDeleteSslConnection @function\n"
    "sceSslDeleteSslConnection:\n");

asm(".global sceSslDisableOption\n"
    ".type sceSslDisableOption @function\n"
    "sceSslDisableOption:\n");

asm(".global sceSslDisableOptionInternal\n"
    ".type sceSslDisableOptionInternal @function\n"
    "sceSslDisableOptionInternal:\n");

asm(".global sceSslDisableOptionInternalInsecure\n"
    ".type sceSslDisableOptionInternalInsecure @function\n"
    "sceSslDisableOptionInternalInsecure:\n");

asm(".global sceSslDisableVerifyOption\n"
    ".type sceSslDisableVerifyOption @function\n"
    "sceSslDisableVerifyOption:\n");

asm(".global sceSslEnableOption\n"
    ".type sceSslEnableOption @function\n"
    "sceSslEnableOption:\n");

asm(".global sceSslEnableOptionInternal\n"
    ".type sceSslEnableOptionInternal @function\n"
    "sceSslEnableOptionInternal:\n");

asm(".global sceSslEnableVerifyOption\n"
    ".type sceSslEnableVerifyOption @function\n"
    "sceSslEnableVerifyOption:\n");

asm(".global sceSslFreeCaCerts\n"
    ".type sceSslFreeCaCerts @function\n"
    "sceSslFreeCaCerts:\n");

asm(".global sceSslFreeCaList\n"
    ".type sceSslFreeCaList @function\n"
    "sceSslFreeCaList:\n");

asm(".global sceSslFreeSslCertName\n"
    ".type sceSslFreeSslCertName @function\n"
    "sceSslFreeSslCertName:\n");

asm(".global sceSslGetAlpnSelected\n"
    ".type sceSslGetAlpnSelected @function\n"
    "sceSslGetAlpnSelected:\n");

asm(".global sceSslGetCaCerts\n"
    ".type sceSslGetCaCerts @function\n"
    "sceSslGetCaCerts:\n");

asm(".global sceSslGetCaList\n"
    ".type sceSslGetCaList @function\n"
    "sceSslGetCaList:\n");

asm(".global sceSslGetFingerprint\n"
    ".type sceSslGetFingerprint @function\n"
    "sceSslGetFingerprint:\n");

asm(".global sceSslGetIssuerName\n"
    ".type sceSslGetIssuerName @function\n"
    "sceSslGetIssuerName:\n");

asm(".global sceSslGetMemoryPoolStats\n"
    ".type sceSslGetMemoryPoolStats @function\n"
    "sceSslGetMemoryPoolStats:\n");

asm(".global sceSslGetNameEntryCount\n"
    ".type sceSslGetNameEntryCount @function\n"
    "sceSslGetNameEntryCount:\n");

asm(".global sceSslGetNameEntryInfo\n"
    ".type sceSslGetNameEntryInfo @function\n"
    "sceSslGetNameEntryInfo:\n");

asm(".global sceSslGetNanoSSLModuleId\n"
    ".type sceSslGetNanoSSLModuleId @function\n"
    "sceSslGetNanoSSLModuleId:\n");

asm(".global sceSslGetNotAfter\n"
    ".type sceSslGetNotAfter @function\n"
    "sceSslGetNotAfter:\n");

asm(".global sceSslGetNotBefore\n"
    ".type sceSslGetNotBefore @function\n"
    "sceSslGetNotBefore:\n");

asm(".global sceSslGetPeerCert\n"
    ".type sceSslGetPeerCert @function\n"
    "sceSslGetPeerCert:\n");

asm(".global sceSslGetPem\n"
    ".type sceSslGetPem @function\n"
    "sceSslGetPem:\n");

asm(".global sceSslGetSerialNumber\n"
    ".type sceSslGetSerialNumber @function\n"
    "sceSslGetSerialNumber:\n");

asm(".global sceSslGetSslError\n"
    ".type sceSslGetSslError @function\n"
    "sceSslGetSslError:\n");

asm(".global sceSslGetSubjectName\n"
    ".type sceSslGetSubjectName @function\n"
    "sceSslGetSubjectName:\n");

asm(".global sceSslInit\n"
    ".type sceSslInit @function\n"
    "sceSslInit:\n");

asm(".global sceSslLoadCert\n"
    ".type sceSslLoadCert @function\n"
    "sceSslLoadCert:\n");

asm(".global sceSslLoadRootCACert\n"
    ".type sceSslLoadRootCACert @function\n"
    "sceSslLoadRootCACert:\n");

asm(".global sceSslRead\n"
    ".type sceSslRead @function\n"
    "sceSslRead:\n");

asm(".global sceSslRecv\n"
    ".type sceSslRecv @function\n"
    "sceSslRecv:\n");

asm(".global sceSslReuseConnection\n"
    ".type sceSslReuseConnection @function\n"
    "sceSslReuseConnection:\n");

asm(".global sceSslSend\n"
    ".type sceSslSend @function\n"
    "sceSslSend:\n");

asm(".global sceSslSetAlpn\n"
    ".type sceSslSetAlpn @function\n"
    "sceSslSetAlpn:\n");

asm(".global sceSslSetMinSslVersion\n"
    ".type sceSslSetMinSslVersion @function\n"
    "sceSslSetMinSslVersion:\n");

asm(".global sceSslSetSslVersion\n"
    ".type sceSslSetSslVersion @function\n"
    "sceSslSetSslVersion:\n");

asm(".global sceSslSetVerifyCallback\n"
    ".type sceSslSetVerifyCallback @function\n"
    "sceSslSetVerifyCallback:\n");

asm(".global sceSslTerm\n"
    ".type sceSslTerm @function\n"
    "sceSslTerm:\n");

asm(".global sceSslUnloadCert\n"
    ".type sceSslUnloadCert @function\n"
    "sceSslUnloadCert:\n");

asm(".global sceSslWrite\n"
    ".type sceSslWrite @function\n"
    "sceSslWrite:\n");
