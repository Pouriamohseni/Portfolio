#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "LineInfo.h"

using namespace std;

const int LINESIZE = 16;

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 2)
        {
            throw domain_error(LineInfo("Usage: diagonal <textstring>", FILE, LINE));
        }

        int fdbinaryout = open("diagonal2.bin", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fdbinaryout == 0)
            throw domain_error(LineInfo("Open Failed File: diagonal2.bin", FILE, LINE));

        char space = '.';

        for (int line = 0; line < LINESIZE; line++)
            for (int column = 0; column < LINESIZE; column++)
                if (write(fdbinaryout, &space, 1) == -1)
                    throw domain_error(LineInfo("write() failed ", FILE, LINE));

        int position = 0;
        for (int arg = 1; arg < argc; arg++)
        {
            int n = strlen(argv[arg]);
            for (int i = 0; i < n; i++)
            {
                if (lseek(fdbinaryout, (LINESIZE + 1) * position, SEEK_SET) == -1)
                    throw domain_error(LineInfo("lseek() failed", FILE, LINE));
                if (write(fdbinaryout, &argv[arg][i], 1) == -1)
                    throw domain_error(LineInfo("write() failed ", FILE, LINE));
                position++;
            }
        }

        close(fdbinaryout);

        cout << "diagonal2.bin" << " has been created." << endl
             << "Use od -c diagonal2.bin to see the contents." << endl;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
        cout << endl
             << "Press the enter key once or twice to leave..." << endl;
        cin.ignore();
        cin.get();
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}