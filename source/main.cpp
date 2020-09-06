#include "automaton.h"

int main(int argc, char **argv) {
    Automaton automaton(argc, argv, 700, 700, 70, 70);
    automaton.setUpdateTime(0.2);
    automaton.run();
    return 0;
}