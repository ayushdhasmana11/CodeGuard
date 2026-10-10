#include "token_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copy_string(const char *source) {
    if (source == NULL) return NULL;
    size_t length = strlen(source) + 1;
    char *copy = (char *)malloc(length);
    if (copy != NULL) memcpy(copy, source, length);
    return copy;
}

TokenNode *token_list_append(TokenNode **head, const char *token) {
    if (head == NULL || token == NULL) return NULL;
    TokenNode *node = (TokenNode *)malloc(sizeof(TokenNode));
    if (node == NULL) return NULL;
    node->token = copy_string(token);
    if (node->token == NULL) { free(node); return NULL; }
    node->next = NULL;
    if (*head == NULL) *head = node;
    else {
        TokenNode *current = *head;
        while (current->next != NULL) current = current->next;
        current->next = node;
    }
    return node;
}

void token_list_print(const TokenNode *head) {
    const TokenNode *current = head;
    while (current != NULL) {
        printf("%s", current->token);
        if (current->next != NULL) printf(" -> ");
        current = current->next;
    }
    printf("\\n");
}

void token_list_free(TokenNode **head) {
    if (head == NULL) return;
    TokenNode *current = *head;
    while (current != NULL) {
        TokenNode *next = current->next;
        free(current->token);
        free(current);
        current = next;
    }
    *head = NULL;
}

MatchNode *match_list_append(MatchNode **head, const char *file_a,
                             const char *file_b, double similarity) {
    if (head == NULL || file_a == NULL || file_b == NULL) return NULL;
    MatchNode *node = (MatchNode *)malloc(sizeof(MatchNode));
    if (node == NULL) return NULL;
    node->file_a = copy_string(file_a);
    node->file_b = copy_string(file_b);
    if (node->file_a == NULL || node->file_b == NULL) {
        free(node->file_a); free(node->file_b); free(node); return NULL;
    }
    node->similarity = similarity;
    node->next = NULL;
    if (*head == NULL) *head = node;
    else {
        MatchNode *current = *head;
        while (current->next != NULL) current = current->next;
        current->next = node;
    }
    return node;
}

void match_list_print(const MatchNode *head) {
    const MatchNode *current = head;
    while (current != NULL) {
        printf("%s <-> %s : %.2f%% similarity\\n",
               current->file_a, current->file_b, current->similarity);
        current = current->next;
    }
}

void match_list_free(MatchNode **head) {
    if (head == NULL) return;
    MatchNode *current = *head;
    while (current != NULL) {
        MatchNode *next = current->next;
        free(current->file_a); free(current->file_b); free(current);
        current = next;
    }
    *head = NULL;
}
