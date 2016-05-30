//
//  ResourcesDecode.h
//  newGame
//
//  Created by jinda.w on 14-7-28.
//
//

#ifndef __newGame__ResourcesDecode__
#define __newGame__ResourcesDecode__

#include <iostream>

class ResourcesDecode
{
public:
    unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long *pSize);
    
    unsigned char* decodeData(unsigned char* buf, unsigned long size, unsigned long *pSize);
    
    static ResourcesDecode* sharedDecode();
    
    void setXXTeaKey(const char* xxtea, int xxteakeyLen, const char* xxteaSign, int xxteaSignLen);
    void cleanupXXTEAKeyAndSign(void);
    
private:
    static ResourcesDecode* m_resourcesDecode;
    ResourcesDecode();
    ResourcesDecode(ResourcesDecode& other);
    const ResourcesDecode& operator= (ResourcesDecode& other);
public:
    bool m_xxteaEnabled;
    char* m_xxteaKey;
    int m_xxteaKeyLen;
    char* m_xxteaSign;
    int m_xxteaSignLen;
};

#endif /* defined(__newGame__ResourcesDecode__) */
