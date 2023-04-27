#include <unistd.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

// The maximum number of customer threads.
#define MAX_Customers 20

// Function prototypes...
void *customer (void *num);
void *barber (void *);
void random_wait (int secs);

// waitingRoom Limits the no of allowed customers
// to enter the waiting room at one time.
sem_t waitingRoom;

// barChair ensures mutually exclusive access to
// the barber chair.
sem_t barChair;

// barPillow is used to allow the barber to sleep
// until a customer arrives.
sem_t barPillow;

// seatBelt is used to make the customer to wait until
// the barber is done styling the customer.
sem_t seatBelt;

// Flag to stop the barber thread when all barber
// have been serviced.
int allDone = 0;

int Number[MAX_Customers];

void change_priority() {
    int new_priority_arr[MAX_Customers];
    unordered_set<int> used_priorities;
    
    for(int j=0; j < MAX_Customers; j++) {
        if(used_priorities.count(Number[j]) == 0) {
            new_priority_arr[j]=(Number[j]);
            used_priorities.insert(Number[j]);
        }
        else {
            int k = Number[j] + 1;
            while(used_priorities.count(k) > 0) k++;
            
            new_priority_arr[j]=k;
            used_priorities.insert(k);
        }
    }
   
    for(int j=0; j < MAX_Customers; j++) Number[j] = new_priority_arr[j];
}

int main(int argc, char *argv[]) {
//argc is the number of arguments being passed into our program from the command line and argv is the
//array of arguments.
//pthread_t is the data type used to uniquely identify a thread. It is returned by pthread_create() and used
//by the application in function calls that require a thread identifier.
  pthread_t dtid;
  pthread_t tid[MAX_Customers];
  int i, numCustomers = 0, numChairs = 0;
 
  int option;
e:cout <<
    ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    << endl;
  cout << "' '" << endl;
  cout << "' WELCOME TO '" << endl;
  cout << "' BARBER'S SHOP '" << endl;
  cout << "' '" << endl;
  cout << "' '" << endl;
  cout << "' Press Your Option :- '" << endl;
  cout << "' 1.press for Info '" << endl;
  cout << "' 2.Enter number of Customers '" << endl;
  cout << "' 3.Enter number of Chairs '" << endl;
  cout << "' 4.Enter the priority of the customers: '" << endl;
  cout << "' '" << endl;
  cout << "' 5.Exit '" << endl;
  cout << "' Enter your choice __ '" << endl;
  cout << "' '" << endl;
  cout <<
    ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    << endl;
  cout << "choice : ";
  cin >> option;
  cout << endl;
  
  switch(option) {
    case 1:
      {
        cout << "This project is based on a real life Barber-Customer problem (in a barber shop)";
        cout << " which is classic inter-process communication and synchronization problem between multiple processes/threads.\n\n";
        cout << "This Problem can be solved by using the concept of semaphores or mutex locks.";
        cout << " We are solving this problem using semaphores.\n";
        
        goto e;
      }
      
    case 2:
      {
        cin >> numCustomers;
        for(i = 0; i < numCustomers; i++) Number[i] = i;
        
        goto e;
      }
      
    case 3:
      {
        cin >> numChairs;
        goto e;
      }
      
    case 4:
      {
        for(i = 0; i < numCustomers; i++)cin>>Number[i];
        change_priority();
        
        goto e;
      }
      
    case 5:
      {
        cout << "Terminal closed\n";
        break;
      }
      
    default:
        cout << "Option you entered is currently unvailable!!\nplease Enter option Valid option! : \n";
        goto e;
        break;
    }
    
  // Make sure the number of threads is less than the number of
  // Customers we can support.
  if(numCustomers > MAX_Customers) {
      printf ("\nThe maximum number of Customers is %d.\n", MAX_Customers);
      exit (-1);
    }
    
  // Initialize the semaphores with initial values...
  sem_init (&waitingRoom, 0, numChairs);
  sem_init (&barChair, 0, 1);
  sem_init (&barPillow, 0, 0);
  sem_init (&seatBelt, 0, 0);
  
  // Create the barber.
  pthread_create (&dtid, nullptr, barber, nullptr);
  
  // Create the Customers.
  for(i = 0; i < numCustomers; i++) {
      pthread_create (&tid[i], nullptr, customer, (void *) &Number[i]);
      sleep(1);
    }
    
  // Join each of the threads to wait for them to finish.
  for(i = 0; i < numCustomers; i++) {
      pthread_join(tid[i],nullptr);
      sleep(1);
  }
  
  // When all of the Customers are styled, kill the
  // barber thread.
  allDone = 1;
  sem_post(&barPillow);
  
  // Wake the barber so he will exit.
  pthread_join(dtid,nullptr);
}

void *customer(void *number) {
      int num = *(int *)number;
      
      // Leave for the shop and take some random amount of
      // time to arrive.
      printf("customer %d leaving for barber shop.\n", num);
      random_wait(2);
      printf ("customer %d arrived at barber shop.\n", num);
      
      // Wait for space to open up in the waiting room...
      sem_wait(&waitingRoom);
      printf("customer %d entering waiting room.\n", num);
      
      // Wait for the barber chair to become free.
      sem_wait(&barChair);
      
      // The chair is free so give up your spot in the
      // waiting room.
      sem_post(&waitingRoom);
      
      // Wake up the barber...
      printf("customer %d getting ready for the cut.\n", num);
      sem_post(&barPillow);
      
      // Wait for the barber to finish cutting your hair.
      sem_wait(&seatBelt);
      
      // Give up the chair.
      sem_post(&barChair);
      printf("customer %d leaving barber's shop.\n", num);
      
      return  0;
}
  
void *barber(void *) {
      // While there are still Customers to be serviced...
      // Our barber can tell if there are
      // Customers still on the way to his shop
      while(!allDone) {
          
          // Sleep until someone arrives and wakes you..
          printf("The barber is calling the next customer\n");sem_wait(&barPillow);
          
          //if all work completed
          if(!allDone) {
              // Take a random amount of time to style the customers
              cout<<"The barber is styling the customer\n\n";
              random_wait(2);
              cout<<"The barber has finished styling.\n\n";
              
              // Release the customer when styling is finished...
              sem_post(&seatBelt);
          }
          else {
              cout<<"No more customers left for the day!\n";
              cout<<"The barber is going home for the day.\n";
                 
          }
      }
      
      return 0;
}

void random_wait(int secs) {
    int len; // Generate a random number...
    len = (int) ((1 * secs) + 1);
    sleep(len);
}