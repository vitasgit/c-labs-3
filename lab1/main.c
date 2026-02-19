#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[]) {
    int ngroups;
    //gid_t groups;  // Вопрос: что значит параметр 100, почему когда меняю значение то меняются и группы?
    struct passwd *pw;
    struct group *gr;
    const char *name = "vitaly";

    pw = getpwnam(name);

    // Узнаём сколько групп у пользователя
    ngroups = 0;
    // Первый вызов с ngroups=0 вернёт -1, но запишет реальное количество в ngroups
    getgrouplist(name, pw->pw_gid, NULL, &ngroups);
    printf("ngroups = %d\n", ngroups);

    gid_t groups[ngroups];
    getgrouplist(name, pw->pw_gid, groups, &ngroups);
    // Выводим имя каждой группы
    printf("Группы:\n");
    for (int i = 0; i < ngroups; i++) {
        gr = getgrgid(groups[i]);
        if (gr != NULL) {
            printf(" (%s)\n", gr->gr_name);
        }
    }

    

    return 0;
}
