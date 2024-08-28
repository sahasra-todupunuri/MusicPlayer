#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Node
{
    string name;
    string film;
    string genre;
    string duration;
    Node *next;
    Node *prev;
};

// Function declarations
Node *createPlaylist(const string &fileName, int size);
void printPlaylist(Node *head);
Node *insertSong(Node *head);
Node *removeDuplicates(Node *head);
Node *deleteSong(Node *head, string &songName);
Node *customizedPlaylist(Node *head, vector<pair<string, string>> &attributes);
void navigatePlaylist(Node *head);
void playInLoop(Node *head, int k);
void shuffleAndPlay(Node *head, const string &startSongName);

int main()
{
    Node *playlist = nullptr;
    int choice;
    string fileName;
    int size, k;
    vector<pair<string, string>> attributes;
    string songName, startSongName;

    while (true)
    {
        cout << "Music Player Menu:\n";
        cout << "1. Create Playlist\n";
        cout << "2. Print Playlist\n";
        cout << "3. Insert Song\n";
        cout << "4. Remove Duplicates\n";
        cout << "5. Delete Song\n";
        cout << "6. Customize Playlist\n";
        cout << "7. Navigate Playlist\n";
        cout << "8. Play Playlist in Loop\n";
        cout << "9. Shuffle and Play\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter file name: ";
            cin >> fileName;
            cout << "Enter number of songs in the file: ";
            cin >> size;
            playlist = createPlaylist(fileName, size);
            break;

        case 2:
            printPlaylist(playlist);
            break;

        case 3:
            playlist = insertSong(playlist);
            break;

        case 4:
            playlist = removeDuplicates(playlist);
            break;

        case 5:
            cout << "Enter the name of the song to delete: ";
            cin >> songName;
            playlist = deleteSong(playlist, songName);
            break;

        case 6:
        {
            string attrType, attrValue;
            cout << "Enter number of attributes: ";
            int numAttributes;
            cin >> numAttributes;
            attributes.clear();
            for (int i = 0; i < numAttributes; ++i)
            {
                cout << "Enter attribute type (name/film/genre/duration): ";
                cin >> attrType;
                cout << "Enter attribute value: ";
                cin >> attrValue;
                attributes.push_back(make_pair(attrType, attrValue));
            }
            Node *customPlaylist = customizedPlaylist(playlist, attributes);
            printPlaylist(customPlaylist);
            // Cleanup custom playlist if needed
            break;
        }

        case 7:
            navigatePlaylist(playlist);
            break;

        case 8:
            cout << "Enter the number of times to loop the playlist: ";
            cin >> k;
            playInLoop(playlist, k);
            break;

        case 9:
            cout << "Enter the name of the song to start with: ";
            cin >> startSongName;
            shuffleAndPlay(playlist, startSongName);
            break;

        case 0:
            // Cleanup code to free memory
            Node *temp;
            while (playlist)
            {
                temp = playlist;
                playlist = playlist->next;
                delete temp;
            }
            cout << "Exiting the program." << endl;
            return 0;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Function implementations

Node *createPlaylist(const string &fileName, int size)
{
    Node *head = nullptr;
    Node *current = nullptr;
    ifstream file(fileName);

    if (!file)
    {
        cerr << "Error opening file: " << fileName << endl;
        return nullptr;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty()) // Skip empty lines
            continue;

        Node *newSong = new Node();
        newSong->name = line;

        if (!getline(file, newSong->film) || newSong->film.empty())
            break;
        if (!getline(file, newSong->genre) || newSong->genre.empty())
            break;
        if (!getline(file, newSong->duration) || newSong->duration.empty())
            break;

        newSong->next = nullptr;
        newSong->prev = nullptr;

        if (!head)
        {
            head = newSong;
            current = head;
        }
        else
        {
            newSong->prev = current;
            current->next = newSong;
            current = newSong;
        }
    }

    if (current)
    {
        current->next = nullptr;
    }

    file.close();
    return head;
}

void printPlaylist(Node *head)
{
    Node *curr = head;
    int index = 1;
    while (curr)
    {
        cout << "-----------------------------------------------------------------------------" << endl;
        cout << "Song Number: " << index << endl;
        cout << "Song Name : " << curr->name << endl;
        cout << "Film Name : " << curr->film << endl;
        cout << "Genre : " << curr->genre << endl;
        cout << "Duration : " << curr->duration << endl;
        curr = curr->next;
        index++;
    }
}

Node *insertSong(Node *head)
{
    Node *newSong = new Node();
    cout << "Enter song name: ";
    cin.ignore();
    getline(cin, newSong->name);
    cout << "Enter film name: ";
    getline(cin, newSong->film);
    cout << "Enter genre: ";
    getline(cin, newSong->genre);
    cout << "Enter duration: ";
    getline(cin, newSong->duration);
    newSong->prev = nullptr;
    newSong->next = nullptr;

    if (!head)
    {
        head = newSong;
    }
    else
    {
        Node *tail = head;
        while (tail->next)
        {
            tail = tail->next;
        }
        tail->next = newSong;
        newSong->prev = tail;
    }
    return head;
}

Node *removeDuplicates(Node *head)
{
    Node *curr = head;
    while (curr)
    {
        Node *next = curr->next;
        while (next)
        {
            if (curr->name == next->name)
            {
                if (next->next)
                {
                    next->next->prev = next->prev;
                }
                if (next->prev)
                {
                    next->prev->next = next->next;
                }
                Node *temp = next;
                next = next->next;
                delete temp;
            }
            else
            {
                next = next->next;
            }
        }
        curr = curr->next;
    }
    return head;
}

Node *deleteSong(Node *head, string &songName)
{
    Node *curr = head;
    while (curr)
    {
        if (curr->name == songName)
        {
            if (curr == head)
            {
                head = curr->next;
                if (head)
                {
                    head->prev = nullptr;
                }
            }
            else
            {
                if (curr->next)
                {
                    curr->next->prev = curr->prev;
                }
                if (curr->prev)
                {
                    curr->prev->next = curr->next;
                }
            }
            delete curr;
            break;
        }
        curr = curr->next;
    }
    return head;
}

Node *customizedPlaylist(Node *head, vector<pair<string, string>> &attributes)
{
    Node *newPlaylistHead = nullptr;
    Node *newPlaylistTail = nullptr;
    Node *curr = head;
    while (curr)
    {
        bool match = true;
        for (const auto &attr : attributes)
        {
            const string &attrType = attr.first;
            const string &attrValue = attr.second;
            if ((attrType == "name" && curr->name != attrValue) ||
                (attrType == "film" && curr->film != attrValue) ||
                (attrType == "genre" && curr->genre != attrValue) ||
                (attrType == "duration" && curr->duration != attrValue))
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            Node *newSong = new Node(*curr);
            newSong->prev = nullptr;
            newSong->next = nullptr;
            if (!newPlaylistHead)
            {
                newPlaylistHead = newSong;
                newPlaylistTail = newSong;
            }
            else
            {
                newSong->prev = newPlaylistTail;
                newPlaylistTail->next = newSong;
                newPlaylistTail = newSong;
            }
        }
        curr = curr->next;
    }
    return newPlaylistHead;
}

void navigatePlaylist(Node *head)
{
    printPlaylist(head);

    int serialNumber;
    cout << "Enter serial number of song: ";
    cin >> serialNumber;

    Node *curr = head;
    int counter = 1;
    while (curr && counter < serialNumber)
    {
        curr = curr->next;
        counter++;
    }

    if (!curr)
    {
        cout << "Invalid serial number." << endl;
        return;
    }

    while (curr)
    {
        cout << "-----------------------------------------------------------------------------" << endl;
        if (curr->prev)
        {
            cout << "Previous Song: " << curr->prev->name << endl;
        }
        cout << "Current Song: " << curr->name << endl;
        cout << "Film Name: " << curr->film << endl;
        cout << "Genre: " << curr->genre << endl;
        cout << "Duration: " << curr->duration << endl;
        if (curr->next)
        {
            cout << "Next Song: " << curr->next->name << endl;
        }

        int choice;
        cout << "Enter 0 for previous song, 1 for next song, or -1 to exit: ";
        cin >> choice;

        if (choice == 1)
        {
            if (curr->next)
            {
                curr = curr->next;
            }
            else
            {
                cout << "No next song available." << endl;
            }
        }
        else if (choice == 0)
        {
            if (curr->prev)
            {
                curr = curr->prev;
            }
            else
            {
                cout << "No previous song available." << endl;
            }
        }
        else if (choice == -1)
        {
            break;
        }
        else
        {
            cout << "Invalid option. Try again." << endl;
        }
    }
}

void playInLoop(Node *head, int k)
{
    for (int i = 0; i < k; i++)
    {
        cout << "\nPlaying Playlist - Loop " << (i + 1) << ":\n";
        printPlaylist(head);
        cout << "\nEnd of Loop " << (i + 1) << "\n";
    }
}

void shuffleAndPlay(Node *head, const string &startSongName)
{
    vector<Node *> songs;
    Node *curr = head;
    while (curr)
    {
        songs.push_back(curr);
        curr = curr->next;
    }

    auto it = find_if(songs.begin(), songs.end(), [&](Node *song)
                      { return song->name == startSongName; });

    if (it == songs.end())
    {
        cout << "Start song not found in the playlist." << endl;
        return;
    }

    random_shuffle(songs.begin(), songs.end());
    auto startIt = find(songs.begin(), songs.end(), *it);

    for (auto i = startIt; i != songs.end(); ++i)
    {
        cout << "Playing: " << (*i)->name << endl;
    }
    for (auto i = songs.begin(); i != startIt; ++i)
    {
        cout << "Playing: " << (*i)->name << endl;
    }
}
