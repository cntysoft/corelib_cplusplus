#ifndef SN_CORELIB_IO_TERMINAL_H
#define SN_CORELIB_IO_TERMINAL_H
#include <QPair>
#include <QMap>
#include "global/global.h"

namespace sn
{
namespace corelib 
{

enum class TerminalColor
{
   Default,
   Black,
   Red,
   Green,
   Yellow,
   Blue,
   Magenta,
   Cyan,
   LightGray,
   DarkGray,
   LightRed,
   LightGreen,
   LightYellow,
   LightBlue,
   LightMagenta,
   LightCyan,
   White
};

class SN_CORELIB_EXPORT Terminal
{
   
public:
   static QByteArray getColorText(const char* str, TerminalColor color = TerminalColor::Default, bool underline = false, bool blink = false);
   static void writeText(const char* str, TerminalColor color = TerminalColor::Default, bool underline = false, bool blink = false);
   static void clearScreen();
   static void setCursorPos(int x, int y);
   static void showCursor();
   static void hideCursor();
   static void resetColor();
   static void forwardCursor(int step = 1);
   static void backwardCursor(int step = 1);
   static QPair<int, int> getWindowSize();
protected:
   const static QMap<TerminalColor, int> colorMap;
   const static int BOLD_CODE = 1;
   const static int UNDERLINE_CODE = 4;
   const static int BLINK_CODE = 5;
   const static int REVERSE_CODE = 7;
protected:
   static QString lastTTYMode;
};

}//sn
}//corelib

#endif // SN_CORELIB_IO_TERMINAL_H

