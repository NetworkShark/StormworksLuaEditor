#pragma once

#ifndef KEYWORDHIGHLIGHT_H
#define KEYWORDHIGHLIGHT_H

#include <map>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

using namespace std;

class KeywordHighlight
{
public:
    bool defaultColor;
    int weight;
    QString color;
    QRegExp regex;
    QStringList keywords;

    KeywordHighlight(QString color = NULL, int weight = 0, bool defaultColor = false) {
        this->defaultColor = defaultColor;
        this->color = color;
        this->weight = weight;
    }
    KeywordHighlight(QString color, QRegExp regexp, int weight = 0) : KeywordHighlight(color, weight) {
        this->regex = regexp;
    }
    KeywordHighlight(QString color, QString keywords, int weight = 0, const char separator = '|') : KeywordHighlight(color, weight) {
        this->keywords = keywords.split(separator, Qt::SkipEmptyParts);
    }

    bool operator==(const KeywordHighlight &other) {
        return this->color == other.color &&
                this->weight == other.weight &&
                this->regex == other.regex &&
                this->keywords == other.keywords &&
                this->defaultColor == other.defaultColor;
    }
    friend bool operator==(const KeywordHighlight &left, const KeywordHighlight &right) {
        return left.color == right.color &&
                left.weight == right.weight &&
                left.regex == right.regex &&
                left.keywords == right.keywords &&
                left.defaultColor == right.defaultColor;
    }
    bool operator!=(const KeywordHighlight &other) { return !operator==(other); }
    friend bool operator!=(const KeywordHighlight &left, const KeywordHighlight &right) { return !(left == right); }

};

#endif // KEYWORDHIGHLIGHT_H

#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
//  Properties
    QString fileSourcePath;
    //map<QString, KeywordHighlight> highlightings;
    struct FONT_SETTINGS {
        QString fontFamily;
        uint fontSize;
        map<QString, KeywordHighlight> highlightings;
    };
    FONT_SETTINGS fontSettings;

//  Functions

    static Settings* readConfig(QString pathfile) {
        if (!QFile::exists(pathfile)) return nullptr;
        QFile file(pathfile);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return nullptr;

        QString lines = file.readAll();
        file.close();
        QJsonDocument doc = QJsonDocument::fromJson(lines.toUtf8());
        if (!doc.isEmpty()) {
            QJsonObject obj = doc.object();
            Settings* settings = new Settings();
            bool retOK = false, highlightingsOK = false;

            for (QJsonObject::iterator itemSettings=obj.begin();itemSettings!=obj.end();++itemSettings) {
                if (itemSettings.key() == QString("font") && itemSettings.value().isObject()) {
                    QJsonObject fontObj = itemSettings.value().toObject();
                    for (QJsonObject::iterator fontSettings=fontObj.begin();fontSettings!=fontObj.end();++fontSettings) {
                        if (fontSettings.key() == QString("fontFamily")) {
                            settings->fontSettings.fontFamily = fontSettings.value().toString();
                        } else if (fontSettings.key() == QString("fontSize")) {
                            settings->fontSettings.fontSize = fontSettings.value().toInt();
                        } else if (fontSettings.key() == QString("highlightings") && fontSettings.value().isObject()) {
                            QJsonObject highlight = fontSettings.value().toObject();
                            for (QJsonObject::iterator itemHighlight=highlight.begin();itemHighlight!=highlight.end();++itemHighlight) {
                                QString key = itemHighlight.key();
                                QJsonObject value = highlight.value(key).toObject();
                                bool def = value.value(QString("defaultColor")).toBool();
                                int weight = value.value(QString("weight")).toInt();
                                QString color = value.value(QString("color")).toString();
                                QString rawRegex = value.value(QString("regex")).toString();
                                QString keywords = value.value(QString("keywords")).toString();
                                KeywordHighlight* keyword = nullptr;
                                if (def)
                                    keyword = new KeywordHighlight(color, weight, def);
                                else if (!rawRegex.isEmpty())
                                    keyword = new KeywordHighlight(color, QRegExp(rawRegex), weight);
                                else
                                    keyword = new KeywordHighlight(color, keywords, weight);
                                if (keyword) {
                                    settings->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>(key, *keyword));
                                    if (!highlightingsOK) highlightingsOK = true;
                                }
                            }
                        }
                    }
                }
            }
            retOK = highlightingsOK;

            if (retOK) {
                settings->fileSourcePath = pathfile;
                return settings;
            }
        }
        return nullptr;

    }
    void writeConfig() {
        writeConfig(this->fileSourcePath);
    }
    void writeConfig(QString pathfile) {
        QJsonObject highlight;
        QString keywordsList;
        for (map<QString, KeywordHighlight>::iterator it=this->fontSettings.highlightings.begin();it!=this->fontSettings.highlightings.end();++it) {
            keywordsList = "";
            if (!it->second.keywords.isEmpty()) {
                keywordsList = it->second.keywords.at(0);
                for (int i=1;i<it->second.keywords.count();i++) {
                    keywordsList += "|" + it->second.keywords.at(i);
                }
            }
            QJsonObject keyword{
                {"defaultColor", it->second.defaultColor},
                {"weight", it->second.weight},
                {"color", it->second.color},
                {"regex", it->second.regex.isEmpty() ? "" : it->second.regex.pattern()},
                    {"keywords", keywordsList}
                };
                highlight.insert(it->first, keyword);
            }
        QJsonDocument doc(
                    QJsonObject{
                        {"font", QJsonObject{
                             {"fontFamily", this->fontSettings.fontFamily},
                             {"fontSize",  (int)this->fontSettings.fontSize},
                             {"highlightings", highlight}
                         }
                        },

                    }
                    );

        QFile file(pathfile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
    void init() {
        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Code",KeywordHighlight("#F926F2",true)));
        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Strings",KeywordHighlight("#F2DF17",QRegExp("(\".+\"|\'.+\')"))));
        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Numbers",KeywordHighlight("#792692",QRegExp("(true|false|\\d+)"))));
        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Comments",KeywordHighlight("#75715E",QRegExp("(--[^\\n]*)"))));

        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Lua_L1",KeywordHighlight("#092672","break|do|else|elseif|end|for|function|if|in|local|repeat|return|then|until|while|or|and|not|math|table|string", '|', false)));
        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Lua_L2",KeywordHighlight("#F92672","tostring|next|tonumber|type|pairs|ipairs|abs|acos|asin|abs|atan|ceil|cos|deg|exp|floor|fmod|huge|max|maxinteger|min|mininteger|modf|pi|rad|random|randomseed|sin|sqrt|tan|tointeger|type|ult|concat|insert|move|pack|remove|sort|unpack|byte|char|dump|find|format|gmatch|gsub|len|lower|match|packsize|reverse|sub|upper", '|', false)));
        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Stormworks_L1",KeywordHighlight("#6609EF","screen|map|input|output|property", '|', false)));
        this->fontSettings.highlightings.insert(pair<QString, KeywordHighlight>("Stormworks_L2",KeywordHighlight("#66D9EF","getWidth|getHeight|setColor|drawClear|drawLine|drawCircle|drawCircleF|drawRect|drawRectF|drawTriangle|drawTriangleF|drawText|drawTextBox|drawMap|setMapColorOcean|setMapColorShallows|setMapColorLand|setMapColorGrass|setMapColorSand|setMapColorSnow|screenToMap|mapToScreen|getBool|getNumber|setBool|setNumber|getText", '|', false)));
    }
    bool operator==(const Settings &other) {
        if (this->fileSourcePath != other.fileSourcePath) return false;
        if (this->fontSettings.highlightings != other.fontSettings.highlightings) return false;
        return true;
    }
    friend bool operator==(const Settings &left, const Settings &right) {
        if (left.fileSourcePath != right.fileSourcePath) return false;
        if (left.fontSettings.highlightings != right.fontSettings.highlightings) return false;
        return true;
    }
    bool operator!=(const Settings &other) { return !operator==(other); }
    friend bool operator!=(const Settings &left, const Settings &right) { return !(left==right); }
};
#endif // SETTINGS_H

/*
{
  "highlightings" : {
    "Code" : {
      "defaultColor" : true|false,
      "color" : "<string>",
      "regex" : "<string>",
      "keywords" : "<string>"
    },
    {...}
  }
}
*/
