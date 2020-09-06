#include "automaton.h"

int main(int argc, char **argv) {
    Automaton automaton(argc, argv, 700, 700, 40, 20);
    automaton.run();
    return 0;
}