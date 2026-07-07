#include "cString.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

static int cstring_grow(cString *obj, size_t target_cap){
    if(target_cap <= obj->cap) return 1;
    size_t new_cap = obj->cap < 16 ? 16 : obj->cap;
    if(new_cap <= SIZE_MAX / 2){
        new_cap <<= 1;
    }
    if(new_cap < target_cap){
        new_cap = target_cap;
    }
    char *data = realloc(obj->data, new_cap);
    if(!data) return 0;
    obj->data = data;
    obj->cap = new_cap;
    return 1;
}

cString *cstring_init(const char *str){
    if(!str) return NULL;
    cString *obj = malloc(sizeof(cString));
    if(!obj) return NULL;
    obj->len = strlen(str);
    obj->cap = obj->len + 1 < 16 ? 16 : obj->len + 1;
    obj->data = malloc(obj->cap);
    if(!obj->data){
        free(obj);
        return NULL;
    }
    memcpy(obj->data, str, obj->len);
    obj->data[obj->len] = '\0';
    return obj;
}

void cstring_copy(cString *restrict dst, const cString *restrict src){
    if(!dst || !src) return;
    if(dst == src) return;
    char *new_data = malloc(src->cap);
    if(!new_data) return;
    free(dst->data);
    dst->len = src->len;
    dst->cap = src->cap;
    dst->data = new_data;
    memcpy(dst->data, src->data, dst->len);
    dst->data[dst->len] = '\0';
}

cString *cstring_clone(const cString *src){
    if(!src) return NULL;
    cString *obj = malloc(sizeof(cString));
    if(!obj) return NULL;
    obj->len = src->len;
    obj->cap = src->cap;
    obj->data = malloc(obj->cap);
    if(!obj->data){
        free(obj);
        return NULL;
    }
    memcpy(obj->data, src->data, obj->len);
    obj->data[obj->len] = '\0';
    return obj;
}

void cstring_free(cString *obj){
    if(!obj) return;
    free(obj->data);
    free(obj);
}

void cstring_push_back(cString *restrict obj, const char *restrict str){
    if(!obj || !str) return;
    if(str[0] == '\0') return;
    size_t str_len = strlen(str);
    if(str_len > SIZE_MAX - obj->len - 1) return;
    if(!cstring_grow(obj, obj->len + str_len + 1)) return;
    memcpy(obj->data + obj->len, str, str_len);
    obj->len += str_len;
    obj->data[obj->len] = '\0';
}

void cstring_push_front(cString *restrict obj, const char *restrict str){
    if(!obj || !str) return;
    if(str[0] == '\0') return;
    size_t str_len = strlen(str);
    if(str_len > SIZE_MAX - obj->len - 1) return;
    if(!cstring_grow(obj, obj->len + str_len + 1)) return;
    memmove(obj->data + str_len, obj->data, obj->len + 1);
    memcpy(obj->data, str, str_len);
    obj->len += str_len;
}

char *cstring_insert(cString *restrict obj, const char *restrict str, size_t idx){
    if(!obj || !str) return NULL;
    if(idx > obj->len) idx = obj->len;
    if(str[0] == '\0') return obj->data + idx;
    size_t str_len = strlen(str);
    if(str_len > SIZE_MAX - obj->len - 1) return NULL;
    if(!cstring_grow(obj, obj->len + str_len + 1)) return NULL;
    memmove(obj->data + str_len + idx, obj->data + idx, obj->len - idx + 1);
    memcpy(obj->data + idx, str, str_len);
    obj->len += str_len;
    return obj->data + idx;
}

void cstring_pop_back(cString *obj, size_t n){
    if(!obj) return;
    if(n > obj->len) n = obj->len;
    obj->data[obj->len - n] = '\0';
    obj->len -= n;    
}

void cstring_pop_front(cString *obj, size_t n){
    if(!obj) return;
    if(n > obj->len) n = obj->len;
    memmove(obj->data, obj->data + n, obj->len - n + 1);
    obj->len -= n;
}

char *cstring_erase(cString *obj, size_t idx, size_t n){
    if(!obj) return NULL;
    if(idx >= obj->len) return obj->data + obj->len;
    if(n > obj->len - idx){
        obj->data[idx] = '\0';
        obj->len = idx;
        return obj->data + idx;
    }
    else{
        memmove(obj->data + idx, obj->data + idx + n, obj->len - idx - n + 1);
        obj->len -= n;
        return obj->data + idx;
    }
}

int cstring_compare(const cString *restrict a, const cString *restrict b){
    int ans = strcmp(a->data, b->data);
    return ans == 0 ? 0 : ans > 0 ? 1 : -1;
}

int cstring_equals(const cString *restrict a, const cString *restrict b){
    return a->len == b->len && !strcmp(a->data, b->data);
}

void cstring_fit(cString *obj){
    if(!obj) return;
    char *new_data = realloc(obj->data, obj->len+1);
    if(!new_data) return;
    obj->data = new_data;
    obj->cap = obj->len+1;
}

char *cstring_find(const cString *restrict obj, const char *restrict str){
    if(!obj || !str) return NULL;
    return strstr(obj->data, str);
}

size_t cstring_count(const cString *restrict obj, const char *restrict str){
    if(!obj || !str) return 0;
    size_t size = strlen(str), count = 0;
    char *ptr = obj->data;
    if(size == 0) return 0;
    while((ptr = strstr(ptr, str)) != NULL){
        count++;
        ptr += size;
    }
    return count;
}

size_t cstring_replace(cString *restrict obj, const char *restrict target, const char *restrict str){
    if(!obj || !target || !str) return 0;
    size_t target_len = strlen(target);
    if(!target_len) return 0;
    size_t str_len = strlen(str);
    char *first = strstr(obj->data, target);
    if(!first) return 0;
    if(str_len <= target_len){
        size_t count = 0;
        char *read = first;
        char *write = first;
        char *end = obj->data + obj->len;
        char *match = first;
        do{
            size_t gap = (size_t)(match - read);
            if(write != read) memmove(write, read, gap);
            write += gap;
            memcpy(write, str, str_len);
            write += str_len;
            read = match + target_len;
            count++;
        } while((match = strstr(read, target)) != NULL);
        if(write != read) memmove(write, read, (size_t)(end - read) + 1);
        obj->len -= count * (target_len - str_len);
        return count;
    }
    else{
        size_t count = 1;
        char *scan = first + target_len;
        char *match;
        while((match = strstr(scan, target)) != NULL){
            count++;
            scan = match + target_len;
        }
        size_t grow = str_len - target_len;
        if(count > (SIZE_MAX - obj->len) / grow) return 0;
        size_t new_len = obj->len + count * grow;
        if(new_len > SIZE_MAX - 1) return 0;
        char *new_data = malloc(new_len + 1);
        if(!new_data) return 0;
        char *write = new_data;
        char *read = obj->data;
        match = first;
        do{
            size_t gap = (size_t)(match - read);
            memcpy(write, read, gap);
            write += gap;
            memcpy(write, str, str_len);
            write += str_len;
            read = match + target_len;
        } while((match = strstr(read, target)) != NULL);
        memcpy(write, read, (size_t)(obj->data + obj->len - read));
        write += (size_t)(obj->data + obj->len - read);
        *write = '\0';
        free(obj->data);
        obj->data = new_data;
        obj->len = new_len;
        obj->cap = new_len + 1;
        return count;
    }
}

void cstring_reserve(cString *obj, size_t n){
    if(!obj) return;
    if(n > SIZE_MAX - 1) return;
    if(n + 1 <= obj->cap) return;
    char *new_data = realloc(obj->data, n + 1);
    if(!new_data) return;
    obj->data = new_data;
    obj->cap = n + 1;
}

cString *cstring_substr(const cString *obj, size_t idx, size_t n){
    if(!obj) return NULL;
    if(idx > obj->len) idx = obj->len;
    if(n > obj->len - idx) n = obj->len - idx;
    cString *out = malloc(sizeof(cString));
    if(!out) return NULL;
    out->len = n;
    out->cap = n + 1 < 16 ? 16 : n + 1;
    out->data = malloc(out->cap);
    if(!out->data){
        free(out);
        return NULL;
    }
    memcpy(out->data, obj->data + idx, n);
    out->data[n] = '\0';
    return out;
}

void cstring_trim(cString *obj){
    if(!obj) return;
    size_t start = 0, end = obj->len;
    while(start < end && isspace((unsigned char)obj->data[start])) start++;
    while(end > start && isspace((unsigned char)obj->data[end - 1])) end--;
    if(start > 0) memmove(obj->data, obj->data + start, end - start);
    obj->len = end - start;
    obj->data[obj->len] = '\0';
}

void cstring_to_upper(cString *obj){
    if(!obj) return;
    for(size_t i=0;i<obj->len;i++){
        obj->data[i] = (char)toupper((unsigned char)obj->data[i]);
    }
}

void cstring_to_lower(cString *obj){
    if(!obj) return;
    for(size_t i=0;i<obj->len;i++){
        obj->data[i] = (char)tolower((unsigned char)obj->data[i]);
    }
}

int cstring_starts_with(const cString *restrict obj, const char *restrict str){
    if(!obj || !str) return 0;
    size_t str_len = strlen(str);
    if(str_len > obj->len) return 0;
    return memcmp(obj->data, str, str_len) == 0;
}

int cstring_ends_with(const cString *restrict obj, const char *restrict str){
    if(!obj || !str) return 0;
    size_t str_len = strlen(str);
    if(str_len > obj->len) return 0;
    return memcmp(obj->data + obj->len - str_len, str, str_len) == 0;
}

void cstring_reverse(cString *obj){
    if(!obj) return;
    for(size_t i=0,j=obj->len;i+1<j--;i++){
        char tmp = obj->data[i];
        obj->data[i] = obj->data[j];
        obj->data[j] = tmp;
    }
}
