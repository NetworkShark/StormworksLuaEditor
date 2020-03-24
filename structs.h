#pragma once

#ifndef KEYWORDHIGHLIGHT_H
#define KEYWORDHIGHLIGHT_H

#include <map>
#include <QString>
#include <QStringList>

class KeywordHighlight
{
public:
    bool defaultColor;
    QString color;
    QRegExp regex;
    QStringList keywords;

    KeywordHighlight() {
        this->defaultColor = false;
        this->color = nullptr;
    }
    KeywordHighlight(QString color, bool defaultColor = false) {
        this->defaultColor = defaultColor;
        this->color = color;
    }
    KeywordHighlight(QString color, QRegExp regexp, bool defaultColor = false) {
        this->defaultColor = defaultColor;
        this->color = color;
        this->regex = regexp;
    }
    KeywordHighlight(QString color, QString keywords, const char separator = '|', bool defaultColor = false) {
        this->defaultColor = defaultColor;
        this->color = color;
        this->keywords = keywords.split(separator, QString::SkipEmptyParts);
    }

};

#endif // KEYWORDHIGHLIGHT_H

#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    std::map<QString, KeywordHighlight> highlightings;


};

#endif // SETTINGS_H
