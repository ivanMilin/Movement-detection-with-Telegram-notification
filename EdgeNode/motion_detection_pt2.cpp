#include <wiringPi.h> 
#include <iostream> 
#include <cstdlib> // za system() 
#include <ctime> 
#include <chrono> 
#include <thread> 
using namespace std; 

const int pirPin = 7; // PIR senzor 
bool recording = false;

int main() {
	if (wiringPiSetup() == -1) {
		cerr << "Greska: wiringPi setup nije uspeo!" << endl;
		return 1;
	}

	pinMode(pirPin, INPUT);

	cout << "Sistem spreman..." << endl;

	while (true) {
		if (digitalRead(pirPin) && !recording) {
			recording = true;
			cout << "Pokret detektovan! Pokrecem snimanje..." << endl;

			// Kreiraj ime fajla sa human-readable timestamp-om
			char buffer[100];
			time_t now = time(nullptr);
			tm *ltm = localtime(&now);
			strftime(buffer, sizeof(buffer), "videos/video_%Y-%m-%d-%H-%M-%S.h264", ltm);
			string filename(buffer);

			// Pokreni snimanje u pozadini sa Full HD i 30 FPS
			string cmd = "rpicam-vid -t 0 --width 1920 --height 1080 --framerate 30 -o " + filename + " > /dev/null 2>&1 &";

			system(cmd.c_str());
		}
		else if (!digitalRead(pirPin) && recording) {
		    
			recording = false;
			cout << "Zaustavljam snimanje..." << endl;

			// Zaustavi rpicam-vid proces
			system("pkill -SIGINT rpicam-vid");
		}

		this_thread::sleep_for(chrono::milliseconds(1000));
	}

	return 0;
}

