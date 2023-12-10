#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "strbuf.h"
// struct strbuf
// {
//     int len;   // 当前缓冲区（字符串）长度
//     int alloc; // 当前缓冲区（字符串）容量
//     char *buf; // 缓冲区（字符串）
// };

// 初始化 sb 结构体，容量为 alloc。
void strbuf_init(struct strbuf *sb, size_t alloc)
{
    sb->len = 0;
    sb->alloc = alloc;
    sb->buf = (char *)malloc(alloc * sizeof(char));
    if (sb->buf == NULL)
    {
        fprintf(stderr, "内存分配失败\n");
        return;
    }
}

// 将字符串填充到 sb 中，长度为 len, 容量为 alloc。
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)
{
    // if (len > sb->alloc)
    // {
    //     sb->buf = (char *)realloc(sb->buf, len * sizeof(char));
    // }
    sb->alloc = alloc;
    sb->len = len;
    sb->buf = (char *)str;
}

// 释放 sb 结构体的内存。
void strbuf_release(struct strbuf *sb)
{
    sb->alloc = 0;
    sb->len = 0;
    free(sb->buf);
    sb->buf = NULL;
}

// 交换两个 strbuf。
void strbuf_swap(struct strbuf *a, struct strbuf *b)
{
    int temp_len = a->len;
    a->len = b->len;
    b->len = temp_len;

    int temp_alloc = a->alloc;
    a->alloc = b->alloc;
    b->alloc = temp_alloc;

    char *temp_buf = a->buf;
    a->buf = b->buf;
    b->buf = temp_buf;
}

// 将 sb 中的原始内存取出，并将 sz 设置为其 alloc 大小 。
char *strbuf_detach(struct strbuf *sb, size_t *sz)
{
    *sz = sb->alloc;
    char *detach_buf = sb->buf;
    return detach_buf;
}

// 比较两个 strbuf 的内存是否相同。
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    if (first->len != second->len)
    {
        return 1;
    }
    return memcmp(first->buf, second->buf, first->len) != 0;
}

// 清空 sb。
void strbuf_reset(struct strbuf *sb)
{
    sb->len = 0;
    free(sb->buf);
    sb->buf = NULL;
}

//>-----------------------------------

void strbuf_grow(struct strbuf *sb, size_t extra)
{
    if (sb->alloc > sb->len + extra)
        return;
    sb->buf = (char *)realloc(sb->buf, (sb->len + extra + 1) * sizeof(char));
    if (sb->buf == NULL)
    {
        fprintf(stderr, "重新分配内存失败\n");
        return;
    }
    sb->alloc = sb->len + extra;
}

// 向 sb 追加长度为 len 的数据 data。
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    strbuf_grow(sb, len);
    memcpy(sb->buf + sb->len, data, len);
    sb->len += len;
    sb->buf[sb->len] = '\0';
}

// 向 sb 追加一个字符 c。
void strbuf_addch(struct strbuf *sb, int c)
{
    strbuf_add(sb, &c, 1);
}

// 向 sb 追加一个字符串 s。
void strbuf_addstr(struct strbuf *sb, const char *s)
{
    strbuf_add(sb, s, strlen(s));
}

// 向一个 sb 追加另一个 strbuf的数据。
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_add(sb, sb2->buf, sb2->len);
}

// 设置 sb 的长度 len。
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    if (len > sb->alloc)
    {
        sb->buf = (char *)realloc(sb->buf, len * sizeof(char));
        sb->alloc = len * sizeof(char);
        sb->len = len;
        return;
    }
    sb->len = len;
    sb->buf[sb->len] = '\0';
}

// 计算 sb 目前仍可以向后追加的字符串长度。
size_t strbuf_avail(const struct strbuf *sb)
{
    size_t tlen;
    tlen = sb->alloc - sb->len - 1;
    return tlen;
}

// 向 sb 内存坐标为 pos 位置插入长度为 len 的数据 data。
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)
{
    strbuf_grow(sb, len);
    memmove(sb->buf + pos + len, sb->buf + pos, sb->len - pos);
    memcpy(sb->buf + pos, data, len);
    sb->buf[sb->len + len] = '\0';
    sb->len += len;
}

//>------------------------------

// 去除 sb 缓冲区左端的所有 空格，tab, '\t'。
void strbuf_ltrim(struct strbuf *sb)
{
    size_t count = 0;
    while (sb->buf[count] == ' ' || sb->buf[count] == '\t')
    {
        count++;
    }
    memmove(sb->buf, sb->buf + count, sb->len - count);
    sb->len -= count;
}

// 去除 sb 缓冲区右端的所有 空格，tab, '\t'。
void strbuf_rtrim(struct strbuf *sb)
{
    int count = sb->len - 1;
    while (sb->buf[count] == '\t' || sb->buf[count] == ' ')
    {
        sb->buf[count--] = '\0';
    }
    sb->len = count + 1;
}

// 删除 sb 缓冲区从 pos 坐标长度为 len 的内容。
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    if (pos + len >= sb->len)
    {
        sb->buf = (char *)realloc(sb->buf, pos * sizeof(char) + 1);
        sb->buf[pos] = '\0';
        sb->len = pos;
    }
    else
    {
        memmove(sb->buf + pos, sb->buf + pos + len, len);
        sb->buf = (char *)realloc(sb->buf, (sb->len - len) * sizeof(char));
        sb->buf[sb->len - len] = '\0';
        sb->len = sb->len - len;
    }
}

// >-----------------------------

// sb 增长 hint ? hint : 8192 大小， 然后将文件描述符为 fd 的所有文件内容追加到 sb 中。
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    
    if (fd < 0)
    {
        return 0;
    }
    FILE *f = fdopen(fd, "r");
    char ch;
    if (f == NULL)
        return sb->len;
    strbuf_grow(sb, (hint ? hint : 8192) + 1);
    while ((ch = getc(f)) != EOF)
    {

        strbuf_addch(sb, ch);
    }
    fclose(f);

    return sb->len;
}

// 将 将文件句柄为 fp 的一行内容（抛弃换行符）读取到 sb 。
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    int ch;
    if (fp == NULL)
        return 0;
    while ((ch = getc(fp)) != EOF)
    {
        if (ch == '\n')
            break;
        strbuf_add(sb, &ch, 1);
    }
    return sb->len;
}

//>--------------------------------------

// 切割字符串
//  将长度为 len 的字符串 str 根据切割字符 terminator 切成多个 strbuf,
//  并从结果返回，max 可以用来限定最大切割数量。返回 struct strbuf 的指针数组，数组的最后元素为 NULL


struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    if (len == 0 || str == NULL)
        return NULL;

    // 记录末尾
    size_t temp = len;
    while (temp > 0 && *(str + temp - 1) == terminator)
    {
        temp--;
    }
    char *end = (char *)(str + temp - 1);

    // 记录开头
    temp = 0;
    while (temp < len && *(str + temp) == terminator)
    {
        temp++;
    }

    if (temp == len)
    {
        // 整个字符串都是分隔符
        struct strbuf **sb = (struct strbuf **)malloc(sizeof(struct strbuf *));
        if (sb == NULL)
        {
            // 内存分配失败
            return NULL;
        }
        sb[0] = NULL;
        return sb;
    }

    char *start = (char *)(str + temp);

    char *pmove = start;

    // 计算最大可能的切割数量
    size_t possibleMax = 0;
    while (pmove <= end)
    {
        while (*pmove != terminator)
        {
            pmove++;
        }

        while (*pmove == terminator)
        {
            ++pmove;
        }

        ++possibleMax;
    }

    // 使用实际的max和可能的max的较小值
    size_t finalMax = (max < possibleMax) ? max : possibleMax;

    struct strbuf **sb = (struct strbuf **)malloc(sizeof(struct strbuf *) * (finalMax + 1));
    if (sb == NULL)
    {
        // 内存分配失败
        return NULL;
    }

    pmove = start; // 重置 pmove 的位置
    int count = 0;
    while (count < finalMax)
    {
        while (*pmove == terminator)
        {
            pmove++;
        }

        char *currentTerminator = (char*)memchr(pmove, terminator,(size_t)(end - pmove+1));

        size_t segmentLen = (currentTerminator != NULL) ? (size_t)(currentTerminator - pmove) : (size_t)(end - pmove+1);

        sb[count] = (struct strbuf *)malloc(sizeof(struct strbuf));
        if (sb[count] == NULL)
        {
            // 内存分配失败
            // 释放之前已分配的内存
            for (size_t i = 0; i < count; ++i)
            {
                free(sb[i]);
            }
            free(sb);
            return NULL;
        }

        // 确保分配内存后才使用 memmove
        sb[count]->buf = (char *)malloc(segmentLen + 1);
        if (sb[count]->buf == NULL)
        {
            // 内存分配失败
            // 释放之前已分配的内存
            free(sb[count]);
            for (size_t i = 0; i < count; ++i)
            {
                free(sb[i]);
            }
            free(sb);
            return NULL;
        }

        // 使用 memmove
        memmove(sb[count]->buf, pmove, segmentLen);
        sb[count]->buf[segmentLen] = '\0';
        sb[count]->len = segmentLen;
        sb[count]->alloc = segmentLen+1;

        count++;
        

        if (pmove != NULL)
        {
            while (*pmove == terminator)
            {
                pmove++;
            }
        }
        pmove = currentTerminator;
    }

    sb[count] = NULL; 
    return sb;
}


// 判断一个 strbuf 是否以指定字符串开头
// target_str : 目标字符串，str : 前缀字符串，strnlen : target_str 长度 ，前缀相同返回 true 失败返回 false
    bool strbuf_begin_judge(char *target_str, const char *str, int strnlen)
    {
        if (str == NULL)
            return true;
        return memcmp(target_str, str, strlen(str)) == 0;
    }

// 获取字符串从坐标 [begin, end) 的所有内容（可以分成引用和拷贝两个模式） 。
//	target_str : 目标字符串，begin : 开始下标，end 结束下标。
// len : target_buf的长度，参数不合法返回 NULL. 下标从0开始，[begin, end)区间。
char *strbuf_get_mid_buf(char *target_buf, int begin, int end, int len)
{
    if (target_buf == NULL)
        return NULL;
    char *str = (char *)malloc(len * sizeof(char));
    memcpy(str, target_buf + begin, end - begin);
    str[end - begin] = '\0';
    return str;
}