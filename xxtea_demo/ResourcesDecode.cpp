//
//  ResourcesDecode.cpp
//  newGame
//
//  Created by jinda.w on 14-7-28.
//
//

extern "C"
{
#include "xxtea.h"
}

#include "ResourcesDecode.h"
#include "CCFileUtils.h"
#include "cocos2d.h"

USING_NS_CC;

ResourcesDecode* ResourcesDecode::m_resourcesDecode = NULL;

ResourcesDecode* ResourcesDecode::sharedDecode()
{
    if (m_resourcesDecode)
    {
        return m_resourcesDecode;
    }
    else
    {
        m_resourcesDecode = new ResourcesDecode();
        return m_resourcesDecode;
    }
}

ResourcesDecode::ResourcesDecode()
{
    
}

void ResourcesDecode::setXXTeaKey(const char *key, int keyLen, const char *sign, int signLen)
{
    if (key && keyLen && sign && signLen)
    {
        m_xxteaKey = (char*)malloc(keyLen);
        memcpy(m_xxteaKey, key, keyLen);
        m_xxteaKeyLen = keyLen;
        
        m_xxteaSign = (char*)malloc(signLen);
        memcpy(m_xxteaSign, sign, signLen);
        m_xxteaSignLen = signLen;
        
        m_xxteaEnabled = true;
    }
    else
    {
        m_xxteaEnabled = false;
    }
}
void ResourcesDecode::cleanupXXTEAKeyAndSign(void)
{
    if (m_xxteaKey)
    {
        free(m_xxteaKey);
        m_xxteaKey = NULL;
        m_xxteaKeyLen = 0;
    }
    if (m_xxteaSign)
    {
        free(m_xxteaSign);
        m_xxteaSign = NULL;
        m_xxteaSignLen = 0;
    }
    
}

unsigned char* ResourcesDecode::getFileData(const char* pszFileName, const char* pszMode, unsigned long *pSize)
{
    unsigned long size;
    unsigned char* buf = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(pszFileName, pszMode, &size);
    
    if (NULL == buf)
    {
        return NULL;
    }
    unsigned char* buffer = NULL;
    ResourcesDecode* decode = ResourcesDecode::sharedDecode();
    bool isXXTEA = decode && decode->m_xxteaEnabled;
    
    for (unsigned int i = 0; isXXTEA && i < decode->m_xxteaSignLen && i < size; ++i)
    {
        isXXTEA = buf[i] = decode->m_xxteaSign[i];
    }
    
    if (isXXTEA)
    {
        //decrypt XXTEA
        xxtea_long  len = 0;
        buffer = xxtea_decrypt(buf+decode->m_xxteaSignLen, (xxtea_long)size -(xxtea_long)decode->m_xxteaSignLen, (unsigned char*)decode->m_xxteaKey, (xxtea_long)decode->m_xxteaKeyLen, &len);
        delete [] buf;
        buf = NULL;
        size = len;
    }
    else
    {
        buffer = buf;
    }
    
    if (pSize)
    {
        *pSize = size;
    }
    return buffer;
}

unsigned char* ResourcesDecode::decodeData(unsigned char* buf, unsigned long size, unsigned long *pSize)
{
    CCAssert(buf != NULL, "decodeData buf not NULL");

    unsigned char* buffer = NULL;
    ResourcesDecode* decode = ResourcesDecode::sharedDecode();
    bool isXXTEA = decode && decode->m_xxteaEnabled;
    
    for (unsigned int i = 0; isXXTEA && i < decode->m_xxteaSignLen && i < size; ++i)
    {
        isXXTEA = buf[i] == decode->m_xxteaSign[i];
    }
    
    if (isXXTEA)
    {
        //decrypt XXTEA
        xxtea_long  len = 0;
        buffer = xxtea_decrypt(buf+decode->m_xxteaSignLen, (xxtea_long)size -(xxtea_long)decode->m_xxteaSignLen, (unsigned char*)decode->m_xxteaKey, (xxtea_long)decode->m_xxteaKeyLen, &len);
        delete [] buf;
        buf = NULL;
        size = len;
    }
    else
    {
        buffer = buf;
    }
    
    if (pSize)
    {
        *pSize = size;
    }
    return buffer;
}