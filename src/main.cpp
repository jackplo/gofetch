#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <unistd.h>
#include "climits"
#include <pwd.h>
#include <sys/utsname.h>
#include <iomanip>
#include <boost/format.hpp>
#include "stdlib.h"
#include <sys/sysinfo.h>
#include <cmath>

#define color(text, code) (std::string("\e[") + std::to_string(code) + ";1m" + text + "\e[0m")

std::string getUser() {
    std::string user = "Unknown";
    if (!getlogin()) return user;

    user = getlogin();
    return user;
}

std::string getHostname() {
    char hostname[HOST_NAME_MAX + 1];
    if (!gethostname(hostname, HOST_NAME_MAX + 1)) return hostname;

    gethostname(hostname, HOST_NAME_MAX + 1);

    return hostname;
}

std::string getShell(passwd* pw) {
    std::string shell;
    shell = pw->pw_shell;
    shell.erase(0, shell.rfind("/") + 1);

    return shell;
}

std::string getMemory() {
    struct sysinfo info;
    sysinfo(&info);
    unsigned long totalMemory = std::floor((info.totalram * info.mem_unit)/1024) * 0.001;
    unsigned long currentMemory = std::floor(((info.freeram + info.bufferram) * info.mem_unit)/1024) * 0.001;

    return std::to_string(currentMemory) + "/" + std::to_string(totalMemory);
}

std::string getDistro() {
    std::string distro = "Unknown";
    std::ifstream operating_system;

    operating_system.open("/etc/os-release");
    if (!operating_system.is_open()) return distro;

    while(std::getline(operating_system, distro)) {
        if (distro.find("PRETTY_NAME=")) {
            std::getline(operating_system, distro);
            break;
        }
    }

    operating_system.close();

    distro.erase(distro.find("PRETTY_NAME"), distro.find("=")+2);
    distro.erase(distro.size()-1, distro.size());

    return distro;
}


int main() {
    struct utsname un;
    uname(&un);

    passwd* pw = getpwuid(getuid());

    std::string dist = getDistro();
    std::string user = getUser();
    std::string hostname = un.nodename;
    std::string kernel = un.release;
    std::string shell = getShell(pw);
    std::string memory = getMemory();

    std::cout << "\n";
 
    std::string test = boost::str( boost::format(" user ::%|5|%|20| ") % user % kernel);

    std::string fetch =
                 "       .---.           \n"
                 "      /     \\         " " user   :: " + user + "\n"
                 "      \\.@-@./         " " dist   :: " + dist + "\n"
                 "      /`\\_/`\\         " "󰇄 host   :: " + hostname + "\n"
                 "     //  _  \\\\        " " kernel :: " + kernel + "\n"
                 "    | \\     )|_       " " shell  :: " + shell + "\n"
                 "   /`\\_`>  <_/ \\      " "󰽙 memory :: " + memory + "\n"
                 "   \\__/'---'\\__/     ";

    std::cout << color(fetch, 33) << std::endl;
    std::cout << "\n";

    return 0;
}
