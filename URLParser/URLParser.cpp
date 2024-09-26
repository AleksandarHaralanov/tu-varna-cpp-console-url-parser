#include <iostream>
#include <fstream>
#include <string>

using namespace std;

static void printError(const string& errorMessage, size_t errorPos) {
    cout << "\033[1;31mError: " << errorMessage << " at position " << errorPos << "\033[0m" << endl;
}

struct URLComponents {
    string scheme;
    string user;
    string password;
    string host;
    string port;
    string path;
};

static void parseURL(const string& urlString) {
    URLComponents components;
    size_t schemePos = urlString.find("://");
    if (schemePos == string::npos) {
        printError("Invalid URL syntax: Expected '://' not found in " + urlString, urlString.length());
        return;
    }
    components.scheme = urlString.substr(0, schemePos);
    size_t hostStartPos = schemePos + 3;
    size_t hostEndPos = urlString.find('@', hostStartPos);

    if (hostEndPos != string::npos) {
        size_t userEndPos = urlString.find(':', hostStartPos);
        if (userEndPos != string::npos && userEndPos < hostEndPos) {
            components.user = urlString.substr(hostStartPos, userEndPos - hostStartPos);
            components.password = urlString.substr(userEndPos + 1, hostEndPos - userEndPos - 1);
        }
        hostStartPos = hostEndPos + 1;
    }

    size_t portStartPos = urlString.find(':', hostStartPos);
    size_t pathStartPos = urlString.find('/', hostStartPos);

    if (pathStartPos == string::npos) {
        pathStartPos = urlString.length();
    }

    components.host = urlString.substr(hostStartPos, (portStartPos != string::npos ? portStartPos : pathStartPos) - hostStartPos);

    if (components.host.empty()) {
        printError("Host not found", hostStartPos);
        return;
    }

    if (portStartPos != string::npos && portStartPos < pathStartPos) {
        components.port = urlString.substr(portStartPos + 1, pathStartPos - portStartPos - 1);
    }

    if (pathStartPos != string::npos && pathStartPos < urlString.length()) {
        components.path = urlString.substr(pathStartPos);
    }

    cout << "Scheme: " << components.scheme << endl;

    if (!components.user.empty() && !components.password.empty()) {
        cout << "User: " << components.user << endl;
        cout << "Password: " << components.password << endl;
    }

    cout << "Host: " << components.host << endl;
    cout << "Port: " << (components.port.empty() ? "default" : components.port) << endl;
    cout << "Path: " << (components.path.empty() ? "/" : components.path) << endl;
}

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "Unable to open file!" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        parseURL(line);
        cout << "-----------------------------" << endl;
    }

    file.close();
    return 0;
}
