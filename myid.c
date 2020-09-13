#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <uuid/uuid.h>


int main(int argc, char const *argv[]) {
    struct passwd *s;
    int gid, list[100];
    int amount = 100;
    if (argc > 1) {
        s = getpwnam(argv[1]);
        gid = s->pw_gid;

        printf("uid=%d(%s) ", s->pw_uid, argv[1]);
        printf("gid=%d(%s) groups=", gid, getgrgid(gid)->gr_name);
        
        getgrouplist(argv[1], gid, list, &amount);
    }
    else {
    int uid = getuid(), gid = getgid();

    printf("uid=%d(%s) ", uid, getpwuid(uid)->pw_name);
    printf("gid=%d(%s) groups=", gid, getgrgid(gid)->gr_name );
 
    getgrouplist(getpwuid(uid)->pw_name, gid, list, &amount);
    }

    for (int i = 0; i < amount; i++) {
        char *name = getgrgid(list[i])->gr_name;
        printf("%d(%s)",list[i], name);
        
        if (i != amount -1) {
            printf(",");
        }
    }
    printf("\n");
    return 0;
}