#include <wiringPi.h>
#include <iostream>
#include <ctime>
using namespace std;

const int pirPin = 7;  // wiringPi pin 7 (odgovara BCM 4)

int main() {
    if (wiringPiSetup() == -1) {
        cout << "Greska: wiringPi setup nije uspeo!" << endl;
        return 1;
    }

    pinMode(pirPin, INPUT);

    cout << "Detekcija pokreta je u toku." << endl;

    while (true) {
        int motion = digitalRead(pirPin);

        if (motion) {
            time_t now = time(0);
            tm *ltm = localtime(&now);

            cout << "[" 
                 << ltm->tm_hour << ":"
                 << ltm->tm_min << ":"
                 << ltm->tm_sec 
                 << "] Pokret detektovan!" << endl;
        }
	else
	{
		cout<< "===============================" <<endl;
	}
	motion = 0;
        delay(1000);
    }

    return 0;
}

