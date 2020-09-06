#include "automaton.h"
#include <unordered_map>
#include <string>
#include <cstdio>

int main(int argc, char **argv) {
    std::unordered_map<std::string, std::string> cmdParams;
    for(int i = 1; i < argc; i++) {
		std::string key(argv[i]);
		if(key.size() > 0 && key[0] == '-') {
			if(i != argc - 1) {
				cmdParams[key] = argv[i + 1];
				i++;
			} else
				cmdParams[key] = "";
		}
	}
    int n = 70, m = 70;
    if (cmdParams.find("-size") != cmdParams.end()) {
        std::string size = cmdParams["-size"];
        unsigned xpos = size.find('x');
        if (xpos == std::string::npos) {
            printf("Invalid size syntax.\n");
            return -1;
        }
        n = std::stoi(size.substr(0, xpos));
        m = std::stoi(size.substr(xpos + 1));
    }
    Automaton automaton(argc, argv, 700, 700, m, n);
    std::string spark = automaton.spark;
    if (cmdParams.find("-mode") != cmdParams.end()) {
        automaton.setMode(cmdParams["-mode"]);
    } else {
        printf("Generation mode has not been seleted. Choose default mode 'spark' (%s).\n", spark.c_str());
    }
    double update_time = 0.5;
    if (cmdParams.find("-update") != cmdParams.end()) {
        update_time = std::stod(cmdParams["-update"]);
    }
    automaton.setUpdateTime(update_time);
    automaton.run();

    return 0;
}