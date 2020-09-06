#include "automaton.h"

int main(int argc, char **argv) {
    Automaton automaton(argc, argv, 700, 700, 100, 100);
    automaton.run();
    return 0;
}