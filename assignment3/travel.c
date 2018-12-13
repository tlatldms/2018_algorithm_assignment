#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tmpg.h"
#include <sys/time.h>

long current_timestamp()
{
    struct timeval te;
    gettimeofday(&te, NULL);                                         // get current time
    long long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // calculate milliseconds
    // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}

struct Person people[100];
//array of st_nodes
struct st_node st_arr[100];
//transportation of [src][dest] exists:1, else:0
int tp_visit[100][100] = {0};
//tp_arr[src site num][dest site num]
struct tp_node tp_arr[100][100] = {
    0,
};

int main()
{
    srand(time(NULL));
    long start_time = current_timestamp();
    long finish_time = 0;
    struct Graph *graph = createGraph(100);

    int s, d, i;
    //create sites info
    createSites();
    for (i = 0; i < 100; i++)
    {
        do
        {
            d = rand() % 100;
        } while (i == d || tp_visit[i][d] == 1);
        tp_arr[i][d].distance = 10 + rand() % 90;
        tp_arr[i][d].tp_price = 5000 + (rand() % 21) * 1000;
        tp_visit[i][d] = 1;
        printf("<TP of st_%d to st_%d> distance:%d, TP_price:%d\n", i, d, tp_arr[i][d].distance, tp_arr[i][d].tp_price);
        addEdge(graph, i, d);
    }
    for (i = 0; i < 200; i++)
    {
        do
        {
            s = rand() % 100;
            d = rand() % 100;
        } while (s == d || tp_visit[s][d] == 1);
        tp_arr[s][d].distance = 10 + rand() % 90;
        tp_arr[s][d].tp_price = 5000 + (rand() % 21) * 1000;
        tp_visit[s][d] = 1;
        printf("<TP of st_%d to st_%d> distance:%d, TP_price:%d\n", s, d, tp_arr[s][d].distance, tp_arr[s][d].tp_price);
        addEdge(graph, s, d);
    }

    printf("\n< Graph of sites >");
    printGraph(graph);

    printf("\n\n< Hotel RBTree >\n");
    htnode htroot = NULL;
    for (i = 0; i < 100; i++)
    {
        ht_insertNode(&htroot, i);
    }
    ht_inorderPrint(&htroot);

    printf("\n\n< Reservation RBTree >\n");
    rvnode rvroot = NULL;
    /*
	for (i=0; i<100; i++) {
		rv_insertNode(&rvroot, i, 0, 0, 0);
	}
	rv_removeNode(rvroot, 12);
	rv_inorderPrint(&rvroot);
	*/

    int ID, org, dst, budget, start, end;
    int select;

    for (i = 0; i < 100; i++)
        people[i].id = 0;

    while (1)
    {
        printf("\n1. Planning a trip\n");
        printf("2. To check the reservation information\n");
        printf("3. To cancel a reservation\n");
        printf("4. Quit\n");
        printf("\nPlease choose what you want to do: ");
        if (scanf("%d", &select) < 0)
            break;

        if (select == 1)
        {
            printf("Please write down your ID in number(1~99): ");
            scanf("%d", &ID);
            while (people[ID].id != 0)
            {
                printf("%d is being used by someone else.\n", people[ID].id);
                printf("Please write down your ID in number(1~10): ");
                scanf("%d", &ID);
            }
            printf("Where is your departure site? ");
            scanf("%d", &org);
            printf("Where is your final destination? ");
            scanf("%d", &dst);
            printf("What is the budget? ");
            scanf("%d", &budget);
            printf("What date are you going to?(ex 181225) ");
            scanf("%d", &start);
            printf("When will you be back?(ex 190101) ");
            scanf("%d", &end);
            printf("\n");
            people[ID].id = ID;
            people[ID].org = org;
            people[ID].dst = dst;
            people[ID].budget = budget;
            people[ID].start = start;
            people[ID].end = end;

            int num = 0;

            if ((num = dijkstra_distance(graph, org, dst)) != 1)
            {
                printf("The distance from site[%d] to site[%d]: ", org, dst);
                printf("%d\n", num);
            }
            else
            {
                printf("There is no way from site[%d] to site[%d]\n\n", org, dst);
                continue;
            }
            if ((num = dijkstra_cost(graph, org, dst)) != 1)
            {
                printf("The price from site[%d] to site[%d]: ", org, dst);
                printf("%d\n", num);
            }
            htnode a = ht_findNode(&htroot, dst);
            printf("\nThe price of the hotel in site[%d]: %dwon\n", dst, a->ht_price);

            int answer;
            printf("Would you like to reserve a hotel in site[%d]? 1. Yes, 2. No : ", dst);
            scanf("%d", &answer);

            if (answer == 1)
            {
                if (budget < num + a->ht_price)
                {
                    printf("Your budget is short\n");
                    continue;
                }
                rv_insertNode(&rvroot, ID, start, end, a->ht_price);
                printf("Successfully reserved!\n");
            }
        }
        if (select == 2)
        {
            printf("What is your ID? ");
            scanf("%d", &ID);
            printf("\n");
            if (people[ID].id == 0)
            {
                printf("There is no any reserved information\n");
            }
            else
            {
                rvnode a = rv_findNode(&rvroot, ID);

                printf("Reservation information of %d\n", ID);
                printf("Start date of tour: %d\nEnd date of tour: %d\nHotel price: %d\n\n", a->tour_start, a->tour_end, a->price);
            }
        }
        if (select == 3)
        {
            printf("What is your ID? ");
            scanf("%d", &ID);
            if (people[ID].id == 0)
            {
                printf("There is no any reserved information");
            }
            else
            {
                int answer;
                printf("Are you sure you want to cancel your reservation? 1. Yes, 2. No : ");
                scanf("%d", &answer);
                if (answer == 1)
                {
                    rv_removeNode(&rvroot, ID);
                    people[ID].id = 0;
                    printf("Successfully removed\n");
                }
            }
        }
        if (select == 4)
            break;
    }

    finish_time = current_timestamp();

    printf("\ntime elapsed : %ld\n", finish_time - start_time);

    return 0;
}
