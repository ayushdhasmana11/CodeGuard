#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TokenNode {
    char *token;
    struct TokenNode *next;
} TokenNode;

typedef struct MatchNode {
    char *file_a;
    char *file_b;
    double similarity;
    struct MatchNode *next;
} MatchNode;

TokenNode *token_list_append(TokenNode **head, const char *token);
void token_list_print(const TokenNode *head);
void token_list_free(TokenNode **head);

MatchNode *match_list_append(MatchNode **head, const char *file_a,
                             const char *file_b, double similarity);
void match_list_print(const MatchNode *head);
void match_list_free(MatchNode **head);

#ifdef __cplusplus
}
#endif
#endif
