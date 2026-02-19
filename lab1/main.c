#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int ngroups;
    gid_t *groups;
    struct passwd *pw;
    struct group *gr;
    const char *name = "vitaly";

    // ссылка на структуру passwd
    pw = getpwnam(name);

    // Узнаём сколько групп у пользователя
    ngroups = 0;
    // Первый вызов с ngroups=0 вернёт -1, но запишет реальное количество в ngroups
    getgrouplist(name, pw->pw_gid, NULL, &ngroups);
    printf("ngroups = %d\n", ngroups);

    // Выделяем память под массив групп и получаем список групп
    groups = malloc(sizeof(*groups)*ngroups);
    getgrouplist(name, pw->pw_gid, groups, &ngroups);

    printf("Группы:\n");
    for (int j = 0; j < ngroups; j++) {
        printf("%d", groups[j]);
        gr = getgrgid(groups[j]);
        if (gr != NULL) printf(" (%s)", gr->gr_name);
        printf("\n");
    }


    // Создаем массив для отслеживания уникальных пользователей
    // Сначала определяем максимальное количество пользователей (ориентировочно)
    char **classmates = NULL;
    int classmates_count = 0;
    int classmates_capacity = 0;

    // Проходим по каждой группе пользователя
    for (int i = 0; i < ngroups; i++) {
        gr = getgrgid(groups[i]);
        if (gr != NULL) {
            // Проходим по всем пользователям в этой группе
            for (int j = 0; gr->gr_mem[j] != NULL; j++) {
                const char *member_name = gr->gr_mem[j];

                // Пропускаем самого исходного пользователя
                if (strcmp(member_name, name) == 0) {continue;}

                // Проверяем, не добавили ли мы этого пользователя уже
                int found = 0;
                for (int k = 0; k < classmates_count; k++) {
                    if (strcmp(classmates[k], member_name) == 0) {
                        found = 1;
                        continue;
                    }
                }

                // Если пользователь новый, добавляем его в список
                if (!found) {
                    // Добавляем нового одногруппника
                    classmates[classmates_count] = strdup(member_name);
                    classmates_count++;
                }

                printf("  - %s\n", member_name);
            }
        }
    }

    // Выводим полный список одногруппников
    printf("\nВсе одногруппники пользователя %s:\n", name);
    for (int i = 0; i < classmates_count; i++) {
        printf("- %s\n", classmates[i]);
    }

    // Освобождаем память
    for (int i = 0; i < classmates_count; i++) {
        free(classmates[i]);
    }
    free(classmates);
    free(groups);

    return 0;
}