#ifndef HANJP_AUTOMATA
#define HANJP_AUTOMATA

namespace Hanjp
{
    enum AMSIG {
        EAT,
        POP
    };

    class Automata {
    public:

    };
    
    class AutomataDefault : public Automata {

    };

    /*
        To use custom automata, write AutomataCustom class in this file and fill all member functions to automata.cpp
        then, target that is built will contain your automata.
        The below is the template

        class AutomataCustom : public Automata {

        };
    */
}

#endif