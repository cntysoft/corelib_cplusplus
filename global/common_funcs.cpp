#include <QString>
#include <pwd.h>
#include <unistd.h>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>
#include <QDebug>
#include <QProcess>
#include <QCryptographicHash>

#include "common_funcs.h"
#include "kernel/application.h"
#include "kernel/errorinfo.h"
#include "io/filesystem.h"

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

void throw_exception(ErrorInfo errorInfo, const QString &context)
{
   if(!context.isEmpty()){
      errorInfo.setExtraErrorInfo("context", context);
   }
   throw errorInfo;
}

void dump_mysql_table(const QString &user, const QString &password, 
                      const QString &dbname, const QString &table, 
                      const QString &targetDir)
{
   QProcess process;
   QStringList args{
      QString("-u%1").arg(user),
            QString("-p%1").arg(password),
            dbname,
            table,
            QString(" > %1/%2.sql").arg(targetDir, table)
   };
   if(!Filesystem::fileExist(targetDir)){
      Filesystem::createPath(targetDir);
   }
   process.setWorkingDirectory(targetDir);
   process.start("mysqldump", args);
   bool status = process.waitForFinished(-1);
   if(!status || process.exitCode() != 0){
      throw ErrorInfo(process.errorString());
   }
}

QString hmac_sha1(const QString &key, const QString &secret)
{
   //Length of the text, that will be hashed
   int   text_lenght;
   //For secret word.
   QByteArray K;
   //Lenght of secret word
   int   K_lenght;
   K_lenght = secret.size();
   text_lenght = key.size();
   //Need to do for XOR operation. Transforms QString to
   //unsigned char
   K = secret.toLocal8Bit();
   //Inner padding
   QByteArray ipad;
   //Outer pad
   QByteArray opad;
   //if secret key > 64 bytes use this to obtain sha1 key
   if(K_lenght > 64)
   {
      QByteArray tempSecret;
      tempSecret.append(secret);
      K = QCryptographicHash::hash(tempSecret,  QCryptographicHash::Sha1);
      K_lenght = 20;
   }
   //Fills ipad and opad with zeros
   //bzero( ipad, sizeof ipad);
   //bzero( opad, sizeof opad);
   ipad.fill( 0, 64);
   opad.fill(0, 64);
   //Copies Secret to ipad and opad
   //bcopy( K, ipad, K_lenght);
   //bcopy( K, opad, K_lenght);
   ipad.replace(0, K_lenght, K);
   opad.replace(0, K_lenght, K);
   //XOR operation for inner and outer pad
   for (int i=0; i<64; i++)
   {
      ipad[i] = ipad[i] ^ 0x36;
      opad[i] = opad[i] ^ 0x5c;
   }
   //Stores hashed content
   QByteArray context;
   //Appends XOR:ed ipad to context
   context.append(ipad,64);
   //Appends key to context
   context.append(key);
   //Hashes Inner pad
   QByteArray Sha1   = QCryptographicHash::hash(context, QCryptographicHash::Sha1);
   context.clear();
   //Appends opad to context
   context.append(opad,64);
   //Appends hashed inner pad to context
   context.append(Sha1);
   Sha1.clear();
   // Hashes outerpad
   Sha1 = QCryptographicHash::hash(context, QCryptographicHash::Sha1);
   // String to return hashed stuff in Base64 format
   return Sha1.toBase64();
}

}//corelib
}//sn