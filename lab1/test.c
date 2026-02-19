#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

// Проверяем, есть ли имя username в массиве членов группы gr_mem
int is_member(char **gr_mem, const char *username) {
    for (int i = 0; gr_mem[i] != NULL; i++) {
        if (strcmp(gr_mem[i], username) == 0) {
            return 1;  // нашли
        }
    }
    return 0;  // не нашли
}

// Проверяем, есть ли имя уже в нашем списке результатов
int already_found(char **result, int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(result[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    const char *username = "vitaly";

    struct passwd *pw = getpwnam(username);

    // Шаг 1: получаем группы нашего пользователя (как в День 1)
    int ngroups = 0;
    getgrouplist(username, pw->pw_gid, NULL, &ngroups);
    gid_t *groups = malloc(ngroups * sizeof(gid_t));
    getgrouplist(username, pw->pw_gid, groups, &ngroups);

    // Шаг 2: массив для хранения найденных одногруппников
    char **result = malloc(256 * sizeof(char *));
    int result_count = 0;

    // Шаг 3: перебираем все группы в системе
    setgrent();
    struct group *gr;
    while ((gr = getgrent()) != NULL) {

        // Проверяем: входит ли наш пользователь в эту группу?
        if (!is_member(gr->gr_mem, username)) {
            continue;  // нет — пропускаем группу
        }

        // Да — перебираем всех членов этой группы
        for (int i = 0; gr->gr_mem[i] != NULL; i++) {
            const char *member = gr->gr_mem[i];

            // Пропускаем самого пользователя
            if (strcmp(member, username) == 0) continue;

            // Пропускаем если уже добавили
            if (already_found(result, result_count, member)) continue;

            // Добавляем в результат
            result[result_count] = strdup(member);  // копируем строку
            result_count++;
        }
    }
    endgrent();

    // Шаг 4: выводим результат
    printf("Одногруппники пользователя %s:\n", username);
    for (int i = 0; i < result_count; i++) {
        printf("  %s\n", result[i]);
    }

    // Освобождаем память
    for (int i = 0; i < result_count; i++) {
        free(result[i]);
    }
    free(result);
    free(groups);

    return 0;
}