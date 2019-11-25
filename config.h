#ifndef CONFIG_H
#define CONFIG_H
#define  CONFIG_AUTOR   "Alex Krieg (KRIA)"
#define  CONFIG_VERSION "00.01.03"
//              Datum   25.11.2019


#include "config_global.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QString>


#define CONFIG_DEFAULT_COMMENT_PREFIX "//"

class CONFIGSHARED_EXPORT Config
{
    public:
        Config();
        Config(QString filename);
        ~Config();
        static const QString version();
        static const QString autor();

        void filename(QString filename);
        QString filename();

        void read();
        void save();

        void fileHead(QStringList head);
        QStringList fileHead();

        void parameter(QString parameterName, QString parameter);
        void parameter(QString parameterName, QString &parameter);
        void parameter(QString parameterName, QString &parameter, QString mode);
        void parameterShort(QString parameterName, short &parameter, QString mode);
        void parameterUShort(QString parameterName, unsigned short &parameter, QString mode);
        void parameterInt(QString parameterName, int &parameter, QString mode);
        void parameterUInt(QString parameterName, unsigned int &parameter, QString mode);
        void parameterDouble(QString parameterName, double &parameter, QString mode);
        void parameterFloat(QString parameterName, float &parameter, QString mode);
        void parameterBool(QString parameterName, bool &parameter, QString mode);
        QString line(int line);
        QStringList text();

        QStringList    parameterList();
        QStringList    parameterValue();
        int parameters();

        void deleteMultipleParameter(bool doDelete);
        bool deleteMultipleParameter();

        void resetCommentPrefixList();
        void addCommentPrefix(QString prefix);
        void deleteCommentPrefix(QString prefix);
        void commentPrefixList(QStringList prefixList);
        QStringList    commentPrefixList();
    private:
        int getParamRow(QString paramName);
        void readParameter();
        void writeText();
        int parameterindex(QString parameterName);

        QString _filename;
        QStringList _fileHead;
        QStringList _parameterNameList;
        QStringList _parameterList;
        QStringList _textList;
        QStringList _commentPrefixList;
        bool    _deleteMultipleParameter;
        bool    _update;

        FILE *_file;
};

#endif // CONFIG_H
