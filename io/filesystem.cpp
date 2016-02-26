#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QByteArray>

#include "io/filesystem.h"

namespace sn{
namespace corelib{

QString Filesystem::m_errorMsg = "";

QString Filesystem::getErrorString()
{
   return m_errorMsg;
}

QFileInfoList Filesystem::ls(const QString &path, int level)
{
   QDir targetDir(path);
   QFileInfoList ret;
   if(!targetDir.exists()){
      return ret;
   }
   if(1 == level){
      return targetDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
   }
   Filesystem::traverseFs(path, level, [&ret](QFileInfo& fileInfo, int)->void{
      ret.append(fileInfo);
   });
   return ret;
}

bool Filesystem::copyFile(const QString &source, const QString &destination, bool overwrite)
{
   if(source == destination){
      return true;
   }
   QFile sourceFile(source);
   if(!sourceFile.exists()){
      return false;
   }
   QFileInfo destinationInfo(destination);
   QDir targetDir = destinationInfo.absoluteDir();
   if(!targetDir.exists()){
      targetDir.mkpath(".");
   }
   if(destinationInfo.exists() && overwrite){
      QFile::remove(destination);
   }
   if(!QFile::copy(source, destination)){
      return false;
   }
   return true;
}

int Filesystem::filePutContents(const QString &filename, const QString &content)
{
   QFileInfo fileInfo(filename);
   QDir targetDir = fileInfo.absoluteDir();
   if(!fileInfo.exists()){
      targetDir.mkpath(".");
   }
   QFile target(filename);
   if(!target.open(QFile::WriteOnly| QFile::Truncate | QFile::Text)){
      return -1;
   }
   return target.write(content.toLocal8Bit());
}

QByteArray Filesystem::fileGetContents(const QString &filename)
{
   QFile target(filename);
   if(!target.exists() || !target.open(QFile::ReadOnly|QFile::Text)){
      return QByteArray();
   }
   return target.readAll();
}

bool Filesystem::deleteDirRecusive(const QString &dir)
{
   return QDir(dir).removeRecursively();
}

bool Filesystem::deleteDir(const QString &dir)
{
   return QDir(dir).remove(".");
}

bool Filesystem::createDir(const QString &dirName)
{
   return QDir(dirName).mkdir(".");
}

bool Filesystem::createPath(const QString &dirPath)
{
   return QDir(dirPath).mkpath(".");
}

bool Filesystem::copyDir(QString source, QString target, bool direct)
{
   if(!Filesystem::dirExist(source)){
      m_errorMsg = QString("%1 is not exist").arg(source);
      return false;
   }
   if(!Filesystem::dirExist(target)){
      m_errorMsg = QString("%1 is not exist").arg(target);
      return false;
   }
   if(source == target.mid(0, source.size())){
      m_errorMsg = QString("target dir : %1 is subdirectory of source dir : %2").arg(target, source);
      return false;
   }
   if(!direct){
      target = target+'/'+QFileInfo(source).baseName();
      while(Filesystem::fileExist(target) && QFileInfo(target).isFile()){
         target += "_副本";
      }
   }
   if(!Filesystem::fileExist(target)){
      Filesystem::createDir(target);  
   }
   int sourceNameLen = source.size();
   Filesystem::traverseFs(source, 0, [&](QFileInfo &fileInfo, int){
      QString sourceFilename = fileInfo.absoluteFilePath();
      QString str = sourceFilename.mid(sourceNameLen);
      QString targetFileName = target + str;
      if(fileInfo.isFile()){
         Filesystem::copyFile(sourceFilename, targetFileName);
      }else if(fileInfo.isDir()){
         Filesystem::createDir(targetFileName);
      }
   });
   return true;
}

bool Filesystem::chmod(const QString &filename, mode_t mode)
{
   if(-1 == ::chmod(filename.toLocal8Bit(), mode)){
      m_errorMsg = "chmod failure";
      return false;
   }
   return true;
}

bool Filesystem::chown(const QString &filename, uid_t user, gid_t group)
{
   if(-1 == ::chown(filename.toLocal8Bit(), user, group)){
      m_errorMsg = "chown failure";
      return false;
   }
   return true;
}

bool Filesystem::isReadable(const QString &filename)
{
   return QFileInfo(filename).isReadable();
}

bool Filesystem::deleteFile(const QString &filePath)
{
   return QFile::remove(filePath);
}

bool Filesystem::isWritable(const QString &filename)
{
   return QFileInfo(filename).isWritable();
}

bool Filesystem::fileExist(const QString &filePath)
{
   return QFileInfo(filePath).exists();
}

bool Filesystem::dirExist(const QString &dirPath)
{
   return QDir(dirPath).exists();
}


}//corelib
}//sn