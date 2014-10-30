#include "palmIOManager.hpp"

#include <string.h>

/*
 * PalmToken
 */

PalmToken *PalmToken::newLParen() {
    PalmToken *ret = new PalmToken(PALM_LPAREN);
    return ret;
}

PalmToken *PalmToken::newRParen() {
    PalmToken *ret = new PalmToken(PALM_RPAREN);
    return ret;
}

PalmToken *PalmToken::newWord(char *word) {
    PalmToken *ret = new PalmToken(PALM_WORD);
    ret->word = strndup(word, 256);
    return ret;
}

PalmToken *PalmToken::newNumber(double number) {
    PalmToken *ret = new PalmToken(PALM_NUMBER);
    ret->number = number;
    return ret;
}


PalmToken::PalmToken(int _type) {
    refcount = 1;
    type = _type;
    word = 0;
}

PalmToken::~PalmToken() {
    if(word) {
        free(word);
    }
}

int PalmToken::retain() {
    refcount++;
    return refcount;
}

int PalmToken::release() {
    refcount--;
    if(refcount < 0) {
        delete this;
        return 0;
    }
    return refcount;
}

double PalmToken::getNumber() {
    if(type == PALM_NUMBER) {
        return number;
    }
    return 0.0;
}

char *PalmToken::getWord() {
    if(type == PALM_WORD) {
        return word;
    }
    return NULL;
}

/*
 * PalmLexer
 */

PalmToken *PalmLexer::lexToken(std::ifstream &file) {
    if(file.eof()) return NULL;

    int pk = file.peek();
    while(pk == '\n' || pk == ' ') {
        file.get();
        pk = file.peek();
    }

    if(pk == '(') {
        file.get();
        return PalmToken::newLParen();
    }

    if(pk == ')') {
        file.get();
        return PalmToken::newRParen();
    }

    char buf[256];
    int i = 0;
    while(pk != '(' && pk != ')' && pk != ' ' && pk != '\n') {
        file.get();
        buf[i] = pk;
        i++;
        pk = file.peek();
    }
    buf[i] = '\0';

    if(isdigit(buf[0]) || buf[0] == '-') {
        bool frac = false;
        for(int j = 1; j < i; j++) {
            if(buf[j] == '.') {
                if(frac) goto INVALID_NUM;
                frac = true;
            }
            if(!isdigit(buf[j]) && buf[j] != '.') {
                //ERROR: invalid number
INVALID_NUM:
                return NULL;
            }
        }

        return PalmToken::newNumber(atof(buf));
    }

    return PalmToken::newWord(buf);
}

/*
 * PalmElement
 */


int PalmElement::retain() {
    return ++refcount;
}

int PalmElement::release() {
    refcount--;
    if(refcount < 0) {
        delete this;
        return 0;
    }
    return refcount;
}

void PalmElement::setNext(PalmElement *elem) {
    //if(next) next->release();
    //if(elem) elem->retain();
    next = elem;
}
PalmElement *PalmElement::getNext() {
    return next;
}

std::string PalmListElement::getName() {
    PalmStringElement *selem = first->asString();
    if(selem) return selem->getValue();
    return "";
}

/*
 * PalmIOManager
 */

// INPUT

PalmElement *PalmIOManager::parseElement(PalmLexer *lex) {
    PalmToken *pk = lex->peek();
    if(!pk) return NULL;

    if(pk->type == PALM_LPAREN) {
        lex->ignore(); // ignore (

        PalmListElement *list = new PalmListElement(parseElement(lex));
        PalmElement *tail = list->getFirst();
        while(tail && lex->peek()->type != PALM_RPAREN) {
            tail->setNext(parseElement(lex));
            tail = tail->getNext();
        }

        if(lex->peek()->type != PALM_RPAREN) {
            printf("ERROR: list without closing rparen\n");
            //ERROR
        }
        lex->ignore(); //ignore )

        return list;
    }

    if(pk->type == PALM_NUMBER) {
        return new PalmNumberElement(lex->get()->getNumber());
    }

    if(pk->type == PALM_WORD) {
        char *str = lex->get()->getWord();
        return new PalmStringElement(str);
    }

    return NULL;
}

vec4 PalmIOManager::inputVector(PalmListElement *list) {
    PalmElement *it = list->getFirst();
    it = it->getNext();

    vec4 ret;
    for(int i = 0; i < 4 && it; i++, it = it->getNext()) {
        if(it->isNumber()) {
            ret.v[i] = (float) it->asNumber()->getValue();
        }
    }

    return ret;
}

void PalmIOManager::inputCamera(PalmListElement *list, Camera *cam) {
    PalmElement *it = list->getFirst();
    if(it->asString() && it->asString()->getValue() != "camera") {
        // ERROR invalid list passed
    }


    it = it->getNext(); //skip name

    while(it) {
        if(it && it->asList()) {
            PalmListElement *itl = it->asList();
            if(itl->getName() == "position") {
                cam->setPosition(inputVector(itl));
            } else if(itl->getName() == "offset") {
                cam->setOffset(inputVector(itl));
            } else if(itl->getName() == "rotation") {
                cam->setRotation(inputVector(itl));
            }
        }

        it = it->getNext();
    }
}

void PalmIOManager::inputTopLevel(PalmListElement *list, MainApplication *app) {
    PalmElement *it = list->getFirst();
    if(!it->isString() || it->asString()->getValue() != "brickgl") {
        //ERROR: not top level
        printf("invalid top level\n");
        return;
    }
    it = it->getNext();

    while(it) {
        PalmListElement *itl = it->asList();
        if(itl) {
            printf("itl: %s\n", itl->getName().c_str());
            if(itl->getName() == "camera") {
                inputCamera(itl, app->getCamera());
            }
        }

        it = it->getNext();
    }
}

void PalmIOManager::load(MainApplication *app, std::string filenm) {
    std::ifstream file;
    file.open(filenm.c_str(), std::ios::in);
    PalmLexer lex(file);

    PalmElement *toplevel = parseElement(&lex);
    if(PalmListElement *brickelem = toplevel->asList()) {
        if(brickelem->getName() == "brickgl") {
            inputTopLevel(brickelem, app);
        }
    }
}

void PalmIOManager::indentLevel(std::ofstream &file) {
    for(int i = 0; i < level; i++) {
        file << "    "; //use spaces
    }
}

// OUTPUT

void PalmIOManager::outputCamera(std::ofstream &file, Camera *cam) {
    level++;
    indentLevel(file);
    file << "(camera ";
    outputVec4(file, "position", cam->getPosition());
    file << " ";
    outputVec4(file, "offset", cam->getOffset());
    file << " ";
    outputVec4(file, "rotation", cam->getRotation());
    file << ")\n";
    level--;
}


void PalmIOManager::outputBricks(std::ofstream &file, MainApplication *app) {

}

void PalmIOManager::outputVec4(std::ofstream &file, std::string label, vec4 pos) {
    file << "(" << label << " " << pos.x << " " << pos.y << " " << pos.z << " " << pos.w << ")";
}

void PalmIOManager::save(MainApplication *app, std::string filenm) {
    std::ofstream file;
    file.open(filenm.c_str(), std::ios::out);
    level = 0;

    file << "(brickgl\n";
    outputCamera(file, app->getCamera());
    outputBricks(file, app);
    file << ")\n";
    file.close();
}
