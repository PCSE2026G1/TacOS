#include <cerrno>
#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

#ifdef GENERATE_DEBUG
#define PCB_SIZE 336
#else
#define PCB_SIZE 186
#endif

#define FDS_SIZE 4
#define PROCESS_FLAGS 0x00c0

struct Process
{
    std::string func;
    unsigned int priority;
    bool file;
};

static void parseArgs(std::size_t size, char* const* argv, Process* prcs)
{
    for (std::size_t i = 0; i < size; i++)
    {
        const char* str = argv[i];
        Process& prc = prcs[i];

        if (str[0] == 'f')
        {
            prc.file = true;
            str++;
        }
        else
        {
            prc.file = false;
        }

        std::size_t numSize = 0;
        while (str[numSize] >= '0' && str[numSize] <= '9')
            numSize++;

        char* buf = new char[numSize + 1];
        std::copy(str, str + numSize, buf);
        buf[numSize] = '\0';
        prc.priority = std::stoul(buf);
        delete[] buf;
        str += numSize;

        prc.func = str;
    }
}

int main(int argc, char** argv)
{
    std::ofstream out(argv[1]);
    if (!out)
    {
        std::cerr << argv[1] << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    std::size_t size = argc - 2;
    Process* prcs = new Process[size];
    parseArgs(size, argv + 2, prcs);

    for (std::size_t i = 0; i < size; i++)
    {
        const Process& prc = prcs[i];

        out << "." << prc.func << "_pcb bs " << PCB_SIZE << std::endl;
        if (prc.file)
        {
            out << "." << prc.func << "_fds ws " << FDS_SIZE << std::endl;
        }
    }

    out << "_app_init_processes" << std::endl;
    for (std::size_t i = 0; i < size; i++)
    {
        const Process& prc = prcs[i];

        if (prc.file)
        {
            out << "    ld g0, #." << prc.func << "_fds" << std::endl;
        }
        else
        {
            out << "    ld g0, #0" << std::endl;
        }
        out << "    push g0" << std::endl;

        out << "    ld g0, #." << prc.func << "_pcb" << std::endl;
        out << "    push g0" << std::endl;

        out << "    ld g0, #" << PROCESS_FLAGS << std::endl;
        out << "    push g0" << std::endl;

        out << "    ld g0, #0" << std::endl;
        out << "    push g0" << std::endl;

        out << "    ld g0, #" << prc.priority << std::endl;
        out << "    push g0" << std::endl;

        out << "    ld g0, #_" << prc.func << std::endl;
        out << "    push g0" << std::endl;

        out << "    call _newProc" << std::endl;
        out << "    add sp, #12" << std::endl;

        out << "    ld g0, #." << prc.func << "_pcb" << std::endl;
        out << "    push g0" << std::endl;
        out << "    call _schProc" << std::endl;
        out << "    add sp, #2" << std::endl;
    }
    out << "    ret" << std::endl;

    delete[] prcs;
    if (!out)
    {
        std::cerr << argv[1] << ": " << std::strerror(errno) << std::endl;
        return 1;
    }

    return 0;
}
