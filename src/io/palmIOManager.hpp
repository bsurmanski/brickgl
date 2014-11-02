#ifndef _PALMIOMANAGER_HPP
#define _PALMIOMANAGER_HPP

#include "brickIOManager.hpp"

#include <iostream>
#include <fstream>

enum {
    PALM_INVALID,
    PALM_LPAREN,
    PALM_RPAREN,
    PALM_WORD,
    PALM_NUMBER,
};

//TODO: use a token pool?
class PalmToken {
    int refcount;

    PalmToken(int type);
    ~PalmToken();

    public:
    int type;

    union {
        double number;
        char *word;
    };

    static PalmToken *newLParen();
    static PalmToken *newRParen();
    static PalmToken *newWord(char *word);
    static PalmToken *newNumber(double number);

    double getNumber();
    char *getWord();

    int retain();
    int release();
};

class PalmLexer {
    std::ifstream &file;
    PalmToken *next;

    public:
    PalmLexer(std::ifstream &f) : file(f), next(0) {}
    PalmToken *lexToken(std::ifstream &file);

    bool eof() {
        return file.eof();
    }

    PalmToken *peek() {
        if(!next) next = lexToken(file);
        return next;
    }

    PalmToken *get() {
        PalmToken *ret;
        if(next) {
            ret = next;
            next = NULL;
        } else {
            ret = lexToken(file);
        }
        return ret;
    }

    void ignore() { next = NULL; }
};

class PalmListElement;
class PalmStringElement;
class PalmNumberElement;

class PalmElement {
    int refcount;
    PalmElement *next; // for lists

    public:
    PalmElement() : refcount(1), next(0) {}
    int retain();
    int release();
    void setNext(PalmElement *elem);
    PalmElement *getNext();
    virtual ~PalmElement(){}

    virtual bool isList() { return asList(); }
    virtual bool isString() { return asString(); }
    virtual bool isNumber() { return asNumber(); }

    virtual PalmListElement *asList() { return NULL; }
    virtual PalmStringElement *asString() { return NULL; }
    virtual PalmNumberElement *asNumber() { return NULL; }
};

class PalmListElement : public PalmElement {
    PalmElement *first;
    public:

    PalmListElement(PalmElement *f=NULL) : first(f) {}
    PalmElement *getFirst() { return first; }
    PalmElement *getElement(unsigned i) {
        PalmElement *e = first;

        while(e && i) {
            e = e->getNext();
            i--;
        }
        return e;
    }
    virtual PalmListElement *asList() { return this; }
    std::string getName(); // actually string in first element (if exists)
};

class PalmStringElement : public PalmElement {
    std::string value;
    public:
    PalmStringElement(std::string val) : value(val) {}
    virtual PalmStringElement *asString() { return this; }
    std::string getValue() { return value; }
};

class PalmNumberElement : public PalmElement {
    double value;
    public:
    PalmNumberElement(double val) : value(val) {}
    virtual PalmNumberElement *asNumber() { return this; }
    double getValue() { return value; }
};

class PalmIOManager : BrickIOManager {
    int level;
    PalmLexer *lexer;

    PalmStringElement *elementAsString(PalmElement *p) {
        if(p) return p->asString();
        return NULL;
    }

    protected:

    PalmElement *parseElement(PalmLexer *lex);
    vec4 inputVector(PalmListElement *list);
    void inputBrick(PalmListElement *list, MainApplication *app);
    void inputCamera(PalmListElement *list, Camera *cam);
    void inputTopLevel(PalmListElement *list, MainApplication *app);

    void outputCamera(std::ofstream &file, Camera *cam);
    void outputBrick(std::ofstream &file, Brick *brick);
    void outputBricks(std::ofstream &file, MainApplication *app);
    void outputVec4(std::ofstream &file, std::string label, vec4 pos);
    void indentLevel(std::ofstream &file);
    public:
    PalmIOManager() : level(0), lexer(0) {}
    virtual void load(MainApplication *app, std::string filenm);
    virtual void save(MainApplication *app, std::string filenm);
};

#endif
