
asm(".global CA_MGMT_allocCertDistinguishedName\n"
    ".type CA_MGMT_allocCertDistinguishedName @function\n"
    "CA_MGMT_allocCertDistinguishedName:\n");

asm(".global CA_MGMT_certDistinguishedNameCompare\n"
    ".type CA_MGMT_certDistinguishedNameCompare @function\n"
    "CA_MGMT_certDistinguishedNameCompare:\n");

asm(".global CA_MGMT_convertKeyBlobToPKCS8Key\n"
    ".type CA_MGMT_convertKeyBlobToPKCS8Key @function\n"
    "CA_MGMT_convertKeyBlobToPKCS8Key:\n");

asm(".global CA_MGMT_convertKeyDER\n"
    ".type CA_MGMT_convertKeyDER @function\n"
    "CA_MGMT_convertKeyDER:\n");

asm(".global CA_MGMT_convertKeyPEM\n"
    ".type CA_MGMT_convertKeyPEM @function\n"
    "CA_MGMT_convertKeyPEM:\n");

asm(".global CA_MGMT_convertPKCS8KeyToKeyBlob\n"
    ".type CA_MGMT_convertPKCS8KeyToKeyBlob @function\n"
    "CA_MGMT_convertPKCS8KeyToKeyBlob:\n");

asm(".global CA_MGMT_convertProtectedPKCS8KeyToKeyBlob\n"
    ".type CA_MGMT_convertProtectedPKCS8KeyToKeyBlob @function\n"
    "CA_MGMT_convertProtectedPKCS8KeyToKeyBlob:\n");

asm(".global CA_MGMT_decodeCertificate\n"
    ".type CA_MGMT_decodeCertificate @function\n"
    "CA_MGMT_decodeCertificate:\n");

asm(".global CA_MGMT_enumAltName\n"
    ".type CA_MGMT_enumAltName @function\n"
    "CA_MGMT_enumAltName:\n");

asm(".global CA_MGMT_enumCrl\n"
    ".type CA_MGMT_enumCrl @function\n"
    "CA_MGMT_enumCrl:\n");

asm(".global CA_MGMT_extractAllCertDistinguishedName\n"
    ".type CA_MGMT_extractAllCertDistinguishedName @function\n"
    "CA_MGMT_extractAllCertDistinguishedName:\n");

asm(".global CA_MGMT_extractBasicConstraint\n"
    ".type CA_MGMT_extractBasicConstraint @function\n"
    "CA_MGMT_extractBasicConstraint:\n");

asm(".global CA_MGMT_extractCertASN1Name\n"
    ".type CA_MGMT_extractCertASN1Name @function\n"
    "CA_MGMT_extractCertASN1Name:\n");

asm(".global CA_MGMT_extractCertTimes\n"
    ".type CA_MGMT_extractCertTimes @function\n"
    "CA_MGMT_extractCertTimes:\n");

asm(".global CA_MGMT_extractKeyBlobEx\n"
    ".type CA_MGMT_extractKeyBlobEx @function\n"
    "CA_MGMT_extractKeyBlobEx:\n");

asm(".global CA_MGMT_extractKeyBlobTypeEx\n"
    ".type CA_MGMT_extractKeyBlobTypeEx @function\n"
    "CA_MGMT_extractKeyBlobTypeEx:\n");

asm(".global CA_MGMT_extractPublicKeyInfo\n"
    ".type CA_MGMT_extractPublicKeyInfo @function\n"
    "CA_MGMT_extractPublicKeyInfo:\n");

asm(".global CA_MGMT_extractSerialNum\n"
    ".type CA_MGMT_extractSerialNum @function\n"
    "CA_MGMT_extractSerialNum:\n");

asm(".global CA_MGMT_extractSignature\n"
    ".type CA_MGMT_extractSignature @function\n"
    "CA_MGMT_extractSignature:\n");

asm(".global CA_MGMT_free\n"
    ".type CA_MGMT_free @function\n"
    "CA_MGMT_free:\n");

asm(".global CA_MGMT_freeCertDistinguishedName\n"
    ".type CA_MGMT_freeCertDistinguishedName @function\n"
    "CA_MGMT_freeCertDistinguishedName:\n");

asm(".global CA_MGMT_freeCertDistinguishedNameOnStack\n"
    ".type CA_MGMT_freeCertDistinguishedNameOnStack @function\n"
    "CA_MGMT_freeCertDistinguishedNameOnStack:\n");

asm(".global CA_MGMT_freeCertificate\n"
    ".type CA_MGMT_freeCertificate @function\n"
    "CA_MGMT_freeCertificate:\n");

asm(".global CA_MGMT_freeKeyBlob\n"
    ".type CA_MGMT_freeKeyBlob @function\n"
    "CA_MGMT_freeKeyBlob:\n");

asm(".global CA_MGMT_freeSearchDetails\n"
    ".type CA_MGMT_freeSearchDetails @function\n"
    "CA_MGMT_freeSearchDetails:\n");

asm(".global CA_MGMT_getCertSignAlgoType\n"
    ".type CA_MGMT_getCertSignAlgoType @function\n"
    "CA_MGMT_getCertSignAlgoType:\n");

asm(".global CA_MGMT_keyBlobToDER\n"
    ".type CA_MGMT_keyBlobToDER @function\n"
    "CA_MGMT_keyBlobToDER:\n");

asm(".global CA_MGMT_keyBlobToPEM\n"
    ".type CA_MGMT_keyBlobToPEM @function\n"
    "CA_MGMT_keyBlobToPEM:\n");

asm(".global CA_MGMT_makeKeyBlobEx\n"
    ".type CA_MGMT_makeKeyBlobEx @function\n"
    "CA_MGMT_makeKeyBlobEx:\n");

asm(".global CA_MGMT_rawVerifyOID\n"
    ".type CA_MGMT_rawVerifyOID @function\n"
    "CA_MGMT_rawVerifyOID:\n");

asm(".global CA_MGMT_reorderChain\n"
    ".type CA_MGMT_reorderChain @function\n"
    "CA_MGMT_reorderChain:\n");

asm(".global CA_MGMT_returnCertificatePrints\n"
    ".type CA_MGMT_returnCertificatePrints @function\n"
    "CA_MGMT_returnCertificatePrints:\n");

asm(".global CA_MGMT_verifyCertWithKeyBlob\n"
    ".type CA_MGMT_verifyCertWithKeyBlob @function\n"
    "CA_MGMT_verifyCertWithKeyBlob:\n");

asm(".global CA_MGMT_verifySignature\n"
    ".type CA_MGMT_verifySignature @function\n"
    "CA_MGMT_verifySignature:\n");

asm(".global CERT_CompSubjectAltNames\n"
    ".type CERT_CompSubjectAltNames @function\n"
    "CERT_CompSubjectAltNames:\n");

asm(".global CERT_CompSubjectAltNames2\n"
    ".type CERT_CompSubjectAltNames2 @function\n"
    "CERT_CompSubjectAltNames2:\n");

asm(".global CERT_CompSubjectAltNamesExact\n"
    ".type CERT_CompSubjectAltNamesExact @function\n"
    "CERT_CompSubjectAltNamesExact:\n");

asm(".global CERT_CompSubjectCommonName\n"
    ".type CERT_CompSubjectCommonName @function\n"
    "CERT_CompSubjectCommonName:\n");

asm(".global CERT_CompSubjectCommonName2\n"
    ".type CERT_CompSubjectCommonName2 @function\n"
    "CERT_CompSubjectCommonName2:\n");

asm(".global CERT_ComputeBufferHash\n"
    ".type CERT_ComputeBufferHash @function\n"
    "CERT_ComputeBufferHash:\n");

asm(".global CERT_GetCertTime\n"
    ".type CERT_GetCertTime @function\n"
    "CERT_GetCertTime:\n");

asm(".global CERT_STORE_addCertAuthority\n"
    ".type CERT_STORE_addCertAuthority @function\n"
    "CERT_STORE_addCertAuthority:\n");

asm(".global CERT_STORE_addIdentity\n"
    ".type CERT_STORE_addIdentity @function\n"
    "CERT_STORE_addIdentity:\n");

asm(".global CERT_STORE_addIdentityNakedKey\n"
    ".type CERT_STORE_addIdentityNakedKey @function\n"
    "CERT_STORE_addIdentityNakedKey:\n");

asm(".global CERT_STORE_addIdentityPSK\n"
    ".type CERT_STORE_addIdentityPSK @function\n"
    "CERT_STORE_addIdentityPSK:\n");

asm(".global CERT_STORE_addIdentityWithCertificateChain\n"
    ".type CERT_STORE_addIdentityWithCertificateChain @function\n"
    "CERT_STORE_addIdentityWithCertificateChain:\n");

asm(".global CERT_STORE_addTrustPoint\n"
    ".type CERT_STORE_addTrustPoint @function\n"
    "CERT_STORE_addTrustPoint:\n");

asm(".global CERT_STORE_createStore\n"
    ".type CERT_STORE_createStore @function\n"
    "CERT_STORE_createStore:\n");

asm(".global CERT_STORE_findCertBySubject\n"
    ".type CERT_STORE_findCertBySubject @function\n"
    "CERT_STORE_findCertBySubject:\n");

asm(".global CERT_STORE_findIdentityByTypeFirst\n"
    ".type CERT_STORE_findIdentityByTypeFirst @function\n"
    "CERT_STORE_findIdentityByTypeFirst:\n");

asm(".global CERT_STORE_findIdentityByTypeNext\n"
    ".type CERT_STORE_findIdentityByTypeNext @function\n"
    "CERT_STORE_findIdentityByTypeNext:\n");

asm(".global CERT_STORE_findIdentityCertChainFirst\n"
    ".type CERT_STORE_findIdentityCertChainFirst @function\n"
    "CERT_STORE_findIdentityCertChainFirst:\n");

asm(".global CERT_STORE_findIdentityCertChainNext\n"
    ".type CERT_STORE_findIdentityCertChainNext @function\n"
    "CERT_STORE_findIdentityCertChainNext:\n");

asm(".global CERT_STORE_findPskByIdentity\n"
    ".type CERT_STORE_findPskByIdentity @function\n"
    "CERT_STORE_findPskByIdentity:\n");

asm(".global CERT_STORE_releaseStore\n"
    ".type CERT_STORE_releaseStore @function\n"
    "CERT_STORE_releaseStore:\n");

asm(".global CERT_STORE_traversePskListHead\n"
    ".type CERT_STORE_traversePskListHead @function\n"
    "CERT_STORE_traversePskListHead:\n");

asm(".global CERT_STORE_traversePskListNext\n"
    ".type CERT_STORE_traversePskListNext @function\n"
    "CERT_STORE_traversePskListNext:\n");

asm(".global CERT_VerifyCertificatePolicies\n"
    ".type CERT_VerifyCertificatePolicies @function\n"
    "CERT_VerifyCertificatePolicies:\n");

asm(".global CERT_VerifyCertificatePolicies2\n"
    ".type CERT_VerifyCertificatePolicies2 @function\n"
    "CERT_VerifyCertificatePolicies2:\n");

asm(".global CERT_VerifyValidityTime\n"
    ".type CERT_VerifyValidityTime @function\n"
    "CERT_VerifyValidityTime:\n");

asm(".global CERT_VerifyValidityTime2\n"
    ".type CERT_VerifyValidityTime2 @function\n"
    "CERT_VerifyValidityTime2:\n");

asm(".global CERT_VerifyValidityTimeWithConf\n"
    ".type CERT_VerifyValidityTimeWithConf @function\n"
    "CERT_VerifyValidityTimeWithConf:\n");

asm(".global CERT_checkCertificateIssuer\n"
    ".type CERT_checkCertificateIssuer @function\n"
    "CERT_checkCertificateIssuer:\n");

asm(".global CERT_checkCertificateIssuer2\n"
    ".type CERT_checkCertificateIssuer2 @function\n"
    "CERT_checkCertificateIssuer2:\n");

asm(".global CERT_checkCertificateIssuerSerialNumber\n"
    ".type CERT_checkCertificateIssuerSerialNumber @function\n"
    "CERT_checkCertificateIssuerSerialNumber:\n");

asm(".global CERT_decryptRSASignature\n"
    ".type CERT_decryptRSASignature @function\n"
    "CERT_decryptRSASignature:\n");

asm(".global CERT_decryptRSASignatureBuffer\n"
    ".type CERT_decryptRSASignatureBuffer @function\n"
    "CERT_decryptRSASignatureBuffer:\n");

asm(".global CERT_enumerateAltName\n"
    ".type CERT_enumerateAltName @function\n"
    "CERT_enumerateAltName:\n");

asm(".global CERT_enumerateAltName2\n"
    ".type CERT_enumerateAltName2 @function\n"
    "CERT_enumerateAltName2:\n");

asm(".global CERT_enumerateCRL\n"
    ".type CERT_enumerateCRL @function\n"
    "CERT_enumerateCRL:\n");

asm(".global CERT_enumerateCRL2\n"
    ".type CERT_enumerateCRL2 @function\n"
    "CERT_enumerateCRL2:\n");

asm(".global CERT_enumerateCRLAux\n"
    ".type CERT_enumerateCRLAux @function\n"
    "CERT_enumerateCRLAux:\n");

asm(".global CERT_extractAllDistinguishedNames\n"
    ".type CERT_extractAllDistinguishedNames @function\n"
    "CERT_extractAllDistinguishedNames:\n");

asm(".global CERT_extractDistinguishedNames\n"
    ".type CERT_extractDistinguishedNames @function\n"
    "CERT_extractDistinguishedNames:\n");

asm(".global CERT_extractDistinguishedNames2\n"
    ".type CERT_extractDistinguishedNames2 @function\n"
    "CERT_extractDistinguishedNames2:\n");

asm(".global CERT_extractDistinguishedNamesFromName\n"
    ".type CERT_extractDistinguishedNamesFromName @function\n"
    "CERT_extractDistinguishedNamesFromName:\n");

asm(".global CERT_extractRSAKey\n"
    ".type CERT_extractRSAKey @function\n"
    "CERT_extractRSAKey:\n");

asm(".global CERT_extractSerialNum\n"
    ".type CERT_extractSerialNum @function\n"
    "CERT_extractSerialNum:\n");

asm(".global CERT_extractSerialNum2\n"
    ".type CERT_extractSerialNum2 @function\n"
    "CERT_extractSerialNum2:\n");

asm(".global CERT_extractValidityTime\n"
    ".type CERT_extractValidityTime @function\n"
    "CERT_extractValidityTime:\n");

asm(".global CERT_extractValidityTime2\n"
    ".type CERT_extractValidityTime2 @function\n"
    "CERT_extractValidityTime2:\n");

asm(".global CERT_getCertExtension\n"
    ".type CERT_getCertExtension @function\n"
    "CERT_getCertExtension:\n");

asm(".global CERT_getCertSignAlgoType\n"
    ".type CERT_getCertSignAlgoType @function\n"
    "CERT_getCertSignAlgoType:\n");

asm(".global CERT_getCertificateExtensions\n"
    ".type CERT_getCertificateExtensions @function\n"
    "CERT_getCertificateExtensions:\n");

asm(".global CERT_getCertificateExtensions2\n"
    ".type CERT_getCertificateExtensions2 @function\n"
    "CERT_getCertificateExtensions2:\n");

asm(".global CERT_getCertificateIssuerSerialNumber\n"
    ".type CERT_getCertificateIssuerSerialNumber @function\n"
    "CERT_getCertificateIssuerSerialNumber:\n");

asm(".global CERT_getCertificateIssuerSerialNumber2\n"
    ".type CERT_getCertificateIssuerSerialNumber2 @function\n"
    "CERT_getCertificateIssuerSerialNumber2:\n");

asm(".global CERT_getCertificateKeyUsage\n"
    ".type CERT_getCertificateKeyUsage @function\n"
    "CERT_getCertificateKeyUsage:\n");

asm(".global CERT_getCertificateKeyUsage2\n"
    ".type CERT_getCertificateKeyUsage2 @function\n"
    "CERT_getCertificateKeyUsage2:\n");

asm(".global CERT_getCertificateSubject\n"
    ".type CERT_getCertificateSubject @function\n"
    "CERT_getCertificateSubject:\n");

asm(".global CERT_getCertificateSubject2\n"
    ".type CERT_getCertificateSubject2 @function\n"
    "CERT_getCertificateSubject2:\n");

asm(".global CERT_getNumberOfChild\n"
    ".type CERT_getNumberOfChild @function\n"
    "CERT_getNumberOfChild:\n");

asm(".global CERT_getRSASignatureAlgo\n"
    ".type CERT_getRSASignatureAlgo @function\n"
    "CERT_getRSASignatureAlgo:\n");

asm(".global CERT_getSignatureItem\n"
    ".type CERT_getSignatureItem @function\n"
    "CERT_getSignatureItem:\n");

asm(".global CERT_getSubjectCommonName\n"
    ".type CERT_getSubjectCommonName @function\n"
    "CERT_getSubjectCommonName:\n");

asm(".global CERT_getSubjectCommonName2\n"
    ".type CERT_getSubjectCommonName2 @function\n"
    "CERT_getSubjectCommonName2:\n");

asm(".global CERT_isRootCertificate\n"
    ".type CERT_isRootCertificate @function\n"
    "CERT_isRootCertificate:\n");

asm(".global CERT_isRootCertificate2\n"
    ".type CERT_isRootCertificate2 @function\n"
    "CERT_isRootCertificate2:\n");

asm(".global CERT_rawVerifyOID\n"
    ".type CERT_rawVerifyOID @function\n"
    "CERT_rawVerifyOID:\n");

asm(".global CERT_rawVerifyOID2\n"
    ".type CERT_rawVerifyOID2 @function\n"
    "CERT_rawVerifyOID2:\n");

asm(".global CERT_setKeyFromSubjectPublicKeyInfo\n"
    ".type CERT_setKeyFromSubjectPublicKeyInfo @function\n"
    "CERT_setKeyFromSubjectPublicKeyInfo:\n");

asm(".global CERT_setKeyFromSubjectPublicKeyInfoCert\n"
    ".type CERT_setKeyFromSubjectPublicKeyInfoCert @function\n"
    "CERT_setKeyFromSubjectPublicKeyInfoCert:\n");

asm(".global CERT_validateCertificate\n"
    ".type CERT_validateCertificate @function\n"
    "CERT_validateCertificate:\n");

asm(".global CERT_validateCertificateWithConf\n"
    ".type CERT_validateCertificateWithConf @function\n"
    "CERT_validateCertificateWithConf:\n");

asm(".global CERT_verifySignature\n"
    ".type CERT_verifySignature @function\n"
    "CERT_verifySignature:\n");

asm(".global CRYPTO_initAsymmetricKey\n"
    ".type CRYPTO_initAsymmetricKey @function\n"
    "CRYPTO_initAsymmetricKey:\n");

asm(".global CRYPTO_uninitAsymmetricKey\n"
    ".type CRYPTO_uninitAsymmetricKey @function\n"
    "CRYPTO_uninitAsymmetricKey:\n");

asm(".global GC_createInstanceIDs\n"
    ".type GC_createInstanceIDs @function\n"
    "GC_createInstanceIDs:\n");

asm(".global MOCANA_freeMocana\n"
    ".type MOCANA_freeMocana @function\n"
    "MOCANA_freeMocana:\n");

asm(".global MOCANA_initMocana\n"
    ".type MOCANA_initMocana @function\n"
    "MOCANA_initMocana:\n");

asm(".global RSA_verifySignature\n"
    ".type RSA_verifySignature @function\n"
    "RSA_verifySignature:\n");

asm(".global SSL_ASYNC_acceptConnection\n"
    ".type SSL_ASYNC_acceptConnection @function\n"
    "SSL_ASYNC_acceptConnection:\n");

asm(".global SSL_ASYNC_closeConnection\n"
    ".type SSL_ASYNC_closeConnection @function\n"
    "SSL_ASYNC_closeConnection:\n");

asm(".global SSL_ASYNC_connect\n"
    ".type SSL_ASYNC_connect @function\n"
    "SSL_ASYNC_connect:\n");

asm(".global SSL_ASYNC_connectCommon\n"
    ".type SSL_ASYNC_connectCommon @function\n"
    "SSL_ASYNC_connectCommon:\n");

asm(".global SSL_ASYNC_getRecvBuffer\n"
    ".type SSL_ASYNC_getRecvBuffer @function\n"
    "SSL_ASYNC_getRecvBuffer:\n");

asm(".global SSL_ASYNC_getSendBuffer\n"
    ".type SSL_ASYNC_getSendBuffer @function\n"
    "SSL_ASYNC_getSendBuffer:\n");

asm(".global SSL_ASYNC_init\n"
    ".type SSL_ASYNC_init @function\n"
    "SSL_ASYNC_init:\n");

asm(".global SSL_ASYNC_initServer\n"
    ".type SSL_ASYNC_initServer @function\n"
    "SSL_ASYNC_initServer:\n");

asm(".global SSL_ASYNC_recvMessage\n"
    ".type SSL_ASYNC_recvMessage @function\n"
    "SSL_ASYNC_recvMessage:\n");

asm(".global SSL_ASYNC_recvMessage2\n"
    ".type SSL_ASYNC_recvMessage2 @function\n"
    "SSL_ASYNC_recvMessage2:\n");

asm(".global SSL_ASYNC_sendMessage\n"
    ".type SSL_ASYNC_sendMessage @function\n"
    "SSL_ASYNC_sendMessage:\n");

asm(".global SSL_ASYNC_sendMessagePending\n"
    ".type SSL_ASYNC_sendMessagePending @function\n"
    "SSL_ASYNC_sendMessagePending:\n");

asm(".global SSL_ASYNC_start\n"
    ".type SSL_ASYNC_start @function\n"
    "SSL_ASYNC_start:\n");

asm(".global SSL_acceptConnection\n"
    ".type SSL_acceptConnection @function\n"
    "SSL_acceptConnection:\n");

asm(".global SSL_acceptConnectionCommon\n"
    ".type SSL_acceptConnectionCommon @function\n"
    "SSL_acceptConnectionCommon:\n");

asm(".global SSL_assignCertificateStore\n"
    ".type SSL_assignCertificateStore @function\n"
    "SSL_assignCertificateStore:\n");

asm(".global SSL_closeConnection\n"
    ".type SSL_closeConnection @function\n"
    "SSL_closeConnection:\n");

asm(".global SSL_connect\n"
    ".type SSL_connect @function\n"
    "SSL_connect:\n");

asm(".global SSL_connectWithCfgParam\n"
    ".type SSL_connectWithCfgParam @function\n"
    "SSL_connectWithCfgParam:\n");

asm(".global SSL_enableCiphers\n"
    ".type SSL_enableCiphers @function\n"
    "SSL_enableCiphers:\n");

asm(".global SSL_findConnectionInstance\n"
    ".type SSL_findConnectionInstance @function\n"
    "SSL_findConnectionInstance:\n");

asm(".global SSL_getCipherInfo\n"
    ".type SSL_getCipherInfo @function\n"
    "SSL_getCipherInfo:\n");

asm(".global SSL_getClientRandom\n"
    ".type SSL_getClientRandom @function\n"
    "SSL_getClientRandom:\n");

asm(".global SSL_getClientSessionInfo\n"
    ".type SSL_getClientSessionInfo @function\n"
    "SSL_getClientSessionInfo:\n");

asm(".global SSL_getCookie\n"
    ".type SSL_getCookie @function\n"
    "SSL_getCookie:\n");

asm(".global SSL_getNextSessionId\n"
    ".type SSL_getNextSessionId @function\n"
    "SSL_getNextSessionId:\n");

asm(".global SSL_getSSLTLSVersion\n"
    ".type SSL_getSSLTLSVersion @function\n"
    "SSL_getSSLTLSVersion:\n");

asm(".global SSL_getServerRandom\n"
    ".type SSL_getServerRandom @function\n"
    "SSL_getServerRandom:\n");

asm(".global SSL_getSessionCache\n"
    ".type SSL_getSessionCache @function\n"
    "SSL_getSessionCache:\n");

asm(".global SSL_getSessionFlags\n"
    ".type SSL_getSessionFlags @function\n"
    "SSL_getSessionFlags:\n");

asm(".global SSL_getSessionInfo\n"
    ".type SSL_getSessionInfo @function\n"
    "SSL_getSessionInfo:\n");

asm(".global SSL_getSessionStatus\n"
    ".type SSL_getSessionStatus @function\n"
    "SSL_getSessionStatus:\n");

asm(".global SSL_getSocketId\n"
    ".type SSL_getSocketId @function\n"
    "SSL_getSocketId:\n");

asm(".global SSL_init\n"
    ".type SSL_init @function\n"
    "SSL_init:\n");

asm(".global SSL_initServerCert\n"
    ".type SSL_initServerCert @function\n"
    "SSL_initServerCert:\n");

asm(".global SSL_initiateRehandshake\n"
    ".type SSL_initiateRehandshake @function\n"
    "SSL_initiateRehandshake:\n");

asm(".global SSL_ioctl\n"
    ".type SSL_ioctl @function\n"
    "SSL_ioctl:\n");

asm(".global SSL_isSessionSSL\n"
    ".type SSL_isSessionSSL @function\n"
    "SSL_isSessionSSL:\n");

asm(".global SSL_lockSessionCacheMutex\n"
    ".type SSL_lockSessionCacheMutex @function\n"
    "SSL_lockSessionCacheMutex:\n");

asm(".global SSL_lookupAlert\n"
    ".type SSL_lookupAlert @function\n"
    "SSL_lookupAlert:\n");

asm(".global SSL_negotiateConnection\n"
    ".type SSL_negotiateConnection @function\n"
    "SSL_negotiateConnection:\n");

asm(".global SSL_recv\n"
    ".type SSL_recv @function\n"
    "SSL_recv:\n");

asm(".global SSL_recvPending\n"
    ".type SSL_recvPending @function\n"
    "SSL_recvPending:\n");

asm(".global SSL_releaseTables\n"
    ".type SSL_releaseTables @function\n"
    "SSL_releaseTables:\n");

asm(".global SSL_retrieveServerNameList\n"
    ".type SSL_retrieveServerNameList @function\n"
    "SSL_retrieveServerNameList:\n");

asm(".global SSL_rngFun\n"
    ".type SSL_rngFun @function\n"
    "SSL_rngFun:\n");

asm(".global SSL_send\n"
    ".type SSL_send @function\n"
    "SSL_send:\n");

asm(".global SSL_sendAlert\n"
    ".type SSL_sendAlert @function\n"
    "SSL_sendAlert:\n");

asm(".global SSL_sendPending\n"
    ".type SSL_sendPending @function\n"
    "SSL_sendPending:\n");

asm(".global SSL_setCookie\n"
    ".type SSL_setCookie @function\n"
    "SSL_setCookie:\n");

asm(".global SSL_setServerCert\n"
    ".type SSL_setServerCert @function\n"
    "SSL_setServerCert:\n");

asm(".global SSL_setServerNameList\n"
    ".type SSL_setServerNameList @function\n"
    "SSL_setServerNameList:\n");

asm(".global SSL_setSessionFlags\n"
    ".type SSL_setSessionFlags @function\n"
    "SSL_setSessionFlags:\n");

asm(".global SSL_shutdown\n"
    ".type SSL_shutdown @function\n"
    "SSL_shutdown:\n");

asm(".global SSL_sslSettings\n"
    ".type SSL_sslSettings @function\n"
    "SSL_sslSettings:\n");

asm(".global SSL_validateCertParam\n"
    ".type SSL_validateCertParam @function\n"
    "SSL_validateCertParam:\n");

asm(".global VLONG_freeVlongQueue\n"
    ".type VLONG_freeVlongQueue @function\n"
    "VLONG_freeVlongQueue:\n");

asm(".global getCertSigAlgo\n"
    ".type getCertSigAlgo @function\n"
    "getCertSigAlgo:\n");

asm(".global sceSslCheckRecvPending\n"
    ".type sceSslCheckRecvPending @function\n"
    "sceSslCheckRecvPending:\n");

asm(".global sceSslClose\n"
    ".type sceSslClose @function\n"
    "sceSslClose:\n");

asm(".global sceSslConnect\n"
    ".type sceSslConnect @function\n"
    "sceSslConnect:\n");

asm(".global sceSslCreateSslConnection\n"
    ".type sceSslCreateSslConnection @function\n"
    "sceSslCreateSslConnection:\n");

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

asm(".global sceSslEnableOption\n"
    ".type sceSslEnableOption @function\n"
    "sceSslEnableOption:\n");

asm(".global sceSslEnableOptionInternal\n"
    ".type sceSslEnableOptionInternal @function\n"
    "sceSslEnableOptionInternal:\n");

asm(".global sceSslFreeCaCerts\n"
    ".type sceSslFreeCaCerts @function\n"
    "sceSslFreeCaCerts:\n");

asm(".global sceSslFreeCaList\n"
    ".type sceSslFreeCaList @function\n"
    "sceSslFreeCaList:\n");

asm(".global sceSslFreeSslCertName\n"
    ".type sceSslFreeSslCertName @function\n"
    "sceSslFreeSslCertName:\n");

asm(".global sceSslGetCaCerts\n"
    ".type sceSslGetCaCerts @function\n"
    "sceSslGetCaCerts:\n");

asm(".global sceSslGetCaList\n"
    ".type sceSslGetCaList @function\n"
    "sceSslGetCaList:\n");

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

asm(".global sceSslRecv\n"
    ".type sceSslRecv @function\n"
    "sceSslRecv:\n");

asm(".global sceSslReuseConnection\n"
    ".type sceSslReuseConnection @function\n"
    "sceSslReuseConnection:\n");

asm(".global sceSslSend\n"
    ".type sceSslSend @function\n"
    "sceSslSend:\n");

asm(".global sceSslSetMinSslVersion\n"
    ".type sceSslSetMinSslVersion @function\n"
    "sceSslSetMinSslVersion:\n");

asm(".global sceSslSetSslVersion\n"
    ".type sceSslSetSslVersion @function\n"
    "sceSslSetSslVersion:\n");

asm(".global sceSslSetVerifyCallback\n"
    ".type sceSslSetVerifyCallback @function\n"
    "sceSslSetVerifyCallback:\n");

asm(".global sceSslShowMemoryStat\n"
    ".type sceSslShowMemoryStat @function\n"
    "sceSslShowMemoryStat:\n");

asm(".global sceSslTerm\n"
    ".type sceSslTerm @function\n"
    "sceSslTerm:\n");

asm(".global sceSslUnloadCert\n"
    ".type sceSslUnloadCert @function\n"
    "sceSslUnloadCert:\n");
