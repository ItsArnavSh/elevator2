#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
using namespace std;
class passenger;
void clearScreen();
int generateRandomInt(int min, int max);
void plotArt(vector<vector<passenger>> waiting,vector<int> lifts,int maxFloor);
void printPMap(vector<vector<passenger>> pMap);
void mergeSort(vector<int>& vec, int const begin, int const end);void mergeSort(vector<int>& vec, int const begin, int const end);
void merge(vector<int>& vec, int const left, int const mid, int const right);

class Elevator
{
    public:
    int currentFloor;
    int direction;
    vector<int> destinations;
    public:
    Elevator();
};

Elevator::Elevator()
{
    currentFloor = 0;
    direction =0;
}

class firstComeFirstServe:public Elevator
{

};

class SCAN:public Elevator
{
    public:
};

class LOOK:public Elevator
{

};

class ShortestPath:public Elevator
{

};


class passenger
{
    public:
    int currentFloor;
    int destination;
    int travelling;//0 means not travelling, 1 means that he is travelling and -1 means he has reached his destination.
    int direction;
    passenger(int floors);
    void printPassenger();
    friend void plotArt(vector<vector<passenger>> humans,vector<int> lifts,int maxFloor);
};
passenger::passenger(int floors)
{
    floors = floors;
    currentFloor = generateRandomInt(0,floors-1);
    destination = generateRandomInt(0,floors-1);
    travelling = 0;
    if(currentFloor==destination)
    {
        destination = currentFloor/2;
        if(currentFloor==destination)//Edge case if both are 0
            currentFloor = destination+floors-1;
    }
    direction = abs(-currentFloor+destination)/(destination-currentFloor);
}
// Add this method to the passenger class
void passenger:: printPassenger() {
    cout << "Current Floor: " << currentFloor << ", Destination: " << destination << ", Travelling: " << travelling << ", Direction: "<< direction << endl;
}
int generateRandomInt(int min, int max) {
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

class elevatorSystem
{
    public:
    int score;
    protected:
    vector<vector<passenger>> human;
    int liftNumber;
    int floors;
    vector<vector<passenger>> waiting;
    public:
    elevatorSystem (){}
    virtual void run()=0;
};
class firstComeFirstServeSystem:public elevatorSystem
{
    public:
    void run()
    {

    }
    firstComeFirstServeSystem(vector<vector<passenger>> huma,int liftNumbe,int maxFloor)
    {
        human = huma;
        liftNumber = liftNumbe;
        floors = maxFloor;
        waiting.resize(floors);
    }
};
class SCANSystem:public elevatorSystem
{
    vector<SCAN> lifts;
    vector<int> liftCoords;
    public:
    SCANSystem(vector<vector<passenger>> huma,int liftNumbe,int floor)
    {
        human = huma;
        liftNumber = liftNumbe;
        floors = floor;
        for(int i=0;i<liftNumber;i++)
        {
            SCAN ob;
            lifts.push_back(ob);
            liftCoords.push_back(0);
        }
    }
    void run()
    {
        liftCoords.resize(lifts.size());
        //We have the list of lifts
        //We also have the PMap of all the people here
        //So we will sequentially generate data and run the draw function directly from here
        plotArt(waiting,liftCoords,floors);//Initial Plot
        for(int tickNumber = 0;tickNumber<human.size();tickNumber++)
        {
            //Adding the people from pmap to waiting
            for(int i=0;i<human[tickNumber].size();i++)
            {
                passenger currentHuman = human[tickNumber][i];
                waiting[currentHuman.currentFloor].push_back(currentHuman);
            }

            //Calculating what directions the lifts will move wrt the new passengers
            for(int i=0;i<lifts.size();i++)
            {
                //THis is the most basic algorithm
                //This lift will just go to the top then at the bottom and repeat
                if(lifts[i].currentFloor==0)
                {
                    lifts[i].direction = 1;
                }
                else if(lifts[i].currentFloor = floors-1)
                {
                    lifts[i].direction = -1;
                }
            }
            //moving the lifts in the specified direction
            for(int i=0;i<lifts.size();i++)
            {
                lifts[i].currentFloor = lifts[i].currentFloor+lifts[i].direction;
            }

            //picking up passengers
            for(int i=0;i<lifts.size();i++)
            {
                if(waiting[lifts[i].currentFloor].size()!=0)
                {
                    //We will add their destinations in the respective lifts, and remove the people from waiting vector.
                    for(int j=0;j<waiting[lifts[i].currentFloor].size();j++)
                    {
                        lifts[i].destinations.push_back(waiting[lifts[i].currentFloor][j].destination);
                    }
                    waiting[lifts[i].currentFloor].clear();
                    //removing from vector
                }
            }
            
            //Checking if the passengers have reached and adding that to the score
            for(int i=0;i<lifts.size();i++)
            {
               // Get the current floor of the i-th elevator
                int checker = lifts[i].currentFloor;

                // Count the number of times the current floor appears in the destination list of the i-th elevator
                // and add it to the score
                score += count(lifts[i].destinations.begin(),lifts[i].destinations.end(),checker);

                // Remove all occurrences of the current floor from the destination list of the i-th elevator
                lifts[i].destinations.erase(remove(lifts[i].destinations.begin(), lifts[i].destinations.end(), checker), lifts[i].destinations.end());
            }

            //Generating liftCoords
            for(int i=0;i<lifts.size();i++)
            {
                liftCoords[i] = lifts[i].currentFloor;
            }
            //Plot after all the calculations
            plotArt(waiting,liftCoords,floors);
        }
    }
};
class LOOKSystem:public elevatorSystem
{
    public:
    void run()
    {

    }
    LOOKSystem(vector<vector<passenger>> human,int liftNumber)
    {

    }
};
class ShortestPathSystem:public elevatorSystem
{
    public:
    void run()
    {

    }
    ShortestPathSystem(vector<vector<passenger>> human,int liftNumber)
    {

    }
};

class building
{
    protected:
    int floors;
    int noOfElevators;
    int currentTick;
    int currentAlgorithm;

    vector<vector<passenger>> pMap;
    public:
    building(int maxFloors,int Elevators,vector<vector<passenger>> pMp);
    void startSimulation();
};
building::building(int maxFloors,int Elevators,vector<vector<passenger>> pMp)
{
    floors = maxFloors;
    noOfElevators = Elevators;
    pMap = pMp;
    currentTick = 0;
    currentAlgorithm = 0;
}

void building::startSimulation()
{
    SCANSystem A(pMap,noOfElevators,floors);
    A.run();
}
//Other Functions

void sleepForMilliseconds(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

vector<vector<passenger>> pMapGenerator(int secs,int floors,int maxSpawnRate)
{
    int ticks = secs*5;
    vector<vector<passenger>> pMap;
    for(int i=0;i<ticks;i++)
    {
        int spawnRate = generateRandomInt(0,maxSpawnRate);
        vector<passenger> alpha;
        for(int j=0;j<spawnRate;j++)
        {
            passenger temp(floors);
            alpha.push_back(temp);
        }
        pMap.push_back(alpha);
    }
    return pMap;
}

void merge(vector<int>& vec, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temporary vectors
    vector<int> leftArray(subArrayOne), rightArray(subArrayTwo);

    // Copy data to temporary vectors
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = vec[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = vec[mid + 1 + j];

    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;

    // Merge the temporary vectors back into vec
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            vec[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            vec[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }

    // Copy the remaining elements of leftArray, if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        vec[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }

    // Copy the remaining elements of rightArray, if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        vec[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

void mergeSort(vector<int>& vec, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively

    auto mid = begin + (end - begin) / 2;
    mergeSort(vec, begin, mid);
    mergeSort(vec, mid + 1, end);
    merge(vec, begin, mid, end);
}

// Function to print pMap
void printPMap(vector<vector<passenger>> pMap) {
    for (int i = 0; i < pMap.size(); i++) {
        cout << "Tick " << i << ":" << endl;
        for (int j = 0; j < pMap[i].size(); j++) {
            pMap[i][j].printPassenger();
        }
    }
}

void plotArt(vector<vector<passenger>> waiting,vector<int> lifts,int maxFloor)
{
    //We have 3 things
    //waiting, which contains the people that are currently standing
    //lifts, that contains the coordinates of the lifts
    //The max Height

    //Lets draw its ASCII Representation
    clearScreen();
    for(int i=maxFloor-1;i>=0;i--)
    {
        //14 ||   |    |   [|]   |    |  || The passengers
        //13 ||   |    |    |   [|]   |  ||

        //Printing Line by line
        cout << setw(3) << i;
        cout << " || ";
        for(int j = 0;j<lifts.size();j++)
        {
            cout << " ";
            if(i==lifts[j])
            {
                cout << "[|]";
            }
            else
            {
                cout << " | ";
            }
            cout << " ";
        }
        //Now printing all the passengers
        cout << " ";
        for(int j=0;j<waiting[i].size();j++)
        {
            cout << "O";
        }
        cout << endl;
    }
}
void clearScreen() {
    system("clear");
}
int main()
{
    cout << "Welcome to the Elevator Simulation"<<endl;
    sleepForMilliseconds(800);
    cout << "Before We begin, let us set up the starting conditions for our simulation"<<endl;
    int maxFloors,noOfElevators,maxSpawnRate,duration;
    cout << "How many Floors Will the building have? ";
    cin >> maxFloors;
    cout << "How many Elevators will be there? ";
    cin >> noOfElevators;
    cout << "What should be the duration of the algorithm in seconds ";
    cin >> duration;
    cout << "One Tick is 200 ms long, so there are 5 ticks in one second.\nWhat is the max passengers you want to spawn with every tick?";
    cin >> maxSpawnRate;

    vector<vector<passenger>> pMap = pMapGenerator(duration,maxFloors,maxSpawnRate);
    printPMap(pMap);
    building wtc(maxFloors,noOfElevators,pMap);
    wtc.startSimulation();
}
