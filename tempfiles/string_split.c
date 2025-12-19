#include <stdio.h>
#include <string.h>

int main()
{
    char str[] = "GET /favicon.ico HTTP/1.1\nHost: localhost:3000\nConnection: keep-alive\nPragma: no-cache\nCache-Control: no-cache\nsec-ch-ua-platform: \"Windows\"\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/142.0.0.0 Safari/537.36\nsec-ch-ua: \"Chromium\";v=\"142\", \"Google Chrome\";v=\"142\", \"Not_A Brand\";v=\"99\"\nDNT: 1\nsec-ch-ua-mobile: ?0\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\nSec-Fetch-Site: same-origin\nSec-Fetch-Mode: no-cors\nSec-Fetch-Dest: image\nReferer: http://localhost:3000/stream\nAccept-Encoding: gzip, deflate, br, zstd\nAccept-Language: en-US,en;q=0.9";
    const char *delimiter = "\n";
    char *token;

    // Get the first token
    token = strtok(str, delimiter);

    // Continue getting tokens until strtok returns NULL
    // while (token != NULL)
    // {
    //     printf("%s\n", token);
    //     token = strtok(NULL, delimiter); // Pass NULL for subsequent calls
    // }
    printf("%s\n", token);
    const char *t1, *t2, *t3;
    t1 = strtok(token, " ");
    t2 = strtok(NULL, " ");
    t3 = strtok(NULL, " ");
    printf("t1 : %s\nt2 : %s\nt3 : %s\n", t1, t2, t3);

    return 0;
}