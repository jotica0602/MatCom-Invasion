#include "visuals.h"

void welcome(){
    printf("\t \t \t \tWELCOME SOLDIER! \n \n");
    sleep(2);
    printf("\t THE COMMAND PROMPT INVADERS ARE BACK AND EVERYONE NEEDS A HERO \n \n ");
    sleep(2);
    printf("\t \t DEFEND YOUR OPERATING SYSTEM NO MATTER WHAT\n \n");
    sleep(2);
    printf("\t \t \t \tGOOD LUCK!!!");
    sleep(2);
    printf("\n\n\t\t\t  PRESS ANY KEY TO START");
    getchar();
}

void draw_screen(int *pRows, int *pCols, char ** grid){
    system("clear");
    my_print();
    pthread_mutex_lock(&grid_lock);
    for (int i = 0; i < *pRows; i++){
        for (int j = 0; j < *pCols; j++){
            // g_max = grid[i][j] == 'M' && grid[i] > g_max? 
            printf("%2c", grid[i][j]);
        }
        printf("\n");
    }
    printf(" Score:%-d-------------------------------------------------------------------------------------------------------------------------------------------------Lives:%d\t\t\t\n" , g_score, g_lives);
    pthread_mutex_unlock(&grid_lock);
}

void my_print(){
    const char *r1 = "\t$$\\      $$\\            $$\\      $$$$$$\\                                $$$$$$\\                                        $$\\                     \n";
    const char *r2 = "\t$$$\\    $$$ |           $$ |    $$  __$$\\                               \\_$$  _|                                       \\__|                    \n";
    const char *r3 = "\t$$$$\\  $$$$ | $$$$$$\\ $$$$$$\\   $$ /  \\__| $$$$$$\\  $$$$$$\\$$$$\\          $$ |  $$$$$$$\\ $$\\    $$\\ $$$$$$\\   $$$$$$$\\ $$\\  $$$$$$\\  $$$$$$$\\  \n";
    const char *r4 = "\t$$\\$$\\$$ $$ | \\____$$\\\\_$$  _|  $$ |      $$  __$$\\ $$  _$$  _$$\\         $$ |  $$  __$$\\\\$$\\  $$  |\\____$$\\ $$  _____|$$ |$$  __$$\\ $$  __$$\\ \n";
    const char *r5 = "\t$$ \\$$$  $$ | $$$$$$$ | $$ |    $$ |      $$ /  $$ |$$ / $$ / $$ |        $$ |  $$ |  $$ |\\$$\\$$  / $$$$$$$ |\\$$$$$$\\  $$ |$$ /  $$ |$$ |  $$ |\n";
    const char *r6 = "\t$$ |\\$  /$$ |$$  __$$ | $$ |$$\\ $$ |  $$\\ $$ |  $$ |$$ | $$ | $$ |        $$ |  $$ |  $$ | \\$$$  / $$  __$$ | \\____$$\\ $$ |$$ |  $$ |$$ |  $$ |\n";
    const char *r7 = "\t$$ | \\_/ $$ |\\$$$$$$$ | \\$$$$  |\\$$$$$$  |\\$$$$$$  |$$ | $$ | $$ |      $$$$$$\\ $$ |  $$ |  \\$  /  \\$$$$$$$ |$$$$$$$  |$$ |\\$$$$$$  |$$ |  $$ |\n";
    const char *r8 = "\t\\__|     \\__| \\_______|  \\____/  \\______/  \\______/ \\__| \\__| \\__|      \\______|\\__|  \\__|   \\_/    \\_______|\\_______/ \\__| \\______/ \\__|  \\__|\n";

    printf("%s",r1);
    printf("%s",r2);
    printf("%s",r3);
    printf("%s",r4);
    printf("%s",r5);
    printf("%s",r6);
    printf("%s",r7);
    printf("%s",r8);
    
}


// $$\      $$\            $$\      $$$$$$\                                $$$$$$\                                        $$\                     
// $$$\    $$$ |           $$ |    $$  __$$\                               \_$$  _|                                       \__|                    
// $$$$\  $$$$ | $$$$$$\ $$$$$$\   $$ /  \__| $$$$$$\  $$$$$$\$$$$\          $$ |  $$$$$$$\ $$\    $$\ $$$$$$\   $$$$$$$\ $$\  $$$$$$\  $$$$$$$\  
// $$\$$\$$ $$ | \____$$\\_$$  _|  $$ |      $$  __$$\ $$  _$$  _$$\         $$ |  $$  __$$\\$$\  $$  |\____$$\ $$  _____|$$ |$$  __$$\ $$  __$$\ 
// $$ \$$$  $$ | $$$$$$$ | $$ |    $$ |      $$ /  $$ |$$ / $$ / $$ |        $$ |  $$ |  $$ |\$$\$$  / $$$$$$$ |\$$$$$$\  $$ |$$ /  $$ |$$ |  $$ |
// $$ |\$  /$$ |$$  __$$ | $$ |$$\ $$ |  $$\ $$ |  $$ |$$ | $$ | $$ |        $$ |  $$ |  $$ | \$$$  / $$  __$$ | \____$$\ $$ |$$ |  $$ |$$ |  $$ |
// $$ | \_/ $$ |\$$$$$$$ | \$$$$  |\$$$$$$  |\$$$$$$  |$$ | $$ | $$ |      $$$$$$\ $$ |  $$ |  \$  /  \$$$$$$$ |$$$$$$$  |$$ |\$$$$$$  |$$ |  $$ |
// \__|     \__| \_______|  \____/  \______/  \______/ \__| \__| \__|      \______|\__|  \__|   \_/    \_______|\_______/ \__| \______/ \__|  \__|

void my_print2(){
    const char *r1 = "\t\t\t\t$$$$$$\\                                           $$$$$$\\                                 \n";
    const char *r2 = "\t\t\t\t$$  __$$\\                                         $$  __$$\\                                \n";
    const char *r3 = "\t\t\t\t$$ /  \\__| $$$$$$\\  $$$$$$\\$$$$\\   $$$$$$\\        $$ /  $$ |$$\\    $$\\  $$$$$$\\   $$$$$$\\  \n";
    const char *r4 = "\t\t\t\t$$ |$$$$\\  \\____$$\\ $$  _$$  _$$\\ $$  __$$\\       $$ |  $$ |\\$$\\  $$  |$$  __$$\\ $$  __$$\\ \n";
    const char *r5 = "\t\t\t\t$$ |\\_$$ | $$$$$$$ |$$ / $$ / $$ |$$$$$$$$ |      $$ |  $$ | \\$$\\$$  / $$$$$$$$ |$$ |  \\__|\n";
    const char *r6 = "\t\t\t\t$$ |  $$ |$$  __$$ |$$ | $$ | $$ |$$   ____|      $$ |  $$ |  \\$$$  /  $$   ____|$$ |      \n";
    const char *r7 = "\t\t\t\t\\$$$$$$  |\\$$$$$$$ |$$ | $$ | $$ |\\$$$$$$$\\        $$$$$$  |   \\$  /   \\$$$$$$$\\ $$ |      \n";
    const char *r8 = "\t\t\t\t\\______/  \\_______|\\__| \\__| \\__| \\_______|       \\______/     \\_/     \\_______|\\__|      \n";

    printf("%s",r1);
    printf("%s",r2);
    printf("%s",r3);
    printf("%s",r4);
    printf("%s",r5);
    printf("%s",r6);
    printf("%s",r7);
    printf("%s",r8);
}

// $$$$$$\                                           $$$$$$\                                 
// $$  __$$\                                         $$  __$$\                                
// $$ /  \__| $$$$$$\  $$$$$$\$$$$\   $$$$$$\        $$ /  $$ |$$\    $$\  $$$$$$\   $$$$$$\  
// $$ |$$$$\  \____$$\ $$  _$$  _$$\ $$  __$$\       $$ |  $$ |\$$\  $$  |$$  __$$\ $$  __$$\ 
// $$ |\_$$ | $$$$$$$ |$$ / $$ / $$ |$$$$$$$$ |      $$ |  $$ | \$$\$$  / $$$$$$$$ |$$ |  \__|
// $$ |  $$ |$$  __$$ |$$ | $$ | $$ |$$   ____|      $$ |  $$ |  \$$$  /  $$   ____|$$ |      
// \$$$$$$  |\$$$$$$$ |$$ | $$ | $$ |\$$$$$$$\        $$$$$$  |   \$  /   \$$$$$$$\ $$ |      
//  \______/  \_______|\__| \__| \__| \_______|       \______/     \_/     \_______|\__|      



void print_ascii_art() {
    printf("\t\t$$\\                                      $$\\        $$$$$$\\                                    $$\\             $$\\                     $$\\ \n");
    printf("\t\t$$ |                                     $$ |      $$  __$$\\                                   $$ |            $$ |                    $$ |\n");
    printf("\t\t$$ |       $$$$$$\\  $$\\    $$\\  $$$$$$\\  $$ |      $$ /  \\__| $$$$$$\\  $$$$$$\\$$$$\\   $$$$$$\\  $$ | $$$$$$\\  $$$$$$\\    $$$$$$\\   $$$$$$$ |\n");
    printf("\t\t$$ |      $$  __$$\\ \\$$\\  $$  |$$  __$$\\ $$ |      $$ |      $$  __$$\\ $$  _$$  _$$\\ $$  __$$\\ $$ |$$  __$$\\ \\_$$  _|  $$  __$$\\ $$  __$$ |\n");
    printf("\t\t$$ |      $$$$$$$$ | \\$$\\$$  / $$$$$$$$ |$$ |      $$ |      $$ /  $$ |$$ / $$ / $$ |$$ /  $$ |$$ |$$$$$$$$ |  $$ |    $$$$$$$$ |$$ /  $$ |\n");
    printf("\t\t$$ |      $$   ____|  \\$$$  /  $$   ____|$$ |      $$ |  $$\\ $$ |  $$ |$$ | $$ | $$ |$$ |  $$ |$$ |$$   ____|  $$ |$$\\ $$   ____|$$ |  $$ |\n");
    printf("\t\t$$$$$$$$\\ \\$$$$$$$\\    \\$  /   \\$$$$$$$\\ $$ |      \\$$$$$$  |\\$$$$$$  |$$ | $$ | $$ |$$$$$$$  |$$ |\\$$$$$$$\\   \\$$$$  |\\$$$$$$$\\ \\$$$$$$$ |\n");
    printf("\t\t\\________| \\_______|    \\_/     \\_______|\\__|       \\______/  \\______/ \\__| \\__| \\__|$$  ____/ \\__| \\_______|   \\____/  \\_______| \\_______|\n");
    printf("\t\t                                                                                     $$ |                                                  \n");
    printf("\t\t                                                                                     $$ |                                                  \n");
    printf("\t\t                                                                                     \\__|                                                  \n");
}

// $$\                                      $$\        $$$$$$\                                    $$\             $$\                     $$\ 
// $$ |                                     $$ |      $$  __$$\                                   $$ |            $$ |                    $$ |
// $$ |       $$$$$$\  $$\    $$\  $$$$$$\  $$ |      $$ /  \__| $$$$$$\  $$$$$$\$$$$\   $$$$$$\  $$ | $$$$$$\  $$$$$$\    $$$$$$\   $$$$$$$ |
// $$ |      $$  __$$\ \$$\  $$  |$$  __$$\ $$ |      $$ |      $$  __$$\ $$  _$$  _$$\ $$  __$$\ $$ |$$  __$$\ \_$$  _|  $$  __$$\ $$  __$$ |
// $$ |      $$$$$$$$ | \$$\$$  / $$$$$$$$ |$$ |      $$ |      $$ /  $$ |$$ / $$ / $$ |$$ /  $$ |$$ |$$$$$$$$ |  $$ |    $$$$$$$$ |$$ /  $$ |
// $$ |      $$   ____|  \$$$  /  $$   ____|$$ |      $$ |  $$\ $$ |  $$ |$$ | $$ | $$ |$$ |  $$ |$$ |$$   ____|  $$ |$$\ $$   ____|$$ |  $$ |
// $$$$$$$$\ \$$$$$$$\    \$  /   \$$$$$$$\ $$ |      \$$$$$$  |\$$$$$$  |$$ | $$ | $$ |$$$$$$$  |$$ |\$$$$$$$\   \$$$$  |\$$$$$$$\ \$$$$$$$ |
// \________| \_______|    \_/     \_______|\__|       \______/  \______/ \__| \__| \__|$$  ____/ \__| \_______|   \____/  \_______| \_______|
//                                                                                      $$ |                                                  
//                                                                                      $$ |                                                  
//                                                                                      \__|                                                  