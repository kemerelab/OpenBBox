#include "event.h"

Event::Event(QString stateName, QList<QString> lines) :
    QObject()
{
    this->stateName = stateName;
    qDebug("State name: %s", qPrintable(stateName));
    this->lines = lines;
    this->type = TYPE_EVENT_NORMAL;

    this->input = this->lines.at(0).section(":", 0, 0);

    for(typeInput = 0; typeInput < sizeof(inputs_keys); typeInput++){
        if(input.contains(inputs_keys[typeInput])){
            qDebug("Type input: %s", qPrintable(inputs_keys[typeInput]));
            break;
        }
    }

    this->nextState = this->lines.at(0).section("--->",1,1);
    qDebug("Input: %s", qPrintable(input));
    qDebug("NextState: %s", qPrintable(nextState));

    //Find type
    if(lines.size() > 1){
        this->type = TYPE_EVENT_WITH_IF;
    }else if((lines.size() == 1) && lines.at(0).contains("IF") && lines.at(0).contains("[") && lines.at(0).contains("]")){
        this->type = TYPE_EVENT_WITH_IF;
    }else{
        this->type = TYPE_EVENT_NORMAL;
    }
    qDebug("Type: %d", type);

    for(int i = 0; i < lines.size(); i++){
            qDebug("%d: %s", i+1, qPrintable(lines.at(i)));
    }

    QString line;
    switch (type) {
        case TYPE_EVENT_NORMAL:
                line =  lines.at(0).section(":", 1, 1);
                line.insert(line.indexOf("--->"),";");
                for(int j = 0; j < line.count(';'); j++){
                    if(!line.section(";",j,j).isEmpty()){
                        outputs.push_back(line.section(";",j,j));
                        qDebug("%d: %s", j+1, qPrintable(line.section(";",j,j)));
                    }
                }
            break;
        case TYPE_EVENT_WITH_IF:
                //get any operation before IF statement
                line =  lines.at(0).section(":", 1, 1);
                if(line.contains("--->")){
                   line = line.section("IF", 0, 0);
                }
                for(int j = 0; j < line.count(';'); j++){
                    outputs.push_back(line.section(";",j,j));
                    qDebug("%d: %s", j+1, qPrintable(line.section(";",j,j)));
                }

                for(int j = 0; j < lines.size(); j++) {
                    if(j == 0){
                        line =  lines.at(0).section(":", 1, 1);
                        line =  line.section(";", line.count(";") - line.section("[",1,1).count(";"), line.count(";"));
                        IFStatements.insert("@initial", line);
                        //qCritical( "Not contain If statements: %s  - %s", "@initial", line));
                    } else if(lines.at(j).contains("@")) {
                        IFStatements.insert(lines.at(j).section(":", 0, 0).remove('\t'),lines.at(j).section(":", 1, 1).remove(" "));
                    }else {
                        qCritical("Error decomposing if: %s", qPrintable(lines.at(j)));

                    }
                }
            break;
        default:
            break;
    }

    qDebug(" ");

    this->cntAux = 0;
}

QString Event::getState() {
    return this->stateName;
}

QString Event::getNextState() {
    if(nextState.contains("SX"))
        return this->stateName;
    else
        return this->nextState;
}

#define OFF 0
#define ON  1

void Event::executeCommand(QString command, Context * context) {
    qDebug("Command: %s", qPrintable(command));
    context->executeCommand(command);
}

QStringList getArgs(QString exp){

    QStringList list;
    int level = 0;
    QString aux;
    for(int i = 0; i < exp.size(); i++){
        if(exp.at(i) == '('){
            level++;
            if(level == 1){
                aux.clear();
            }else{
                aux.push_back(exp.at(i));
            }
        }
        else if(exp.at(i) == ')'){
            level--;
            if(level == 0){
                list.push_back(aux);
            }else{
                aux.push_back(exp.at(i));
            }
        }else{
                aux.push_back(exp.at(i));
        }
    }
    return list;
}

QStringList getLogicArgs(QString exp){
    QStringList list = getArgs(exp);

    for(int i = 0; i < list.size(); i++){
        exp = exp.remove(list.at(i));
    }

    QStringList test = exp.split("()");
    test.removeAll("");
    return test;
}

bool Event::executeIF(QString condition, Context * context) {
        bool result = context->executeIF(condition);
        qCritical( "Executing IF: %s = %s", qPrintable(condition), result ? "True" : "False");
        return result;
}

bool Event::initEvent(Context * context) {

    initTime = context->getSystemTime().tv_sec*1000LL + context->getSystemTime().tv_usec/1000;

    switch(typeInput) {
            case TYPE_INPUT_START:
                p1 = input.section(inputs_keys[TYPE_INPUT_START], 0, 0);
                if(p1 != "") {
                    cntAux = 0;
                }else{
                    cntAux = context->getValue(p1);
                }
            break;
            case TYPE_INPUT_TIME_SEC:
                p1 = input.section(inputs_keys[TYPE_INPUT_TIME_SEC], 0, 0);
            break;
            case TYPE_INPUT_TIME_MIN:
                p1 = input.section(inputs_keys[TYPE_INPUT_TIME_MIN], 0, 0);
            break;
            case TYPE_INPUT_RESPONSE:
                p1 = input.section(inputs_keys[TYPE_INPUT_RESPONSE], 0, 0);
                p2 = input.section(inputs_keys[TYPE_INPUT_RESPONSE], 1, 1);
                context->getPinExpected()->append(context->getValue(p2));
                if(p1 != "") {
                    cntAux = context->getValue(p1);
                } else {
                    cntAux = 1;
                }
            break;
            case TYPE_INPUT_ZPULSE:
                p1 = input.section(inputs_keys[TYPE_INPUT_ZPULSE], 0, 0);
                p2 = input.section(inputs_keys[TYPE_INPUT_ZPULSE], 1, 1);
            break;
            default:
                qCritical( "Input event not found by interpreter");
                return false;
            break;
    }

    executeCommands = false;
    return true;
}

bool Event::updateEvent(Context * context) {

     switch(typeInput) {
            case TYPE_INPUT_START:
                this->cntAux--;
                if(this->cntAux <= 0) {
                    if(p1 != "") {
                        cntAux = 1;
                    } else {
                        cntAux = context->getValue(p1);
                    }
                    executeCommands = true;
                }
            break;
            case TYPE_INPUT_TIME_SEC:
                if(context->isTimePassed(initTime, context->getValue(p1) * 1000)){
                    executeCommands = true;
                }
            break;
            case TYPE_INPUT_TIME_MIN:
                if(context->isTimePassed(initTime, context->getValue(p1) * 1000 * 60)){
                     executeCommands = true;
                }
            break;
            case TYPE_INPUT_RESPONSE:
                if(context->isEventHappened((int)context->getValue(p2))){
                    this->cntAux--;
                    if(this->cntAux <= 0) {
                        if(p1 != "") {
                            cntAux = context->getValue(p1);
                        } else {
                            cntAux = 1;
                        }
                        executeCommands = true;
                        qDebug("pinexpected size: %d", context->getPinExpected()->size());
                        context->clearPinExpected();
                    }
                }
            break;
            case TYPE_INPUT_ZPULSE:
                if(context->isZPulseRequested((int)context->getValue(p2))){
                    this->cntAux--;
                    if(this->cntAux <= 0) {
                        if(p1 != "") {
                            cntAux = 1;
                        } else {
                            cntAux = context->getValue(p1);
                        }
                        executeCommands = true;
                    }
                }
            break;
            default:
                qCritical( "Input event not found by interpreter");
                return false;
            break;
    }

    QString line;
    QString key = "@initial";
    bool stopIF = false;
    if(executeCommands) {
       switch (type) {
            case TYPE_EVENT_NORMAL:
                    for(int i = 0; i < outputs.size(); i++) {
                        executeCommand(outputs.at(i), context);
                    }
                    return true;
                break;
            case TYPE_EVENT_WITH_IF:
                    //execute first commands
                    for(int i = 0; i < outputs.size(); i++) {
                        executeCommand(outputs.at(i), context);
                    }

                    key = "@initial";
                    stopIF = false;

                    while(!stopIF) {
                        line =  IFStatements.value(key);

                        if(line.contains("--->") && line.contains("[") && line.contains("]")){ //in case of syntax 3 of if, dont execute commands in the line
                            line = line.section("IF", 0, 0);
                        }

                        //execute all commands
                        for(int j = 0; j < line.count(';'); j++) {
                            if(line.section(";",j,j) != "" && !line.section(";",j,j).isEmpty()){
                                executeCommand(line.section(";",j,j), context);
                            }
                        }

                        //Execute if statements if the string have
                        if(IFStatements.contains(key)){
                            if(IFStatements.value(key).contains("IF") && IFStatements.value(key).contains("[") && IFStatements.value(key).contains("]")){
                                if(executeIF(IFStatements.value(key).section("IF",1,1).section("[", 0, 0), context)){
                                    //qCritical( "Executing IF: %s", IFStatements.value(key).section("IF",1,1).section("[", 0, 0)));
                                    if(IFStatements.value(key).count("@") == 2){
                                        line = IFStatements.value(key);
                                        line = line.mid(line.indexOf("@"), line.lastIndexOf(",") - line.indexOf("@"));
                                        key = line;
                                    }else if(IFStatements.value(key).count("@") == 1){
                                        line = IFStatements.value(key).section("[", 1, 1);
                                        line = line.section("]",0,0);
                                        key = line;
                                    }else if(IFStatements.value(key).count("@") == 0){
                                        //execute inside []
                                        line = IFStatements.value(key).section("[", 1, 1);
                                        line = line.section("]",0,0);

                                        //execute all commands
                                        for(int j = 0; j < line.count(';'); j++) {
                                            if(line.section(";",j,j) != "" && !line.section(";",j,j).isEmpty()){
                                                executeCommand(line.section(";",j,j), context);
                                            }
                                        }

                                        this->nextState = IFStatements.value(key).section("--->", 1, 1);
                                        stopIF = true;
                                   }
                                } else {
                                    if(IFStatements.value(key).count("@") == 2){
                                        line = IFStatements.value(key);
                                        line = line.mid(line.lastIndexOf("@"), line.lastIndexOf("]") - line.lastIndexOf("@"));
                                        key = line;
                                    }else if(IFStatements.value(key).count("@") == 1){
                                        this->nextState = "SX";
                                        stopIF = true;
                                    }else if(IFStatements.value(key).count("@") == 0){
                                        this->nextState = "SX";
                                        stopIF = true;
                                   }
                                }
                            }else if(IFStatements.value(key).contains("--->")){
                                //qCritical( "Not contain If statements: %s  - %s", key), IFStatements.value(key)));
                                this->nextState = IFStatements.value(key).section("--->", 1, 1);
                                stopIF = true;
                            }
                        }else{
                            qCritical( "If statement key not found: %s", qPrintable(key));
                            QList<QString> keys = IFStatements.keys();
                            for(int i = 0; i < keys.size(); i++) {
                                qCritical( "Possible key: %s", qPrintable(keys.at(i)));
                            }
                            return false;
                        }
                    }
                   return true;
                break;
            default:
                break;
        }
    }

    return false;
}
