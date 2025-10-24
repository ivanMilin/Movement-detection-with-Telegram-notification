#include <wiringPi.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <curl/curl.h>
#include <sys/stat.h>
#include <limits.h>

using namespace std;

// PIN konfiguracija
const int pirPin = 7;  // wiringPi 7 = BCM 4

// IP servera
const string server_url = "XXXXXXXXXXXXXXXXXXXXXXXXXXX";

// Provera da li fajl postoji i da nije prazan
bool file_ready(const string &path) {
    struct stat st;
    return (stat(path.c_str(), &st) == 0 && st.st_size > 0);
}

// Funkcija koja salje fajl preko HTTP POST
bool send_file_http(const string &server_url, const string &file_path) {
    char abs_path[PATH_MAX];
    if (!realpath(file_path.c_str(), abs_path)) {
        cerr << "Ne mogu da dobijem apsolutnu putanju za fajl: " << file_path << endl;
        return false;
    }

    CURL *curl = curl_easy_init();
    if (!curl) return false;

    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mime_name(part, "file");
    curl_mime_filedata(part, abs_path);

    curl_easy_setopt(curl, CURLOPT_URL, server_url.c_str());
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_mime_free(mime);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        return false;
    }
    if (http_code != 200) {
        cerr << "Server returned HTTP code " << http_code << endl;
        return false;
    }

    return true;
}

int main() {
    if (wiringPiSetup() == -1) 
    {
        cerr << "Greska: wiringPi setup nije uspeo!" << endl;
        return 1;
    }

    pinMode(pirPin, INPUT);
    

    bool recording = false;
	int motion= 0;
    string current_file;

    while (true) 
    {
		
		//this_thread::sleep_for(chrono::seconds(10));
		
		cout << "Sistem spreman. Cekam pokret..." << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));			
        
        motion = digitalRead(pirPin);

        if (motion && !recording) 
        {
            recording = true;
            cout << "Pokret detektovan! Pokrecem snimanje..." << endl;

            // Kreiraj ime fajla sa human-readable timestamp-om
            char buffer[100];
            time_t now = time(nullptr);
            tm *ltm = localtime(&now);
            strftime(buffer, sizeof(buffer), "videos/video_%Y-%m-%d-%H-%M-%S.h264", ltm);
            string filename(buffer);

            current_file = filename;

            // Pokreni snimanje u pozadini, sve logove utisaj
            string cmd = "rpicam-vid --timeout 20000 --width 1920 --height 1080 --framerate 30 -o " + filename + " > /dev/null 2>&1";
            system(cmd.c_str());

            cout << "Snimanje zavrseno: " << filename << endl;

            int tries = 0;
            while (tries < 10 && !file_ready(current_file)) 
            {
                this_thread::sleep_for(chrono::milliseconds(300));
                tries++;
            }

            if (file_ready(current_file)) 
            {
                cout << "Saljem fajl na server..." << endl;
                if (send_file_http(server_url, current_file)) 
                {
                    cout << "Fajl uspesno poslat!" << endl;
                    remove(current_file.c_str()); // obrisi fajl sa diska
					cout << "Fajl obrisan sa diska." << endl;
                } 
                else 
                {
                    cerr << "Slanje fajla nije uspelo!" << endl;
                }
            } 
            else 
            {
                cerr << "Fajl nije spreman ili je prazan: " << current_file << endl;
            }

            recording = false;	
	    motion = 0;
            delay(10000);
	}

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}
