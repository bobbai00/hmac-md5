#include "hmac.h"


unsigned char * ipad(int times) {
    unsigned char * ret = (unsigned char *)malloc(sizeof(unsigned char) * times);
    for(int i = 0;i<times;i++) {
        ret[i] = 0x36;
    }
    return ret;
};


unsigned char * opad(int times) {
    unsigned char * ret = (unsigned char *)malloc(sizeof(unsigned char) * times);
    for(int i = 0;i<times;i++) {
        ret[i] = 0x5c;
    }
    return ret;
};

unsigned char * padKey(unsigned char * key,int key_size ,int B) {
        unsigned char * ret = (unsigned char *)malloc(sizeof(unsigned char) * B);
    for(int i = 0;i<B;i++) {
        ret[i] = 0;
        if(i < key_size){
            ret[i] = key[i];
        }
    }
    return ret;

};

void XOR(unsigned char * key, unsigned char * val, int size) {
    for(int i = 0;i < size;i++) {
        key[i] = key[i] ^ val[i];
    }
}

void HMAC(unsigned char * key, int B, unsigned char * msg, unsigned char * output, int* size){
    int key_size = 0;
    while(key[key_size] != '\0') {
        key_size++;
    }
    unsigned char * pad_key = padKey(key, key_size,B);
    
};



