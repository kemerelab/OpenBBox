#include "event.h"

Event::Event(QString stateName, QList<QString> lines) :
    QObject()
{
    this->stateName = stateName;
    printf("State name: %s\n",stateName.toAscii().data());
    this->lines = lines;
    this->type = TYPE_EVENT_NORMAL;

    this->input = this->lines.at(0).section(":", 0, 0);

    for(typeInput = 0; typeInput < sizeof(inputs_keys); typeInput++){
        if(input.contains(inputs_keys[typeInput])){
            printf("Type input: %s\n",inputs_keys[typeInput].toAscii().data());
            break;
        }
    }

    this->nextState = this->lines.at(0).section("--->",1,1);
    printf("Input: %s\n", input.toAscii().data());
    printf("NextState: %s\n", nextState.toAscii().data());

    //Find type
    if(lines.size() > 1){
        this->type = TYPE_EVENT_WITH_IF;
    }else if((lines.size() == 1) && lines.at(0).contains("IF") && lines.at(0).contains("[") && lines.at(0).contains("]")){
        this->type = TYPE_EVENT_WITH_IF;
    }else{
        this->type = TYPE_EVENT_NORMAL;
    }
    printf("Type: %d\n", type);

    for(int i = 0; i < lines.size(); i++){
            printf("%d: %s\n", i+1,lines.at(i).toAscii().data());
    }

    QString line;
    switch (type) {
        case TYPE_EVENT_NORMAL:
                line =  lines.at(0).section(":", 1, 1);
                line.insert(line.indexOf("--->"),";");
                for(int j = 0; j < line.count(';'); j++){
                    if(!line.section(";",j,j).isEmpty()){
                        outputs.push_back(line.section(";",j,j));
                        printf("%d: %s\n", j+1,line.section(";",j,j).toAscii().data());
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
                    printf("%d: %s\n", j+1,line.section(";",j,j).toAscii().data());
                }

                for(int j = 0; j < lines.size(); j++) {
                    if(j == 0){
                        line =  lines.at(0).section(":", 1, 1);
                        line =  line.section(";", line.count(";") - line.section("[",1,1).count(";"), line.count(";"));
                        IFStatements.insert("@initial", line);
                        //fprintf(stderr, "Not contain If statements: %s  - %s\n", "@initial", line.toAscii().data());
                    } else if(lines.at(j).contains("@")) {
                        IFStatements.insert(lines.at(j).section(":", 0, 0).remove('\t'),lines.at(j).section(":", 1, 1).remove(" "));
                    }else {
                        fprintf(stderr,"Error decomposing if: %s\n", lines.at(j).toAscii().data());
                        fflush(stderr);
                    }
                }
            break;
        default:
            break;
    }

    printf("\n");

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
    printf("Command: %s\n", command.toAscii().data());
    fflush(stdout);
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
        fprintf(stderr, "Executing IF: %s = %s\n", condition.toAscii().data(), result ? "True" : "False");
        return result;
}

bool Event::initEvent(Context * context) {

    initTime = context->getSystemTime();
    //printf("InitTime = %ld\n", initTime);

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
                if(p1 != "") {
                    cntAux = 1;
                } else {
                    cntAux = context->getValue(p1);
                }
            break;
            case TYPE_INPUT_ZPULSE:
                p1 = input.section("#Z", 0, 0);
                p2 = input.section("#Z", 1, 1);
            break;
            default:
                fprintf(stderr, "Input event not found by interpreter\n");
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
                            cntAux = 1;
                        } else {
                            cntAux = context->getValue(p1);
                        }
                        executeCommands = true;
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
                return false;
                fprintf(stderr, "Input event not found by interpreter\n");
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
                                    //fprintf(stderr, "Executing IF: %s\n", IFStatements.value(key).section("IF",1,1).section("[", 0, 0).toAscii().data());
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
                                //fprintf(stderr, "Not contain If statements: %s  - %s\n", key.toAscii().data(), IFStatements.value(key).toAscii().data());
                                this->nextState = IFStatements.value(key).section("--->", 1, 1);
                                stopIF = true;
                            }
                        }else{
                            fprintf(stderr, "If statement key not found: %s\n", key.toAscii().data());
                            QList<QString> keys = IFStatements.keys();
                             for(int i = 0; i < keys.size(); i++) {
                                 fprintf(stderr, "Possible key: %s\n", keys.at(i).toAscii().data());
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
