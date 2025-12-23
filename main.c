#include <stdio.h> // includes standard input output library
#include <time.h> // includes time functions
#include <stdlib.h> // includes srand(), rand(), etc functions
#include <string.h> // includes functions related to strings
#include <ctype.h> // includes functions like toupper(), etc.

# ifdef _WIN32
    #include <conio.h> // includes getch() to read input directly without pressing enter key
    #include <windows.h> // includes functions like sleep()
# else
    #include <termios.h> // it doesn't has getch() pre-defined thus has to define it manually
    #include <unistd.h> // includes functions like usleep()

    // the following code snippet to define getch() for linux os is being taken from ChatGPT
    int getch() 
    {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
# endif
/*
Here, # ifdef _WIN32 is used instead of if(_WIN32) because _WIN32 is a macro and is defined before compilation of the program,
not during the runtime. And it includes preprocessor derective which are implemented before compilation. Thus, # ifdef is used.
*/


#define RESET "\033[0m" // to reset to normal
#define BOLD "\033[1m" // for visibility in displaying word
#define ITALICS "\033[3m" // for formatting story
#define REV_COL "\033[7m" // for selection animation
#define CYAN "\033[36m" // welcome title for eye catching
#define G_BROWN "\033[38;5;136m" // welcome title for eye cathching
#define GREEN "\033[32m" // for positive feedback
#define RED  "\033[31m" // for warning messages
#define YELLOW "\033[33m" // for guessed letter to put emphasis
#define MAGENTA "\033[35m" // for urgency on lives left
#define BLUE "\033[34m" // for clarity on input prompt

#define LINE_UP "\033[1A" // moves cursor one line up
#define CLEAR_LINE "\r\033[K" // \r moves the cursor to the beginning of the line, rest clear the lne from cursor to the end of line

#define MAX_INCORRECTS 6


typedef struct word_Bank
{
    char word[11];
    char hint[65];
    int length;
} word_Bank;

word_Bank easy_words [25] = 
{
    {"APPLE", "A fruit that keeps doctors away", 5},
    {"CAT", "A pet that purrs and chases mice", 3},
    {"GHOST", "You can't see it... but it sees you", 5},
    {"BOOK", "A silent storyteller", 4},
    {"LIGHT", "The thing darkness fears", 5},
    {"FIRE", "Warm, useful,...or destructive", 4},
    {"CLOCK", "Ticks towards your fate", 5},
    {"MOON", "Lights up the night", 4},
    {"CHAIR", "It's waiting for someone", 5},
    {"HAT", "A wizard or a farmer might wear", 3},
    {"DREAM", "Sometimes lovely, sometimes haunting", 5},
    {"BIRD", "Flies free, sings loud", 4},
    {"FISH", "Has fins, and breathes where you can't", 4},
    {"PEN", "Mightier than a sword", 3},
    {"SMILE", "A twisted curve… even shadows wear it before the strike", 5},
    {"BALL", "Round shaped toy which helps in many games in different size", 4},
    {"BELL", "Sometimes it sounds spiritual, but sometimes eerie", 4},
    {"COIN", "Used to pay small amount", 4},
    {"FROG", "Jumps and croaks in the rain", 4},
    {"DOOR", "Opens into a new room", 4},
    {"CHALK", "Draws on the blackboard", 5},
    {"ROPE", "A wrong guess will tighten it", 4},
    {"CLOUD", "Fluffy and white, but becomes dark before pouring", 5},
    {"BROOM", "Used to clean, or maybe fly...", 5},
    {"DRUM", "Makes a beat when you tap", 4}
};

word_Bank medium_words [25] = 
{
    {"ANCHOR", "Keeps the ship steady… or drags it down", 6},
    {"STATUE", "Stone still, yet hauntingly lifelike", 6},
    {"TEMPLE", "A sacred place, or a cursed one", 6},
    {"CASKET", "Where the last story sleeps", 6},
    {"MARROW", "Life's core—deep inside the bone", 6},
    {"SCREAM", "Some echoes never stop", 6},
    {"DAGGER", "Trust fails when this slips in", 6},
    {"FOREST", "Alive with whispers, but none human", 6},
    {"DROUGHT", "When even the sky forgets to cry", 7},
    {"LANTERN", "A gentle glow… guiding or deceiving", 7},
    {"HOSPITAL", "Where hope and fear sleep side by side", 8},
    {"FAILURE", "A lesson dressed in shame", 7},
    {"MISTAKE", "A teacher that you never invited", 7},
    {"JUSTICE", "Not always blind, just selective", 7},
    {"FUNERAL", "Where silence speaks the loudest", 7},
    {"ADDICTION", "A chain you almost asked for", 9},
    {"SILENCE", "Sometimes louder than screams", 7},
    {"ASHES", "What's left when the fire forgets", 5},
    {"HUNGER", "When even air tastes of longing", 6},
    {"SUNRISE", "Proof that darkness doesn't last", 7},
    {"JOURNEY", "Every step matters, not just the destination", 7},
    {"HARMONY", "When pieces play the same tune", 7},
    {"NURTURE", "Growth made gentle", 7},
    {"RESILIENCE", "Bending without breaking", 10},
    {"BLESSING", "Unexpected grace", 8}
};

word_Bank hard_words [25] = 
{
    {"POSSESSION", "The body moves. The soul doesn't", 10},
    {"MORGUE", "They lie still, but the room isn't quiet", 6},
    {"CEMETERY", "Stones tell stories... most better left unread", 8},
    {"EXORCISM", "What you hope works before it's too late", 8},
    {"GALLOWS", "Still creaking, though no one swings", 7},
    {"ELEVATOR", "You pressed floor 2. It stopped at 13", 8},
    {"CARPET", "The stain came back again", 6},
    {"JEALOUSY", "Green, yes. But it leaves black bruises", 8},
    {"BLAME", "Easy to give. Hard to return", 5},
    {"PRESSURE", "The weight you carry for dreams that aren't yours", 8},
    {"GRATITUDE", "The light in remembering what remains", 9},
    {"COMPASSION", "Soft hands on another's sharp edges", 10},
    {"LOVE", "The most dangerous and beautiful spell", 4},
    {"STREET", "Where lives cross, but rarely collide", 6},
    {"NOTEBOOK", "Scribbles become memories here", 8},
    {"CURTAIN", "Reveals by being removed", 7},
    {"BRIDGE", "What stands between two choices", 6},
    {"MALIGNANT", "Not just evil — it spreads", 9},
    {"HALLUCINATE", "What you see when reality slips", 11},
    {"CORRUPTION", "It spreads from the inside out", 10},
    {"NIGHTMARE", "More real than dreams should be", 9},
    {"HOWL", "Heard under full moons… not human", 4},
    {"DEMON", "Born of fire, raised in shadow", 5},
    {"TRAIN", "Comes, goes, sometimes doesn't stop for you", 5},
    {"TABLE", "A stage for meals, talks, silence", 5}
};

const char* win_Msg[] = 
{
    "The shadows retreat... You've bested the curse — for now.",
    "Victory tastes like ash in a world still cursed.",
    "You've broken the noose... but the whispers persist.",
    "The demon sleeps... but only because you made it bleed.",
    "You won... but at what cost?"
};

const char* lost_Msg[] = 
{
    "The ritual failed. Your soul now feeds the darkness.",
    "Silence falls. Your fate is sealed in the demon's ledger.",
    "The hangman grins. Another name, another whisper.",
    "You swung... and so did the rope.",
    "You couldn't outrun the shadows. They feast tonight."
};



void display_Welcome_Scrn(); // displays welcome screen
void clear_Console(); // clears the console
int choice_Selection_using_Arrows_for_Main_Menu(int*); // used to select choices using arrows for main menu
void display_Main_Menu_V1(int); // displays main menu at the start of the game
void display_Instructions(); // display the instructions to play the game
void pause_For(float); // pauses the program for the specified second(s) using sleep() or usleep() functions as per the os
void display_Credits(); // displays credits of the game
void display_Exit_Msg(); // displays exit message to the user in a story format
void display_Game_Difficulty_Menu(int); // displays the game difficulty choice in case of VS Comp
int choice_Selection_using_Arrows_for_Game_Difficulty_Menu(int*); // used to select choices using arrows for game difficulty menu
void display_Main_Menu_V2(int); // displays main menu while in the loop
void play_Game(int, char[]); // contains the logic of the gameplay (VS Comp)
int is_Guessed_Letter_Present(char[], char); // checks if guessed letter is present or not
void draw_Hangman(int); // draws the hangman progessively
void show_Demon_ASCII(); // shows demon ascii art
void update_Scoreboard_VS_Comp(int, char[], int); // logic to update scoreboard
void display_Scoreboard(); // displays scoreboard.txt file in the console



void clear_Console()
{
    /*
    _WIN32 is a default definition defined by compilers in Windows OS.
    Thus, the function checks if _WIN32 is defined or not, if it is defined then the program is running in Windows and windows specific
    statement is used. And if not, then the program is running on Linux or MacOS.
    */

    # ifdef _WIN32
        system("cls");
    # else
        system("clear");
    # endif
}

void display_Welcome_Scrn()
{
    clear_Console();

    printf(BOLD RED "╔═════════════════════════════════════════════════════════════════╗" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "               🕯️  WELCOME TO THE " BOLD RED "CURSED HANGMAN" RESET "  🕯️               " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "             " ITALICS "\"" BOLD CYAN "One" RESET ITALICS " word. " BOLD CYAN "One" RESET ITALICS " soul. " BOLD CYAN "One" RESET ITALICS " fatal mistake.\"" RESET "            " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "        " ITALICS CYAN "The mist thickens. A rope swings in the distance." RESET "        " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "     " ITALICS CYAN "Can you uncover the forbidden word, or join the doomed?" RESET "     " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║═════════════════════════════════════════════════════════════════║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "        " BOLD GREEN "Correct guess" RESET " -> " ITALICS "will guide you through the tale." RESET "        " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "   " BOLD RED "Incorrect guess" RESET " -> " ITALICS "tightens the noose... and spills blood." RESET "    " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                   " ITALICS BLUE "Press any key to continue..." RESET "                  " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "╚═════════════════════════════════════════════════════════════════╝" RESET "\n");
    getch();
}

void display_Main_Menu_V1(int selected)
{
    clear_Console();
    printf(BOLD RED "╔═════════════════════════════════════════════════════════════════╗" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                         ☠️  " BOLD RED "MAIN MENU" RESET " ☠️                          " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    
    for (int i = 1; i <= 5; i++)
    {
        if (i == selected)
        {
            if (i == 1) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[1] Begin the Ritual (Start Game)" RESET "                   " BOLD RED "║" RESET "\n");
            else if (i == 2) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[2] Read the Curse (Instructions)" RESET "                   " BOLD RED "║" RESET "\n");
            else if (i == 3) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[3] Blood Stained records (High Scores)" RESET "             " BOLD RED "║" RESET "\n");
            else if (i == 4) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[4] Summoner's Journal (Credits)" RESET "                    " BOLD RED "║" RESET "\n");
            else printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[5] Abandon Hope (Exit)" RESET "                             " BOLD RED "║" RESET "\n");
        }
        else
        {
            if (i == 1) printf(BOLD RED "║" RESET "             [1] Begin the Ritual (Start Game)                   " BOLD RED "║" RESET "\n");
            else if (i == 2) printf(BOLD RED "║" RESET "             [2] Read the Curse (Instructions)                   " BOLD RED "║" RESET "\n");
            else if (i == 3) printf(BOLD RED "║" RESET "             [3] Blood Stained records (High Scores)             " BOLD RED "║" RESET "\n");
            else if (i == 4) printf(BOLD RED "║" RESET "             [4] Summoner's Journal (Credits)                    " BOLD RED "║" RESET "\n");
            else printf(BOLD RED "║" RESET "             [5] Abandon Hope (Exit)                             " BOLD RED "║" RESET "\n");
        }
    }

    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "            🩸 " ITALICS RED "Choose your fate... or be chosen..." RESET " 🩸            " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "      🩸 " ITALICS RED "Use " BOLD BLUE "↑" RESET ITALICS RED " and " BOLD BLUE "↓" RESET ITALICS RED " arrow keys. Press " BOLD BLUE "Enter" RESET ITALICS RED " to select." RESET " 🩸       " BOLD RED "║" RESET "\n");
    printf(BOLD RED "╚═════════════════════════════════════════════════════════════════╝" RESET "\n\n");
}

int choice_Selection_using_Arrows_for_Main_Menu(int* main_menu_Choice)
{
    int ch = getch(); // variable is used istead of directly calling the functions because not to skip any muti-byte character value

    #ifdef _WIN32
        if (ch == 0 || ch == 224)
        {
            ch = getch();

            // for up arrow
            if (ch == 72) *main_menu_Choice = ((*main_menu_Choice + 3) % 5) + 1; // 5 is the menu size or number of menu options

            // for down arrow
            else if (ch == 80) *main_menu_Choice = ((*main_menu_Choice + 5) % 5) + 1; // here also, 5 is the number of menu options
        }

        else if (ch == 13)
        {
            return 0; // when enter key is pressed the function will return 0
        }

        return -1;

    #else
        // Arrow keys are multi-bytes 27,  91, and 65 for up and 66 for down and for Enter key in Windows it is 13 and in Linux is 10
        if (ch == 27) 
        {
            if (getch() == 91) 
            {
                ch = getch();

                // for up arrow
                if (ch == 65) *main_menu_Choice = ((*main_menu_Choice + 3) % 5) + 1; // 5 is the menu size or number of menu options

                // for down arrow
                else if (ch == 66) *main_menu_Choice = ((*main_menu_Choice + 5) % 5) + 1; // here also, 5 is the number of menu options
            }
        }

        else if (ch == 10)
        {
            return 0; // when enter key is pressed the function will return 0
        }

        return -1;

    #endif
}

// Minimum value that can be passed: Windows -> 0.001 and Linux or MacOS -> 0.000001
void pause_For(float seconds)
{
#ifdef _WIN32
    Sleep((DWORD)(seconds * 1000));   // milliseconds
#else
    usleep((useconds_t)(seconds * 1000000)); // microseconds
#endif
}

void display_Instructions()
{
    clear_Console();

    printf(BOLD RED "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf("                       ☠️  " BOLD RED "THE CURSE BE UPON YOU" RESET " ☠️                        \n");
    printf(BOLD RED "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
    
    printf(ITALICS G_BROWN "Your fate is tied to a word — hidden, cursed, and forbidden." RESET "\n");
    printf(ITALICS G_BROWN "Reveal it, one letter at a time... or suffer the noose." RESET "\n\n");

    printf("🩸 " ITALICS CYAN "You will be shown the " BOLD "number of letters" RESET ITALICS CYAN " in the secret word." RESET "\n");
    printf("🩸 " ITALICS CYAN "Guess " BOLD "one letter" RESET ITALICS CYAN " per turn." RESET "\n");
    printf("🩸 " ITALICS CYAN "Correct guess will reveal parts of the word." RESET "\n");
    printf("🩸 " ITALICS CYAN "Each wrong guess fuels the curse — the hangman draws closer to death." RESET "\n");
    printf("🩸 " ITALICS CYAN "If the " BOLD "full body is drawn" RESET ITALICS CYAN " before the word is revealed, the soul is lost." RESET "\n");
    printf("🩸 " ITALICS CYAN "Beware — you will " BOLD "only" RESET ITALICS CYAN " get " BOLD "6 wrong chances." RESET "\n");
    printf("🩸 " ITALICS CYAN BOLD "Symbols" RESET ITALICS CYAN ", " BOLD "numbers" RESET ITALICS CYAN ", and " BOLD "multiple-letter" RESET ITALICS CYAN " guesses are " BOLD "rejected" RESET ITALICS CYAN " by the curse." RESET "\n\n");

    printf("⚠️  " RED "Choose wisely. " BOLD "Each mistake" RESET RED " draws " BOLD "more blood" RESET RED " and " BOLD "a chance will be deducted." RESET "\n\n");
    printf(ITALICS BLUE "Press any key to revert back to the Main Menu..." RESET "\n\n");
    getch();
}

void display_Credits()
{
    clear_Console();

    printf(BOLD G_BROWN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf("                   📖 " BOLD G_BROWN"SUMMONER'S JOURNAL — CREDITS" RESET " 📖                   \n");
    printf(BOLD G_BROWN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");

    printf("🙋 " BOLD CYAN "Coder:" RESET "\n");
    printf("   " BOLD "Debarpan Roy" RESET " (a.k.a " ITALICS "DRoy-007" RESET " or " ITALICS "Code Surgeon" RESET ")\n\n");
    
    printf("🎨 " BOLD CYAN "Story & Lore:" RESET "\n");
    printf("   " ITALICS "Inspired by ancient curses, haunted forests, and a love for the gothic." RESET "\n");
    printf("   " ITALICS "Storyline is generated by ChatGPT." RESET "\n\n");

    printf("💻 " BOLD CYAN "Programming & Spell-Coding:" RESET "\n");
    printf("   " ITALICS "Written in " BOLD "C" RESET ITALICS " using raw terminal magic, emoji glyphs, and blood rituals." RESET "\n\n");

    printf("🎭 " BOLD CYAN "Special Thanks:" RESET "\n");
    printf("   🦴 " ITALICS "Bugs (for keeping the thrill alive)" RESET "\n");
    printf("   🐍 " ITALICS "Terminal escape sequences (for that cursed animation touch)" RESET "\n");
    printf("   🧍‍♀️ 🧍‍♂️  " ITALICS "Parents for supporting me (for the knowledge and things to code)" RESET "\n\n");

    printf("🩸 " CYAN BOLD "This game was forged in the shadows — to " ITALICS "learn" RESET CYAN BOLD ", to " ITALICS "entertain" RESET CYAN BOLD ", and to\n");
    printf("    " ITALICS "whisper dark tales through code..." RESET "\n\n");

    printf(ITALICS BLUE "Press any key to revert back to the Main Menu..." RESET "\n\n");
    getch();
}

void display_Exit_Msg()
{
    clear_Console();

    printf(BOLD RED "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf("                    💀 " BOLD RED "ABANDON HOPE" RESET " 💀\n");
    printf(BOLD RED "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");

    printf(ITALICS YELLOW "The candle flickers... The mist fades..." RESET "\n\n");

    printf(ITALICS YELLOW "You turn your back on the gallows, the whispers silenced..." RESET "\n");
    printf(ITALICS YELLOW "But remember - curse is never truly lifted." RESET "\n\n");

    printf(ITALICS MAGENTA "Next time... it might not just be a game." RESET "\n\n");

    printf( BOLD ITALICS MAGENTA "🩸 Farewell, Mortal... 🩸" RESET "\n\n");
}

void display_Game_Difficulty_Menu(int selected)
{
    clear_Console();

    int i = 1;

    printf( BOLD RED "╔═════════════════════════════════════════════════════════════════╗" RESET "\n");
    printf( BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                     ☠️   " BOLD RED "GAME DIFFICULTY" RESET " ☠️                       " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");

    if (i == selected) printf( BOLD RED "║" RESET "             " BOLD BLUE "> " REV_COL "[1] 🟢 EASY — \"The Cursed Runes\"" RESET "                  " BOLD RED "║" RESET "\n");
    else printf( BOLD RED "║" RESET "               [1] 🟢 EASY — \"The Cursed Runes\"                  " BOLD RED "║" RESET "\n");
    i++;

    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Simpler words" RESET "                                 " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Hints are clearer" RESET "                             " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Summoner\'s candle burns bright..." RESET " 🔥          " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");

    if (i == selected) printf( BOLD RED "║" RESET "             " BOLD BLUE "> " REV_COL "[2] 🟡 MEDIUM — \"The Forbidden Glyphs\"" RESET "            " BOLD RED "║" RESET "\n");
    else printf( BOLD RED "║" RESET "               [2] 🟡 MEDIUM — \"The Forbidden Glyphs\"            " BOLD RED "║" RESET "\n");
    i++;
    
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Medium words" RESET "                                  " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Less forgiving" RESET "                                " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Shadows start to whisper..." RESET " 🕯️ 👁️               " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");

    if (i == selected) printf( BOLD RED "║" RESET "             " BOLD BLUE "> " REV_COL "[3] 🔴 HARD — \"The Ancient Name\"" RESET "                  " BOLD RED "║" RESET "\n");
    else printf( BOLD RED "║" RESET "               [3] 🔴 HARD — \"The Ancient Name\"                  " BOLD RED "║" RESET "\n");
    
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Hard words (sometimes obscure words)" RESET "          " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " Quickend curse buildup" RESET "                        " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                 " RED "-" RESET ITALICS G_BROWN " The Hangman watches every move..." RESET " ☠️           " BOLD RED "║" RESET "\n");
    printf( BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf( BOLD RED "╚═════════════════════════════════════════════════════════════════╝" RESET "\n\n");
}

int choice_Selection_using_Arrows_for_Game_Difficulty_Menu(int* game_difficulty_Choice)
{
    int ch = getch();

    #ifdef _WIN32
        if (ch == 0 || ch == 224)
        {
            ch = getch();

            // for up arrow
            if (ch == 72) *game_difficulty_Choice = ((*game_difficulty_Choice + 1) % 3) + 1;

            // for down arrow
            else if (ch == 80) *game_difficulty_Choice = (*game_difficulty_Choice % 3) + 1;
        }

        else if (ch == 13)
        {
            return 0; // when enter key is pressed the function will return 0
        }

        return -1;

    #else
        // Arrow keys are multi-bytes 27,  91, and 65 for up and 66 for down and for Enter key in Windows it is 13 and in Linux is 10
        if (ch == 27) 
        {
            if (getch() == 91) 
            {
                ch = getch();

                // for up arrow
                if (ch == 65) *game_difficulty_Choice = ((*game_difficulty_Choice + 1) % 3) + 1;

                // for down arrow
                else if (ch == 66) *game_difficulty_Choice = (*game_difficulty_Choice % 3) + 1;
            }
        }

        else if (ch == 10)
        {
            return 0; // when enter key is pressed the function will return 0
        }

        return -1;

    #endif
}

void display_Main_Menu_V2(int selected)
{
    clear_Console();
    printf(BOLD RED "╔═════════════════════════════════════════════════════════════════╗" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                         ☠️  " BOLD RED "MAIN MENU" RESET " ☠️                          " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    
    for (int i = 1; i <= 5; i++)
    {
        if (i == selected)
        {
            if (i == 1) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[1] Reignite the Curse (Play Again)" RESET "                 " BOLD RED "║" RESET "\n");
            else if (i == 2) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[2] Read the Curse (Instructions)" RESET "                   " BOLD RED "║" RESET "\n");
            else if (i == 3) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[3] Blood Stained records (High Scores)" RESET "             " BOLD RED "║" RESET "\n");
            else if (i == 4) printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[4] Summoner's Journal (Credits)" RESET "                    " BOLD RED "║" RESET "\n");
            else printf(BOLD RED "║" RESET "           " BOLD BLUE "> " REV_COL "[5] Abandon Hope (Exit)" RESET "                             " BOLD RED "║" RESET "\n");
        }
        else
        {
            if (i == 1) printf(BOLD RED "║" RESET "             [1] Begin the Ritual (Start Game)                   " BOLD RED "║" RESET "\n");
            else if (i == 2) printf(BOLD RED "║" RESET "             [2] Read the Curse (Instructions)                   " BOLD RED "║" RESET "\n");
            else if (i == 3) printf(BOLD RED "║" RESET "             [3] Blood Stained records (High Scores)             " BOLD RED "║" RESET "\n");
            else if (i == 4) printf(BOLD RED "║" RESET "             [4] Summoner's Journal (Credits)                    " BOLD RED "║" RESET "\n");
            else printf(BOLD RED "║" RESET "             [5] Abandon Hope (Exit)                             " BOLD RED "║" RESET "\n");
        }
    }

    printf(BOLD RED "║" RESET "                                                                 " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "            🩸 " ITALICS RED "Choose your fate... or be chosen..." RESET " 🩸            " BOLD RED "║" RESET "\n");
    printf(BOLD RED "║" RESET "      🩸 " ITALICS RED "Use " BOLD BLUE "↑" RESET ITALICS RED " and " BOLD BLUE "↓" RESET ITALICS RED " arrow keys. Press " BOLD BLUE "Enter" RESET ITALICS RED " to select." RESET " 🩸       " BOLD RED "║" RESET "\n");
    printf(BOLD RED "╚═════════════════════════════════════════════════════════════════╝" RESET "\n\n");
}

void play_Game(int game_difficulty_Choice, char name[])
{
    int incorrects = 0; // stores the incorrect attempts
    char guessed_Letter = '\0'; // stores the guessed letter by the user dynamically
    char all_guessed_Letter[26];
    int blanks_counter; // counts the number of blanks printed
    int result = 0; // if win then 1
    
    clear_Console();
    
    printf("🔥 " ITALICS CYAN "The circle is drawn..." RESET "\n");
    pause_For(3);
    int random_word_selector = rand() % 25; // (rand() % max - min + 1) + min generates random number between max and min
    printf("⚔️  " ITALICS CYAN "A word has been chosen by the Demon..." RESET "\n");
    pause_For(5);
    printf("🧠  " ITALICS CYAN "Decipher it or be claimed by the Void..." RESET "\n\n");

    printf(BLUE "Press any key to continue..." RESET "\n");
    getch();

    clear_Console();

    int counter = 0;

    while(incorrects < MAX_INCORRECTS) 
    {
        clear_Console();
        blanks_counter = 0;
        
        if (game_difficulty_Choice == 1) // easy mode
        {
            printf("☠️  " BOLD RED "THE WORD OF BINDING" RESET " ☠️\n\n");
            for (int i = 0; i < easy_words[random_word_selector].length; i++) 
            {
                char current = easy_words[random_word_selector].word[i];
                int found = 0;
                for (int j = 0; j < counter; j++) 
                {
                    if (all_guessed_Letter[j] == current) 
                    {
                        found = 1;
                        break;
                    }
                }
                if (found)
                {
                    printf(YELLOW "%c " RESET, current);
                }
                else
                {
                    printf(RED "_ " RESET);
                    blanks_counter++;
                }
            }

            if (blanks_counter == 0)
            {
                result = 1;
                clear_Console();
                printf(GREEN BOLD "\n🔥 You have shattered the seal!\n");
                printf("🕯️  The word was: '%s'\n", easy_words[random_word_selector].word);
                printf("🎉 You live to see another moonrise... for now.\n\n" RESET);
                printf(BLUE "Press any key to continue...\n" RESET);
                getch();
                break;
            }

            guessed_Letter = '\0';
            printf("   [Letters: %d]\n\n", easy_words[random_word_selector].length);

            printf(MAGENTA "Lives Left: " RESET);
            for (int i = 1; i <= MAX_INCORRECTS - incorrects; i++)
            {
                printf("❤️ ");
            }
            printf("\n");
            printf(GREEN "Hint:" RESET " %s\n", easy_words[random_word_selector].hint);
            while(1)
            {
                printf(BLUE "Guess Letter: " RESET);
                scanf(" %c", &guessed_Letter);
                while ((getchar()) != '\n'); // clear input buffer before scanf

                if (guessed_Letter >= 65 && guessed_Letter <= 90 || guessed_Letter >= 97 && guessed_Letter <= 122) break;
                else
                {
                    printf(RED ITALICS "Runes rejected! This realm only accepts sacred alphabets (A-Z or a-z)." RESET "\n");
                    printf(RED "⏳  Wait 5 seconds before the curse allows you to try again..." RESET);
                    pause_For(5);
                    printf(CLEAR_LINE);
                    printf(LINE_UP);
                    printf(CLEAR_LINE);
                    printf(LINE_UP);
                    printf(CLEAR_LINE);
                }
            }
            guessed_Letter = toupper(guessed_Letter);
            all_guessed_Letter[counter] = guessed_Letter;
            counter++;

            if (!is_Guessed_Letter_Present(easy_words[random_word_selector].word, guessed_Letter))
            {
                incorrects++;
                draw_Hangman(incorrects);
                if (incorrects == 6) show_Demon_ASCII();
            }
        } // block end for easy mode

        if (game_difficulty_Choice == 2) // medium mode
        {
            printf("☠️  " BOLD RED "THE WORD OF BINDING" RESET " ☠️\n\n");
            for (int i = 0; i < medium_words[random_word_selector].length; i++) 
            {
                char current = medium_words[random_word_selector].word[i];
                int found = 0;
                for (int j = 0; j < counter; j++) 
                {
                    if (all_guessed_Letter[j] == current) 
                    {
                        found = 1;
                        break;
                    }
                }
                if (found)
                {
                    printf(YELLOW "%c " RESET, current);
                }
                else
                {
                    printf(RED "_ " RESET);
                    blanks_counter++;
                }
            }

            if (blanks_counter == 0)
            {
                result = 1;
                clear_Console();
                printf(GREEN BOLD "\n🔥 You have shattered the seal!\n");
                printf("🕯️  The word was: '%s'\n", medium_words[random_word_selector].word);
                printf("🎉 You live to see another moonrise... for now.\n\n" RESET);
                printf(BLUE "Press any key to continue...\n" RESET);
                getch();
                break;
            }

            guessed_Letter = '\0';
            printf("   [Letters: %d]\n\n", medium_words[random_word_selector].length);

            printf(MAGENTA "Lives Left: " RESET);
            for (int i = 1; i <= MAX_INCORRECTS - incorrects; i++)
            {
                printf("❤️ ");
            }
            printf("\n");
            printf(GREEN "Hint:" RESET " %s\n", medium_words[random_word_selector].hint);
            while(1)
            {
                printf(BLUE "Guess Letter: " RESET);
                scanf(" %c", &guessed_Letter);
                while ((getchar()) != '\n'); // clear input buffer before scanf

                if (guessed_Letter >= 65 && guessed_Letter <= 90 || guessed_Letter >= 97 && guessed_Letter <= 122) break;
                else
                {
                    printf(RED ITALICS "Runes rejected! This realm only accepts sacred alphabets (A-Z or a-z)." RESET "\n");
                    printf(RED "⏳  Wait 5 seconds before the curse allows you to try again..." RESET);
                    pause_For(5);
                    printf(CLEAR_LINE);
                    printf(LINE_UP);
                    printf(CLEAR_LINE);
                    printf(LINE_UP);
                    printf(CLEAR_LINE);
                }
            }
            guessed_Letter = toupper(guessed_Letter);
            all_guessed_Letter[counter] = guessed_Letter;
            counter++;

            if (!is_Guessed_Letter_Present(medium_words[random_word_selector].word, guessed_Letter))
            {
                incorrects++;
                draw_Hangman(incorrects);
                if (incorrects == 6) show_Demon_ASCII();
            }
        } // block end for medium mode

        if (game_difficulty_Choice == 3) // hard mode
        {
            printf("☠️  " BOLD RED "THE WORD OF BINDING" RESET " ☠️\n\n");
            for (int i = 0; i < hard_words[random_word_selector].length; i++) 
            {
                char current = hard_words[random_word_selector].word[i];
                int found = 0;
                for (int j = 0; j < counter; j++) 
                {
                    if (all_guessed_Letter[j] == current) 
                    {
                        found = 1;
                        break;
                    }
                }
                if (found)
                {
                    printf(YELLOW "%c " RESET, current);
                }
                else
                {
                    printf(RED "_ " RESET);
                    blanks_counter++;
                }
            }

            if (blanks_counter == 0)
            {
                result = 1;
                clear_Console();
                printf(GREEN BOLD "\n🔥 You have shattered the seal!\n");
                printf("🕯️  The word was: '%s'\n", hard_words[random_word_selector].word);
                printf("🎉 You live to see another moonrise... for now.\n\n" RESET);
                printf(BLUE "Press any key to continue...\n" RESET);
                getch();
                break;
            }

            guessed_Letter = '\0';
            printf("   [Letters: %d]\n\n", hard_words[random_word_selector].length);

            printf(MAGENTA "Lives Left: " RESET);
            for (int i = 1; i <= MAX_INCORRECTS - incorrects; i++)
            {
                printf("❤️ ");
            }
            printf("\n");
            printf(GREEN "Hint:" RESET " %s\n", hard_words[random_word_selector].hint);
            while(1)
            {
                printf(BLUE "Guess Letter: " RESET);
                scanf(" %c", &guessed_Letter);
                while ((getchar()) != '\n'); // clear input buffer before scanf

                if (guessed_Letter >= 65 && guessed_Letter <= 90 || guessed_Letter >= 97 && guessed_Letter <= 122) break;
                else
                {
                    printf(RED ITALICS "Runes rejected! This realm only accepts sacred alphabets (A-Z or a-z)." RESET "\n");
                    printf(RED "⏳  Wait 5 seconds before the curse allows you to try again..." RESET);
                    pause_For(5);
                    printf(CLEAR_LINE);
                    printf(LINE_UP);
                    printf(CLEAR_LINE);
                    printf(LINE_UP);
                    printf(CLEAR_LINE);
                }
            }
            guessed_Letter = toupper(guessed_Letter);
            all_guessed_Letter[counter] = guessed_Letter;
            counter++;

            if (!is_Guessed_Letter_Present(hard_words[random_word_selector].word, guessed_Letter))
            {
                incorrects++;
                draw_Hangman(incorrects);
                if (incorrects == 6) show_Demon_ASCII();
            }
        } // block end for hard mode
    }
    update_Scoreboard_VS_Comp(game_difficulty_Choice, name, result);
}

int is_Guessed_Letter_Present(char word[], char guessed_Letter)
{
    int is_Present = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        if (guessed_Letter == word[i]) is_Present = 1;
    }

    if (is_Present == 1) return 1;
    else return 0;
}

void draw_Hangman(int incorrects)
{
    switch(incorrects) {
        case 1:
            printf("  _______   \n");
            printf(" |/      |  \n");
            printf(" |      ( ) \n");
            printf(" |          \n");
            printf(" |          \n");
            printf(" |          \n");
            printf("_|___       \n\n");
            break;
            
        case 2:
            printf("  _______   \n");
            printf(" |/      |  \n");
            printf(" |      ( ) \n");
            printf(" |       |  \n");
            printf(" |       |  \n");
            printf(" |          \n");
            printf("_|___       \n\n");
            break;
            
        case 3:
            printf("  _______   \n");
            printf(" |/      |  \n");
            printf(" |      ( ) \n");
            printf(" |      /|  \n");
            printf(" |       |  \n");
            printf(" |          \n");
            printf("_|___       \n\n");
            break;
            
        case 4:
            printf("  _______     \n");
            printf(" |/      |    \n");
            printf(" |      ( )   \n");
            printf(" |      /|\\"); printf("  \n");
            printf(" |       |    \n");
            printf(" |            \n");
            printf("_|___         \n\n");
            break;

        case 5:
            printf("  _______     \n");
            printf(" |/      |    \n");
            printf(" |      ( )   \n");
            printf(" |      /|\\"); printf("  \n");
            printf(" |       |    \n");
            printf(" |      /     \n");
            printf("_|___         \n\n");
            break;
            
        case 6:
            printf("  _______     \n");
            printf(" |/      |    \n");
            printf(" |      (x)   \n");
            printf(" |      /|\\"); printf("  \n");
            printf(" |       |    \n");
            printf(" |      / \\"); printf("  \n");
            printf("_|___         \n\n");
            printf(BOLD RED "💀 THE RITUAL HAS CLAIMED ANOTHER SOUL 💀\n\n" RESET);
            break;
    }
    printf(BLUE "Press any key to continue...\n" RESET);
    getch();
}

void show_Demon_ASCII() {
    clear_Console();

    printf(RED BOLD "   ,    ,    /\\"); printf("   /\\"); printf("\n");
    printf("  /( /\\"); printf(" )\\"); printf("  _\\"); printf(" \\"); printf("_/ /_"); printf("\n");
    printf("  |\\"); printf("_||_/| < \\"); printf("_   _/ >"); printf("\n");
    printf("  \\"); printf("______/  \\"); printf("|0   0|/"); printf("\n");
    printf("    _\\"); printf("/_   _(_  ^  _)_"); printf("\n");
    printf("   ( () ) /`\\"); printf("|V\"\"\"V|/`"); printf("\\"); printf("\n");
    printf("     {}   \\"); printf("  \\"); printf("_____/  /"); printf("\n");
    printf("     ()   /\\"); printf("   )=(   /"); printf("\\"); printf("\n");
    printf("     {}  /  \\"); printf("_/\\"); printf("=/\\"); printf("_/  "); printf("\\"); printf("\n");

    printf("\n     THE DEMON HAS RISEN");
    printf("\n🔥  YOUR SOUL IS CLAIMED  🔥" RESET "\n\n\n");
    printf(BLUE "Press any key to continue...\n" RESET);
    getch();
}

void update_Scoreboard_VS_Comp(int game_difficulty_Choice, char name[], int result)
{
    FILE* file = fopen("scoreboard.txt", "a");

    fprintf(file, "═══════════════════════════════════════════════════════════════\n");
    if (game_difficulty_Choice == 1) fprintf(file, "  🎮 MODE: VS Computer                  🧠  DIFFICULTY: Easy  \n");
    else if (game_difficulty_Choice == 2) fprintf(file, "  🎮 MODE: VS Computer                  🧠  DIFFICULTY: Medium\n");
    else fprintf(file, "  🎮 MODE: VS Computer                  🧠  DIFFICULTY: Hard  \n");
    fprintf(file, "═══════════════════════════════════════════════════════════════\n\n");
    
    fprintf(file, "🧑  PLAYER: %s\n", name);
    if (result == 1) 
    {
        fprintf(file, "🏆  RESULT: Won\n");
        fprintf(file, "📜  DESCRIPTION: %s\n", win_Msg[(rand() % 5)]);
    }
    else 
    {
        fprintf(file, "🏆  RESULT: Lost\n");
        fprintf(file, "📜  DESCRIPTION: %s\n", lost_Msg[(rand() % 5)]);
    }
    fprintf(file, "═══════════════════════════════════════════════════════════════\n\n");

    fclose(file);
}

void display_Scoreboard()
{
    FILE* file = fopen("scoreboard.txt", "r");

    if (file == NULL) 
    {
        clear_Console();
        printf(RED ITALICS "\n☠️  The Hall of Echoes is silent...\n");
        printf("📜  No fates have been recorded.\n");
        printf("🕸️  Either no soul has dared the ritual...\n");
        printf("    ...or the scroll of records has been lost to time.\n\n");
        printf("🔮  Begin your descent, and etch your legacy in blood.\n\n" RESET);

        printf(ITALICS BLUE "Press any key to revert back to the Main Menu..." RESET);
        getch();

        return;
    }

    clear_Console();

    char line[256];
    printf(BOLD RED "═══════════════════════════════════════════════════════════════\n" RESET);
    printf(BOLD RED " ------------ BLOOD STAINED RECORDS (SCOREBOARD) -------------\n" RESET);
    printf(BOLD RED "═══════════════════════════════════════════════════════════════\n\n\n" RESET);
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s\n", line);
    }
    fclose(file);

    printf(ITALICS BLUE "\n\n\nPress any key to revert back to the Main Menu...\n" RESET);
    printf(BOLD RED "═══════════════════════════════════════════════════════════════\n" RESET);
    getch();
}


int main()
{
    srand(time(NULL));

    int main_menu_Choice = 1; // stores the menu option selected by the user, default is the first option 
    int game_difficulty_Choice; // stores the game difficulty option selected by the user, default is the first option
    int game_once_played = 0; // 0 for false and 1 for true
    char name[20]; // stores the name of the player
    int is_Alpha;

    display_Welcome_Scrn(); // displays the welcome screen

    do
    {
        game_difficulty_Choice = 1; // stores the game difficulty option selected by the user, default is the first option

        if (game_once_played == 1)
        {
            while(1)
            {
                display_Main_Menu_V2(main_menu_Choice);

                if (choice_Selection_using_Arrows_for_Main_Menu(&main_menu_Choice) == 0) break;
            }
        }
        else 
        {
            while(1)
            {
                display_Main_Menu_V1(main_menu_Choice);

                if (choice_Selection_using_Arrows_for_Main_Menu(&main_menu_Choice) == 0) break;
            }
        }

        switch (main_menu_Choice)
        {
            case 1:
                while(1)
                {
                    display_Game_Difficulty_Menu(game_difficulty_Choice);
    
                    if (choice_Selection_using_Arrows_for_Game_Difficulty_Menu(&game_difficulty_Choice) == 0) break;
                }

                if (game_once_played == 0)
                {
                    while (1)
                    {
                        clear_Console();
                        printf(BLUE "Enter your name: " RESET);
                        scanf("%19s", name);
                        while ((getchar()) != '\n'); // clear input buffer before scanf

                        int len = strlen(name);
                        for(int i = 0; i < len; i++)
                        {
                            is_Alpha = isalpha(name[i]);
                            if (is_Alpha == 0) 
                            {
                                printf(RED "⚠️  Realm accepts only Alphabet A to Z or a to z.\n" RESET);
                                printf(ITALICS BLUE "Press any key to continue..." RESET);
                                getch();
                                break;
                            }
                        }
                        if (is_Alpha) break;
                    }
                    int len = strlen(name);
                    for(int i = 0; i < len; i++)
                    {
                        if (i == 0) name[i] = toupper(name[i]);
                        else name[i] = tolower(name[i]);
                    }
                }

                play_Game(game_difficulty_Choice, name);
                game_once_played = 1;
                break;

            case 2: 
                display_Instructions();
                break;

            case 3:
                display_Scoreboard();
                break;

            case 4: 
                display_Credits();
                break;

            case 5: 
                display_Exit_Msg();
                break;
                
        } // switch case closed
    } // do block closed
    while(main_menu_Choice != 5);

    return 0;
}
