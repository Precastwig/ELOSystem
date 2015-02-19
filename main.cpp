#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

using namespace std;



class Data {
public:
    Data(){
        name = "";
        rating = 1500;
    }
    string name;
    double rating;

    friend std::ostream& operator<<(std::ostream& o, const Data& p)
    {
    o << p.name << "\n";
    o << p.rating << "\n";
    return o;
    }

    friend std::istream& operator>>(std::istream& o, Data& p)
    {
    o >> p.name;
    o >> p.rating;
    return o;
    }
};

vector<Data> people;

class FileSave
{
public:
    FileSave()
    {
        load();
    }

    ~FileSave()
    {
        save();
    }

    void save()
    {
        ofstream out{"flatsave.elo", std::ios::out | std::ofstream::binary};
        copy( begin(people), end(people),
             ostream_iterator<Data>{out});
    }

    void load()
    {
        ifstream in{"flatsave.elo", std::ios::in | std::ifstream::binary};
        istream_iterator<Data> input(in);
        copy( input, istream_iterator<Data>(), back_inserter(people) );

    }
};

int getindex(string player)
{
    for(int i=0; i<100; i++)
    {
        if(people[i].name == player)
            return i;
    }
    return -1;
}

void match(string player1, string player2, double outcome)
{
    int p1 = getindex(player1);
    int p2 = getindex(player2);
    if(p1 != -1 || p2 != -1)
    {
        double r1 = people[p1].rating;
        double r2 = people[p2].rating;

        double expectedp1 = ( 1 / (
                1 + pow(10, (r2 - r1) / 400)
                              ));
        double expectedp2 = ( 1 / (
                1 + pow(10, (r1 - r2) / 400)
                              ));

        people[p1].rating = r1 + (32 * (outcome - expectedp1));
        people[p2].rating = r2 + (32 * (-outcome - expectedp2));
    }
}

int menuitem = 0;

void menu()
{
    menuitem = 0;
    cout << "Name          ELO" << "\n";
    Data temp = Data();
    vector<Data> ptemp = people;
    int keep = 0;
    for(int i=0; i< people.size(); i++)
    {
        temp.rating = 0.0;
        for(int j=0; j < ptemp.size(); j++)
        {
            if(ptemp[j].rating >= temp.rating)
            {
                temp = ptemp[j];
                keep = j;
            }
        }
        cout << temp.name << "  :  ";
        cout << temp.rating << "\n";
        ptemp.erase(ptemp.begin() + keep);
    }
    cout << "\n";
    cout << "1. Add player" << "\n";
    cout << "2. Record game" << "\n";
    cout << "3. Save and exit" << "\n";
    cin >> menuitem;
}

void addplayer()
{
    Data temp = Data();
    cout << "Player name : ";
    string obj;
    cin >> obj;
    temp.name = obj;
    int choice;
    cout << "Give player a custom starting rating? (y=1/n=0)" << "\n";
    cin >> choice;
    if(choice == 1)
    {
        int rate = 1500;
        cout << "Enter custom starting rating : ";
        cin >> rate;
        if(rate >= 1 && rate <= 2500)
        {
            temp.rating = rate;
        }
    }
    else
    {
        temp.rating = 1500;
    }
    people.push_back(temp);
}

void addgame()
{
    string temp1 = "";
    string temp2 = "";
    double temp3 = 0.0;
    cout << "Enter whites name : ";
    cin >> temp1;
    cout << "Enter blacks name : ";
    cin >> temp2;
    cout << "Enter outcome (-1.0 to 1.0, where positive is in whites favour) : ";
    cin >> temp3;
    match(temp1, temp2, temp3);
    cout << "\n";
}

int main()
{
    FileSave save = FileSave();
    while(menuitem != 3)
    {
        menu();
        if(menuitem == 1) {
            addplayer();
            save.save();
        }
        if(menuitem == 2) {
            addgame();
            save.save();
        }
    }
    return 0;
}
