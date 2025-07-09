#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define HOSTS_PATH "C:\\Windows\\System32\\drivers\\etc\\hosts"
#define DOMAIN_NAME "app.loc"
#define IP_BUFFER_SIZE 46
#define LINE_BUFFER_SIZE 512

int get_local_ip(char *ip_buffer, int buffer_len) {
    WSADATA wsaData;
    char hostname[256];
    struct hostent *host;
    struct in_addr addr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return -1;
    }

    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        WSACleanup();
        return -1;
    }

    host = gethostbyname(hostname);
    if (host == NULL) {
        WSACleanup();
        return -1;
    }

    addr.s_addr = *(u_long*)host->h_addr_list[0];
    strncpy(ip_buffer, inet_ntoa(addr), buffer_len - 1);
    ip_buffer[buffer_len - 1] = '\0';

    WSACleanup();
    return 0;
}

void update_hosts_file(const char *ip) {
    FILE *fp;
    char lines[100][LINE_BUFFER_SIZE];
    int count = 0;
    int found = 0;
    int i;

    fp = fopen(HOSTS_PATH, "r");
    if (!fp) {
        perror("Failed to open hosts file for reading");
        return;
    }

    while (fgets(lines[count], LINE_BUFFER_SIZE, fp) && count < 100) {
        if (strstr(lines[count], DOMAIN_NAME)) {
            snprintf(lines[count], LINE_BUFFER_SIZE, "%s\t%s\n", ip, DOMAIN_NAME);
            found = 1;
        }
        count++;
    }
    fclose(fp);

    if (!found && count < 100) {
        snprintf(lines[count], LINE_BUFFER_SIZE, "%s\t%s\n", ip, DOMAIN_NAME);
        count++;
    }

    fp = fopen(HOSTS_PATH, "w");
    if (!fp) {
        perror("Failed to open hosts file for writing");
        return;
    }

    for (i = 0; i < count; i++) {
        fputs(lines[i], fp);
    }

    fclose(fp);
    printf("? Updated hosts file: %s -> %s\n", DOMAIN_NAME, ip);
}

int main() {
    char ip[IP_BUFFER_SIZE];

    while (1) {
        if (get_local_ip(ip, IP_BUFFER_SIZE) == 0) {
            update_hosts_file(ip);
        } else {
            fprintf(stderr, "? Failed to get IP address\n");
        }

        Sleep(15000);  // 15 seconds
    }

    return 0;
}
