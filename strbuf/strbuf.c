#include "strbuf.h"
void strbuf_init(struct strbuf *sb, size_t alloc)//ok
{
    sb->alloc=alloc;
    sb->len=0;
    sb->buf=(char*)calloc(alloc,sizeof(char));
}
void strbuf_attach(struct strbuf *sb, void *str, size_t len, size_t alloc)//ok
{
    sb->alloc=alloc;
    strbuf_grow(sb,len+1);
    memcpy(sb->buf,(char*)str,len);
    (sb->buf)[len]='\0';
    strbuf_setlen(sb,len);
}
void strbuf_release(struct strbuf *sb)//ok
{
    free(sb->buf);
    sb->buf=NULL;
    sb->len=0;
    sb->alloc=0;
}
void strbuf_swap(struct strbuf *a, struct strbuf *b)//ok
{
    struct strbuf temp=*a;
    *a=*b;
    *b=temp;
}
char *strbuf_detach(struct strbuf *sb, size_t *sz)//ok
{
    *sz=sb->alloc;
    return sb->buf;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)//ok
{
    int n;
    if(first->alloc==second->alloc)
    {      
        if(memcmp(first->buf,second->buf,first->alloc)==0)
        {
            n=1;
        }
        else
        {
            n=0;
        }
    }
    else
    {
        return n;
    }
}
void strbuf_reset(struct strbuf *sb)//ok
{
    for(int i=0;i<=sb->len;i++)
    {
        sb->buf[i]='0';
    }
    sb->len=0;
}
void strbuf_grow(struct strbuf *sb, size_t extra)//ok
{
    if(sb->alloc-sb->len-1<extra)
    {
        sb->buf=(char*)realloc(sb->buf,(sb->len+extra+1)*sizeof(char));
        sb->alloc=sb->len+extra+1;
    }
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)//ok
{
    strbuf_grow(sb,len);
    memcpy(sb->buf+sb->len,(char*)data,len);
    (sb->buf)[sb->len+len]='\0';
    strbuf_setlen(sb,sb->len+len);
}
void strbuf_addch(struct strbuf *sb, int c)//ok
{
    strbuf_grow(sb,1);
    (sb->buf)[sb->len]=c;
    (sb->buf)[sb->len+1]='\0';
    strbuf_setlen(sb,sb->len+1);
}
void strbuf_addstr(struct strbuf *sb, const char *s)//ok
{
    strbuf_grow(sb,strlen(s));
    strcat(sb->buf,s);
    strbuf_setlen(sb,sb->len+strlen(s));
}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)//ok
{
    strbuf_grow(sb,sb2->len);
    memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
    (sb->buf)[sb->len+sb2->len]='\0';
    //strcat(sb->buf,sb2->buf);
    strbuf_setlen(sb,sb->len+sb2->len);
}
void strbuf_setlen(struct strbuf *sb, size_t len)//ok
{
    sb->len=len;
}
size_t strbuf_avail(const struct strbuf *sb)//ok
{
    size_t avail=sb->alloc-sb->len-1;
    return avail;
}
void strbuf_insert(struct strbuf *sb, size_t pos, const void *data, size_t len)//?
{
    strbuf_grow(sb,len);
    memmove(sb->buf+pos+len,sb->buf+pos-1,sb->len-pos+2);
    memcpy(sb->buf+pos-1,(char*)data,len);
    strbuf_setlen(sb,sb->len+len);
}
void strbuf_ltrim(struct strbuf *sb)
{
    int slow=0;
    for(int fast=0;fast<=sb->len;fast++)
    {
        if(sb->buf[fast]==' '||sb->buf[fast]=='\t')
        {
            sb->buf[slow]=sb->buf[fast];
            slow++;
        }
    }
}
void strbuf_rtrim(struct strbuf *sb)
{
    int slow=0;
    for(int fast=sb->len;fast<=sb->alloc;fast++)
    {
        if(sb->buf[fast]==' '||sb->buf[fast]=='\t')
        {
            sb->buf[slow]=sb->buf[fast];
            slow++;
        }
    }
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len)
{
    memmove(sb->buf+pos-1,sb->buf+pos+len-1,sb->len-(pos-1)-len+1);
    strbuf_setlen(sb,sb->len-len);
}
ssize_t strbuf_read(struct strbuf *sb, int fd, size_t hint)
{
    strbuf_grow(sb,hint?hint:8192);
    int ch=0;
    FILE *fp=fdopen(fd,"r");
    while (ch!=EOF)
    {
        ch=getc(fp);
        strbuf_addch(sb,ch);
    }
    return sb->len;
}
int strbuf_getline(struct strbuf *sb, FILE *fp)
{
    int ch=0;
    while (ch!=EOF&&ch!='\n')
    {
        ch=getc(fp);
        strbuf_addch(sb,ch);
    } 
    return sb->len;
}
struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{

}
bool strbuf_begin_judge(char* target_str, const char* str, int strlen)
{

}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{

}