#ifndef USER
#define USER

struct birthdate {
    int year;
    int month;
    int date;
};
typedef struct users {
    int user_type; // 0����Ա 1ҽ�� 2����  
    long long id;
    char name[20];
    char password[33];
    char phone[100];
    char department[100]; // ����ҽ������  
    char title[100];      // ����ҽ������  
    char sex[20];
    struct birthdate birth;
    struct users* next; // ָ����һ���ڵ��ָ��  
} USERS;

void create_data();
int create_user(int is_admin);
void add_user(USERS* new_user);
int read_user_data();
USERS* login();
void free_users();
USERS* find_user_by_id(long long input_id);
void display_user_info(USERS* user);
int delete_user(long long input_id);
int user_main(USERS* current);

#endif