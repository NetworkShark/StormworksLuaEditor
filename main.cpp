#include "mainwindow.h"
#include "structs.h"

#include <QApplication>

using namespace std;

Settings globalSettings;

void initSettings();
int main(int argc, char *argv[])
{
    initSettings();

    QApplication a(argc, argv);
    MainWindow w;
    w.globalSettings = &globalSettings;

    w.show();
    return a.exec();
}

void initSettings() {
    globalSettings = Settings();
    map<QString, KeywordHighlight> highlights;
    highlights.insert(pair<QString, KeywordHighlight>("Code",KeywordHighlight("#F926F2",true)));
    highlights.insert(pair<QString, KeywordHighlight>("Strings",KeywordHighlight("#F2DF17",QRegExp("(\".+\"|\'.+\')"))));
    highlights.insert(pair<QString, KeywordHighlight>("Numbers",KeywordHighlight("#792692",QRegExp("(true|false|\\d+)"))));
    highlights.insert(pair<QString, KeywordHighlight>("Comments",KeywordHighlight("#75715E",QRegExp("(--[^\\n]*)"))));
    highlights.insert(pair<QString, KeywordHighlight>("Lua_L1",KeywordHighlight("#092672","break|do|else|elseif|end|for|function|if|in|local|repeat|return|then|until|while|or|and|not|math|table|string")));
    highlights.insert(pair<QString, KeywordHighlight>("Lua_L2",KeywordHighlight("#F92672","tostring|next|tonumber|type|pairs|ipairs|abs|acos|asin|abs|atan|ceil|cos|deg|exp|floor|fmod|huge|max|maxinteger|min|mininteger|modf|pi|rad|random|randomseed|sin|sqrt|tan|tointeger|type|ult|concat|insert|move|pack|remove|sort|unpack|byte|char|dump|find|format|gmatch|gsub|len|lower|match|packsize|reverse|sub|upper")));
    highlights.insert(pair<QString, KeywordHighlight>("Stormworks_L1",KeywordHighlight("#6609EF","screen|map|input|output|property")));
    highlights.insert(pair<QString, KeywordHighlight>("Stormworks_L2",KeywordHighlight("#66D9EF","getWidth|getHeight|setColor|drawClear|drawLine|drawCircle|drawCircleF|drawRect|drawRectF|drawTriangle|drawTriangleF|drawText|drawTextBox|drawMap|setMapColorOcean|setMapColorShallows|setMapColorLand|setMapColorGrass|setMapColorSand|setMapColorSnow|screenToMap|mapToScreen|getBool|getNumber|setBool|setNumber|getText")));
    globalSettings.highlightings = highlights;
}
