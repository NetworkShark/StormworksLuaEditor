#pragma once

#ifndef KEYWORDHIGHLIGHT_H
#define KEYWORDHIGHLIGHT_H

#include <QString>
#include <QStringList>

class keywordHighlight
{
public:
    QString color;
    QStringList keywords;

    keywordHighlight(QString color, QString keywords, const char separator = '|')
    {
        this->color = color;
        this->keywords = keywords.split(separator, QString::SkipEmptyParts);
    }

};

#endif // KEYWORDHIGHLIGHT_H
