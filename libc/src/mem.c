#include <libc/mem.h>
int memcmp(char* str1, char* str2, int len){
        for(int i = 0; i < len; i++){
                if(str1[i] != str2[i])
                        return 1;
        }
        return 0;
}

void *memset(void* bufptr, int value, size_t size) {
        unsigned char* buf = (unsigned char*) bufptr;
        for (size_t i = 0; i < size; i++)
                buf[i] = (unsigned char) value;
        return bufptr;
}
void memcpy(char* buf1, char* buf2, size_t len){
        for(size_t i = 0; i < len; i++)
                buf2[i] = buf1[i];
}

