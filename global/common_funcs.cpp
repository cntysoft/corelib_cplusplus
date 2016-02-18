#include <QString>
#include <pwd.h>
#include <unistd.h>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>
#include <QDebug>

#include "common_funcs.h"
#include "kernel/application.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{

QString get_current_user_home_dir()
{
   passwd* pwd = getpwuid(getuid());
   return QString(pwd->pw_dir);
}

Application* get_application_ref()
{
   return Application::instance();
}

QString get_corelib_version()
{
   return QString(SN_CORELIB_VERSION);
}

static void format_str_cycle(const QString &format, const QStringList &args, QString &result)
{
   switch (args.size()) {
   case 1:
   {
      result = format.arg(args.at(0));
      break;
   }
   case 2:
   {
      result = format.arg(args.at(0), args.at(1));
      break;
   }
   case 3:
   {
      result = format.arg(args.at(0), args.at(1), 
                          args.at(2));
      break;
   }
   case 4:
   {
      result = format.arg(args.at(0), args.at(1), 
                          args.at(2), args.at(3));
      break;
   }
   case 5:
   {
      result = format.arg(args.at(0), args.at(1), 
                          args.at(2), args.at(3),
                          args.at(4));
      break;
   }
   case 6:
   {
      result = format.arg(args.at(0), args.at(1), 
                          args.at(2), args.at(3),
                          args.at(4), args.at(5));
      break;
   }
   case 7:
   {
      result = format.arg(args.at(0), args.at(1), 
                          args.at(2), args.at(3),
                          args.at(4), args.at(5),
                          args.at(6));
      break;
   }
   case 8:
   {
      result = format.arg(args.at(0), args.at(1), 
                          args.at(2), args.at(3),
                          args.at(4), args.at(5),
                          args.at(6), args.at(7));
      break;
   }
   case 9:
   {
      result = format.arg(args.at(0), args.at(1), 
                          args.at(2), args.at(3),
                          args.at(4), args.at(5),
                          args.at(6), args.at(7),
                          args.at(8));
      break;
   }
   }
}

QString SN_CORELIB_EXPORT format_str(const QString &format, const QStringList &args)
{
   QString result;
   if(args.isEmpty()){
      result = format;
   }else if(args.size() <= 9){
      result = format;
      int total = 0;
      QRegularExpression regex("%\\d+|%s");
      QRegularExpressionMatchIterator it = regex.globalMatch(format);
      while(it.hasNext()){
         QRegularExpressionMatch mret = it.next();
         result.replace(mret.capturedStart(), mret.capturedLength(), QString("%%1").arg(total+1));
         total++;
      }
      
      format_str_cycle(result, args, result);
   }else{
      QRegularExpression regex("%\\d+|%s");
      QRegularExpressionMatchIterator it = regex.globalMatch(format);
      result = format;
      int total = 0;
      int cycle = 0;
      while(it.hasNext()){
         QRegularExpressionMatch mret = it.next();
         result.replace(mret.capturedStart(), mret.capturedLength(), QString("%%1").arg(total+1));
         int mod = total % 9;
         if(0 == mod){
            cycle++;
         }
         total++;
      }
      for(int i = 0; i < cycle; i++){
         QStringList cycleItems;
         for(int j = 0; j < 9; j++){
            int pos = i * 9 + j;
            if(pos == total){
               break;
            }
            cycleItems.append(args.at(pos));
         }
         format_str_cycle(result, cycleItems, result);
      }
   }
   return result;
}

bool is_scalar(const QVariant &value)
{
   if(value.type() == QVariant::Int || value.type() == QVariant::Bool ||
         value.type() == QVariant::UInt || value.type() == QVariant::LongLong ||
         value.type() == QVariant::ULongLong || value.type() == QVariant::Double ||
         value.type() == QVariant::Char){
      return true;
   }
   return false;
}

void SN_CORELIB_EXPORT throw_exception(ErrorInfo errorInfo, const QString &context)
{
   if(!context.isEmpty()){
      errorInfo.setExtraErrorInfo("context", context);
   }
   throw errorInfo;
}

}//corelib
}//sn