#include <QProcess>
#include <QStringList>
#include <QByteArray>

#include <sys/ioctl.h>
#include <iostream>
#include <termios.h>
#include <cstdio>
#include <unistd.h>

#include "terminal.h"
namespace sn 
{
namespace corelib 
{

const QMap<TerminalColor, int> Terminal::colorMap{
   {TerminalColor::Default, 39},
   {TerminalColor::Black, 30},
   {TerminalColor::Red, 31},
   {TerminalColor::Green, 32},
   {TerminalColor::Yellow, 33},
   {TerminalColor::Blue, 34},
   {TerminalColor::Magenta, 35},
   {TerminalColor::Cyan, 36},
   {TerminalColor::LightGray, 37},
   {TerminalColor::DarkGray, 90},
   {TerminalColor::LightRed, 91},
   {TerminalColor::LightGreen, 92},
   {TerminalColor::LightYellow, 93},
   {TerminalColor::LightBlue, 94},
   {TerminalColor::LightMagenta, 95},
   {TerminalColor::LightCyan, 96},
   {TerminalColor::White, 97}
};

QByteArray Terminal::getColorText(const char* str, TerminalColor color, bool underline, bool blink)
{
   QByteArray ret("\e[");
   QStringList codesList;
   if(underline){
      codesList << QByteArray::number(UNDERLINE_CODE);
   }
   if(blink){
      codesList << QByteArray::number(BLINK_CODE);
   }
   codesList << QString::number(colorMap[color]);
   ret.append(codesList.join(';'));
   ret.append('m');
   ret.append(str);
   ret.append("\e[0m");
   return ret;
}

void Terminal::writeText(const char *str, TerminalColor color, bool underline, bool blink)
{
   std::cout << getColorText(str, color, underline, blink).constData() << std::flush;
}

void Terminal::clearCurrentLine()
{
   std::cout << "\x1b[1A\x1b[2K" << std::flush;
}

void Terminal::clearScreen()
{
   std::cout << "\x1b[2J" << std::flush;      // reset bg color
   setCursorPos(1, 1); // reset cursor position
}

void Terminal::setCursorPos(int x, int y)
{
   std::cout << QString("\x1b[%1;%2f").arg(y).arg(x).toStdString() << std::flush;
}

void Terminal::showCursor()
{
   std::cout << "\x1b[?25h" << std::flush;;
}

void Terminal::hideCursor()
{
   std::cout << "\x1b[?25l" << std::flush;;
}

void Terminal::forwardCursor(int step)
{
   std::cout << QString("\x1b[%1C").arg(step).toStdString() << std::flush;
}

void Terminal::backwardCursor(int step)
{
   std::cout << QString("\x1b[%1D").arg(step).toStdString() << std::flush;
}

void Terminal::resetColor()
{
   std::cout << "\x1b[0;49m";  // reset bg color
   std::cout << "\x1b[22;39m"; // reset fg bold, bright and faint
   std::cout << "\x1b[25;39m"; // reset fg blink
   std::cout << "\x1b[24;39m"; // reset fg underline
   std::cout << std::flush;
}

QPair<int, int> Terminal::getWindowSize()
{
   struct winsize windowSize;
   QPair<int, int> pair;
   if(-1 != ioctl(STDIN_FILENO, TIOCGWINSZ, &windowSize)){
      pair.first = windowSize.ws_col;
      pair.second = windowSize.ws_row;
   }
   return pair;
}

}//corelib
}//sn