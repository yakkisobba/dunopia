// ╔══════════════════════════════════════════════════════════════════╗
// ║                   DUNOPIA — QuizManager.cpp                      ║
// ╚══════════════════════════════════════════════════════════════════╝

#include "QuizManager.hpp"

QuizManager::QuizManager()
{
    bank =
    {
        {   // 0
            "Who authored 'Noli Me Tangere', exposing Spanish abuses (1887)?",
            {"Andres Bonifacio","Marcelo del Pilar","Jose Rizal","Emilio Aguinaldo"},
            2
        },
        {   // 1
            "What does 'Noli Me Tangere' mean in English?",
            {"I Shall Return","Land of Promise","My Last Farewell","Touch Me Not"},
            3
        },
        {   // 2
            "Where was Jose Rizal executed on December 30, 1896?",
            {"Fort Santiago","Corregidor Island","Bagumbayan (Luneta)","Intramuros"},
            2
        },
        {   // 3
            "Rizal's farewell poem, written the night before execution, is?",
            {"Sa Aking mga Kabata","Florante at Laura","El Filibusterismo","Mi Ultimo Adios"},
            3
        },
        {   // 4
            "What is the sequel to Noli Me Tangere, also written by Rizal?",
            {"Ibong Adarna","El Filibusterismo","Florante at Laura","Pasyon at Rebolusyon"},
            1
        },
        {   // 5
            "Who founded the Katipunan (KKK) secret society in 1892?",
            {"Jose Rizal","Emilio Aguinaldo","Andres Bonifacio","Antonio Luna"},
            2
        },
        {   // 6
            "Who defeated Ferdinand Magellan at the Battle of Mactan (1521)?",
            {"Rajah Humabon","Lapu-Lapu","Sultan Kudarat","Rajah Sulayman"},
            1
        },
        {   // 7
            "Who was proclaimed first President of the Philippines (1899)?",
            {"Jose Rizal","Andres Bonifacio","Emilio Aguinaldo","Manuel Quezon"},
            2
        },
        {   // 8
            "In what year did the Philippines declare independence from Spain?",
            {"1896","1897","1898","1900"},
            2
        },
        {   // 9
            "The Philippine-American War broke out in what year?",
            {"1896","1898","1899","1902"},
            2
        },
        {   // 10
            "How many rays does the sun on the Philippine flag have?",
            {"6","7","8","9"},
            2
        },
        {   // 11
            "What is the national language of the Philippines?",
            {"Cebuano","Ilocano","Kapampangan","Filipino (Tagalog)"},
            3
        },
        {   // 12
            "Which word best describes the Filipino tradition of 'Bayanihan'?",
            {"Individual glory","Community cooperation","Wartime bravery","Harvest ritual"},
            1
        },
        {   // 13
            "What year did the Philippines gain full independence from the USA?",
            {"1935","1941","1944","1946"},
            3
        },
        {   // 14
            "How many years did Spain colonize the Philippines (approximately)?",
            {"200","277","333","400"},
            2
        },
    };
}

const Question& QuizManager::get(int idx) const
{
    return bank[idx % static_cast<int>(bank.size())];
}

bool QuizManager::checkAnswer(int qi, int answer) const
{
    return get(qi).correctIndex == answer;
}

int QuizManager::size() const
{
    return static_cast<int>(bank.size());
}
