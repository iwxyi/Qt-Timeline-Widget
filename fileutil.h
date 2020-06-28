#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QStandardPaths>

class FileUtil
{
public:
    static QString readTextFile(QString path)
    {
        return readTextFile(path, QTextCodec::codecForName(QByteArray("utf-8")));
    }

    static QString readTextFile(QString path, QString codec)
    {
        return readTextFile(path, QTextCodec::codecForName(QString(codec).toLatin1()));
    }

    static QString readTextFile(QString path, QTextCodec *codec)
    {
        QString text;
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "无法打开文件：" << path;
    //        QMessageBox::critical(nullptr, QObject::tr("错误"), QObject::tr("无法打开文件\n路径：%1").arg(path));
            return "";
        }
        if (!file.isReadable())
        {
            qDebug() << "无法打开文件：" << path;
    //        QMessageBox::critical(nullptr, QObject::tr("错误"), QObject::tr("该文件不可读\n路径：%1").arg(path));
            return "";
        }
        QTextStream text_stream(&file);
        text_stream.setCodec(codec);
        while(!text_stream.atEnd())
        {
            text = text_stream.readAll();
        }
        file.close();
        return text;
    }

    static bool writeTextFile(QString path, QString text)
    {
        return writeTextFile(path, text, QTextCodec::codecForName(QByteArray("utf-8")));
    }

    static bool writeTextFile(QString path, QString text, QTextCodec *codec)
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::critical(nullptr, QObject::tr("保存失败"), QObject::tr("打开文件失败\n路径：%1\n可能是无读写文件权限，请手动创建这个文件").arg(path));
        }
        QTextStream text_stream(&file);
        text_stream.setCodec(codec);
        text_stream << text;
        file.close();
        return true;
    }

    static QString readTextFileWithFolder(QString file, QString folder, QTextCodec* codec)
    {
        QString full = readTextFile(file, codec);

        if (folder.isEmpty())
            return full;
        if (!folder.endsWith("/"))
            folder += "/";
        QDir dir(folder);
        if (!dir.exists())
            return full;
        QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        foreach (QString file, files)
        {
            full += "\n" + readTextFile(folder + file, codec);
        }
        return full;
    }

    static bool writeTextFile(QString path, QString text, QString codec)
    {
        return writeTextFile(path, text, QTextCodec::codecForName(QString(codec).toLatin1()));
    }

    static bool copyFile(QString old_path, QString new_path, bool cover)
    {
        if (!isFileExist(old_path) || isFileExist(new_path))
        {
            if (!cover)
                return false;
            else
                deleteFile(new_path);
        }
        return QFile::copy(old_path, new_path);
    }

    static bool copyFile2(QString old_path, QString new_path)
    {
        if (!isFileExist(old_path))
            return false;
        if (isFileExist(new_path))
            deleteFile(new_path);
        return QFile::copy(old_path, new_path);
    }

    static bool deleteFile(QString path)
    {
        if (!isFileExist(path)) return true;
        if (QFileInfo(path).isFile())
            return QFile::remove(path);
        else
            return deleteDir(path);
    }

    static bool renameFile(QString path, QString new_path)
    {
        if (!isFileExist(path)) return false;
        if (isFileExist(new_path)) return false;
        return QFile::rename(path, new_path);
    }

    static bool isFileExist(QString path)
    {
        QFileInfo file_info(path);
        return file_info.exists();
    }

    static bool isDir(QString path)
    {
        QFileInfo file_info(path);
        if (!file_info.exists()) // 需要确保存在……
            return false;
        return file_info.isDir();
    }

    static bool ensureFileExist(QString path)
    {
        QFileInfo file_info(path);
        if (file_info.exists())
        {
            if (file_info.isDir())
            {
                QDir dir(path);
                dir.rmdir(path); // 删除目录

                QFile file(path);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) // 创建文件
                    QMessageBox::critical(nullptr, QObject::tr("创建失败"), QObject::tr("创建文件失败\n路径：%1").arg(path));
                file.close();
                return false;
            }
            return true;
        }
        else
        {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) // 创建文件
                QMessageBox::critical(nullptr, QObject::tr("创建失败"), QObject::tr("创建文件失败\n路径：%1").arg(path));
            file.close();
            return false;
        }
    }

    static bool ensureDirExist(QString path)
    {
        QFileInfo file_info(path);
        if (file_info.exists() && file_info.isFile())
        {
            QFile file(path);
            file.remove(); // 删除文件

            QDir dir(path); // 创建目录
            return dir.mkpath(path);
        }

        QDir dir(path);
        if (dir.exists())
        {
            return true;
        }
        else
        {
            // dir.mkdir(path) ;// 创建文件夹
            return dir.mkpath(path); // 创建多级目录
        }
    }

    static QString readExistedTextFile(QString path)
    {
        ensureFileExist(path);
        return readTextFile(path, QTextCodec::codecForName(QByteArray("utf-8")));
    }

    static bool ensureFileNotExist(QString path)
    {
        QFileInfo file_info(path);
        if (file_info.exists())
        {
            if (file_info.isDir())
            {
                QDir dir(path);
                dir.rmdir(path); // 删除目录
            }
            else
            {
                QFile file(path);
                file.remove();
            }
            return false;
        }
        return true;
    }

    static bool canBeFileName(QString name)
    {
        // 判断是否包含特殊字符
        QChar cs[] = {'\\', '/', ':', '*', '?', '"', '<', '>', '|', '\''};
        for (int i = 0; i < 10; i++)
            if (name.contains(cs[i]))
                return false;
        return true;
    }

    static QString getPathWithIndex(QString dir, QString name, QString suffix)
    {
        int index = 0;
        if (!dir.endsWith("/"))
            dir += "/";
        if (isFileExist(dir+name+suffix))
        {
            index++;
            while(isFileExist(dir+name+QString("(%1)").arg(index)+suffix))
                index++;
        }

        if (index)
            return dir+name+QString("(%1)").arg(index)+suffix;
        else
            return dir+name+suffix;
    }

    static QString getDirByFile(QString file_path)
    {
        QFileInfo info(file_path);
        if (info.isDir())
            return file_path;
        int index = file_path.lastIndexOf("/");
        if (index == -1)
            index = file_path.lastIndexOf("\\");
        if (index == -1)
            return file_path;
        return file_path.left(index); // 不包含末尾的 /
    }

    static void addLinkToDeskTop(const QString& filename,const QString& name)
    {
         QFile::link(filename, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/").append(name+".lnk"));
    }

    static bool deleteDir(const QString &path)
    {
        if (path.isEmpty())
            return false;

        QDir dir(path);
        if(!dir.exists())
            return true;

        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList fileList = dir.entryInfoList();
        foreach (QFileInfo fi, fileList)
        {
            if (fi.isFile())
                fi.dir().remove(fi.fileName());
            else
                deleteDir(fi.absoluteFilePath());
        }
        return dir.rmpath(dir.absolutePath());
    }

    static bool copyDir(const QString &source, const QString &destination, bool override)
    {
        QDir directory(source);
        if (!directory.exists())
        {
            return false;
        }

        QString srcPath = QDir::toNativeSeparators(source);
        if (!srcPath.endsWith(QDir::separator()))
            srcPath += QDir::separator();
        QString dstPath = QDir::toNativeSeparators(destination);
        if (!dstPath.endsWith(QDir::separator()))
            dstPath += QDir::separator();

        QDir root_dir(destination);
        root_dir.mkpath(destination);

        bool error = false;
        QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
        for (QStringList::size_type i=0; i != fileNames.size(); ++i)
        {
            QString fileName = fileNames.at(i);
            QString srcFilePath = srcPath + fileName;
            QString dstFilePath = dstPath + fileName;
            QFileInfo fileInfo(srcFilePath);
            if (fileInfo.isFile() || fileInfo.isSymLink())
            {
                if (override)
                {
                    QFile::setPermissions(dstFilePath, QFile::WriteOwner);
                }
                QFile::copy(srcFilePath, dstFilePath);
            }
            else if (fileInfo.isDir())
            {
                QDir dstDir(dstFilePath);
                dstDir.mkpath(dstFilePath);
                if (!copyDir(srcFilePath, dstFilePath, override))
                {
                    error = true;
                }
            }
        }

        return !error;
    }

    static QString readTextFileIfExist(QString path)
    {
        if (!isFileExist(path))
            return "";
        return readTextFile(path, QTextCodec::codecForName(QByteArray("utf-8")));
    }
};

#endif // FILEUTIL_H
