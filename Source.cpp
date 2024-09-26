#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Audio {
private:
    string title;
    string artist;
    int duration; 

public:
    Audio(const string& t, const string& a, int d) : title(t), artist(a), duration(d) {}
    // Display information about the audio recording
    void play() const {
        cout << "In progress: " << title << " - " << artist << " (" << duration << " sec)" << endl;
    }
    // Get the name of the audio recording
    string getTitle() const {
        return title;
    }
    string getArtist() const {
        return artist;
    }
};

class AudioCollection {
private:
    vector<Audio> audios;

public:
    void addAudio(const Audio& audio) {
        audios.push_back(audio);
        cout << "Audio recording added: " << audio.getTitle() << " - " << audio.getArtist() << endl;
    }

    void removeAudio(const string& title) {
        audios.erase(remove_if(audios.begin(), audios.end(),
            [&](const Audio& a) { return a.getTitle() == title; }), audios.end());
        cout << "Audio recording deleted: " << title << endl;
    }

    Audio* findAudio(const string& title) {
        for (auto& audio : audios) {
            if (audio.getTitle() == title) {
                return &audio;
            }
        }
        return nullptr;
    }

    void listAll() const {
        for (const auto& audio : audios) {
            audio.play();
        }
    }
};
// Proxy class for the audio collection
class ProxyAudioCollection {
private:
    AudioCollection* realCollection;

public:
    ProxyAudioCollection() : realCollection(nullptr) {}

    // Creates a collection when it is needed
    AudioCollection* getCollection() {
        if (!realCollection) {
            realCollection = new AudioCollection();
        }
        return realCollection;
    }
    // Adds audio through a proxy
    void addAudio(const Audio& audio) {
        getCollection()->addAudio(audio);
    }

    void removeAudio(const string& title) {
        getCollection()->removeAudio(title);
    }

    Audio* findAudio(const string& title) {
        return getCollection()->findAudio(title);
    }

    void listAll() const {
        if (realCollection) {
            realCollection->listAll();
        }
        else {
            cout << "Collection is empty" << endl;
        }
    }

    ~ProxyAudioCollection() {
        delete realCollection;
    }
};

int main() {
    ProxyAudioCollection proxyCollection;

    proxyCollection.addAudio(Audio("Song 1", "Artist 1", 210));
    proxyCollection.addAudio(Audio("Song 2", "Artist 2", 180));

    cout << "List of all audio recordings:" << endl;
    proxyCollection.listAll();
    // Search for an audio recording
    cout << "Search 'Song 1':" << endl;
    Audio* foundAudio = proxyCollection.findAudio("Song 1");
    if (foundAudio) {
        foundAudio->play();
    }
    else {
        cout << "Audio not found." << endl;
    }

    proxyCollection.removeAudio("Song 1");
    // Display all audio recordings after deletion
    cout << "List after deletion 'Song 1':" << endl;
    proxyCollection.listAll();

    return 0;
}
