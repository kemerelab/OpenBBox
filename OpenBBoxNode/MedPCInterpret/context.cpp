#include "context.h"

Context::Context(const uint * gpioInputs, const uint * gpioOutputs)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    this->gpioInputs = gpioInputs;
    this->gpioOutputs = gpioOutputs;

    int i = 0;
    for(i = 0; i < NUM_OUTPUTS; i++) {
        GPIO::gpio_export(gpioOutputs[i]);
        GPIO::gpio_set_dir(gpioOutputs[i], 1);
        GPIO::gpio_set_value(gpioOutputs[i], 0);
    }

}

QHash<QString, int>          *Context::getConstants(){
    return &constsMap;
}

QHash<QString, float  >      *Context::getVariables(){
    return &variablesMap;
}

QHash<QString, QVector<float>* >      *Context::getArrays(){
    return &arraysMap;
}

QHash<int, bool>       *Context::getZPulses(){
    return &zpulseMap;
}

QList<int> *Context::getInputsEvents(){
    return &inputsEvents;
}

bool Context::setValue(QString var, float value) {
    var = var.replace(";","");
    var = var.replace(" ","");
    var = var.toUpper();

    if(var.contains(ARRAY_LIST_DELIMITER_BEGIN) && var.at(var.size()-1) == ARRAY_LIST_DELIMITER_END) {
        QString vararray = var.section(ARRAY_LIST_DELIMITER_BEGIN, 0 , 0);
        QString arg = var.remove(0, var.indexOf(ARRAY_LIST_DELIMITER_BEGIN)+1);
        arg = arg.remove(arg.lastIndexOf(ARRAY_LIST_DELIMITER_END), arg.size());

        if(arraysMap.contains(vararray)){
            if(arraysMap.value(vararray)->size() > (int)getValue(arg)){
                arraysMap.value(vararray)->replace((int)getValue(arg), value);
                return true;
            }
        }
    }else{
        if(variablesMap.contains(var)) {
             variablesMap[var] = value;
             return true;
        }
    }
    return false;
}

int randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

bool Context::addVarIfNotExists(QString var) {

    if(var.contains("^")){
        if(constsMap.contains(var))
            return true;
    }else
    if(var.contains("(")){
        if(arraysMap.contains(var.section("(",0,0)))
            return true;
    }else
    if(variablesMap.contains(var)){
        return true;
    }

    variablesMap.insert(var, 0);
    return false;
}

float Context::getValue(QString var) {
    var = var.replace(";","");
    var = var.replace(" ","");

    bool isNumber = false;
    float ret = var.toFloat(&isNumber);
    if(isNumber)    // optimazing - if is just a number return the number
        return ret;

    if(variablesMap.contains(var)){ //optimazing - if is just a normal variable
        return variablesMap.value(var);
    }else if(constsMap.contains(var)){
        return constsMap.value(var);
    }

    var = var.replace("^","_");
    var = var.toUpper();

    QString functionString("(function() { ");

    QList<QString> constKeys = constsMap.keys();
    for(int i = 0; i < constKeys.size(); i++) {
        QString line = constKeys.at(i);
        line = line.replace("^","_");
        functionString.push_back("var "+line+" = "+QString::number(constsMap.value(constKeys.at(i)))+";");
    }

    QList<QString> varKeys = variablesMap.keys();
    for(int i = 0; i < varKeys.size(); i++) {
        QString line = varKeys.at(i);
        functionString.push_back("var "+line+" = "+QString::number(variablesMap.value(varKeys.at(i)))+";");
    }

    QList<QString> listKeys = arraysMap.keys();
    //put all arrays
    for(int i = 0; i < listKeys.size(); i++) {
        QVector<float> * array = arraysMap.value(listKeys.at(i));
        QString arrayString = "";
        int j = 0;
        for(j = 0; j < array->size() - 1; j++) {
            arrayString.push_back(QString::number(array->at(j))+",");
        }
        arrayString.push_back(QString::number(array->at(j)));

        QString newVar("var "+listKeys.at(i)+" = ["+arrayString+"];");

        functionString.push_back(newVar);
    }

    if(var.contains("(") && var.contains(")")){ //optimizing
        QString exp = var;
        QString aux = "";
        exp.push_front("(");
        int level = 0;
        for(int i = 1; i < exp.size(); i++) {
            if(exp.at(i-1) != '(' && exp.at(i-1) != '+' && exp.at(i-1) != '-' && exp.at(i-1) != '*' && exp.at(i-1) != '/' && exp.at(i) == '(') {
                aux.push_back("[");
                level++;
            } else if(exp.at(i) == ')' && level!=0) {
                level--;
                aux.push_back("]");
            }else{
                 aux.push_back(exp.at(i));
            }
        }

        functionString.push_back("return "+ aux + "})");
    }else{
        functionString.push_back("return "+ var + "})");
    }
    QScriptEngine myEngine;
    QScriptValue fun = myEngine.evaluate(functionString);
    QScriptValueList args;
    QScriptValue threeAgain = fun.call(QScriptValue(), args);
    if(threeAgain.isError()){
        //error
        qCritical("Value not found");
        return 0;
    }

    return (float)threeAgain.toNumber();
}

const QString ops_key[] = {"<>", "<=", ">=", "="};
const QString ops_change[] = {"!=", "<=", ">=","=="};
const QString logic_ops_key[] = {"AND", "OR", "NOT"};
const QString logic_ops_change[] = {"&&", "||", "!"};//"ANDNOT","ORNOT"

bool Context::executeIF(QString var){
    var = var.replace(";","");
    var = var.replace(" ","");
    var = var.replace("^","_");
    var = var.toUpper();

    QString functionString("(function() { ");

    QList<QString> constKeys = constsMap.keys();
    for(int i = 0; i < constKeys.size(); i++) {
        QString line = constKeys.at(i);
        line = line.replace("^","_");
        functionString.push_back("var "+line+" = "+QString::number(constsMap.value(constKeys.at(i)))+";");
    }

    QList<QString> varKeys = variablesMap.keys();
    for(int i = 0; i < varKeys.size(); i++) {
        QString line = varKeys.at(i);
        functionString.push_back("var "+line+" = "+QString::number(variablesMap.value(varKeys.at(i)))+";");
    }

    QList<QString> listKeys = arraysMap.keys();
    //put all arrays
    for(int i = 0; i < listKeys.size(); i++) {
        QVector<float> * array = arraysMap.value(listKeys.at(i));
        QString arrayString = "";
        int j = 0;
        for(j = 0; j < array->size() - 1; j++) {
            arrayString.push_back(QString::number(array->at(j))+",");
        }
        arrayString.push_back(QString::number(array->at(j)));

        QString newVar("var "+listKeys.at(i)+" = ["+arrayString+"];");

        functionString.push_back(newVar);
    }

    QString exp = var;

    //replace = or <> if exists
    for(uint i = 0; i < sizeof(ops_key)/sizeof(QString); i++) {
        if(exp.contains(ops_key[i])){
            exp = exp.replace(ops_key[i], ops_change[i]);
            break;
        }
    }

    //replace logic operations
    for(uint i = 0; i < sizeof(logic_ops_key)/sizeof(QString); i++) {
        if(exp.contains(")"+logic_ops_key[i]+"("))
            exp = exp.replace(")"+logic_ops_key[i]+"(", ")"+logic_ops_change[i]+"(");
    }

    //TODO optimize that
    QString aux = "";
    if(exp.contains("(") && exp.contains(")")){
        exp.push_front("(");
        int level = 0;
        for(int i = 1; i < exp.size(); i++) {
            if(exp.at(i-1) != '(' && exp.at(i-1) != '+' && exp.at(i-1) != '-' && exp.at(i-1) != '*' && exp.at(i-1) != '/' && exp.at(i-1) != '&'  && exp.at(i-1) != '|' && exp.at(i-1) != '=' && exp.at(i-1) != '<' && exp.at(i-1) != '>' && exp.at(i) == '(') {
                aux.push_back("[");
                level++;
            } else if(exp.at(i) == ')' && level!=0) {
                level--;
                aux.push_back("]");
            }else{
                 aux.push_back(exp.at(i));
            }
        }

        functionString.push_back("if("+ aux +"){return true;}else{return false;}})");
    }else{
        functionString.push_back("if("+ exp +"){return true;}else{return false;}})");
    }

    QScriptEngine myEngine;
    QScriptValue fun = myEngine.evaluate(functionString);
    QScriptValueList args;
    QScriptValue threeAgain = fun.call(QScriptValue(), args);

    if(threeAgain.isError()){
        qDebug("Error parsing to javascript!!!");
        return false;
    }

    return (float)threeAgain.toBoolean();
}

const QString commands[] = {"ADD", "SUB", "LIMIT", "SET", "BIN", "RANDD", "RANDI", "LIST", "ARITHMETICMEAN", "GEOMETRICMEAN", "HARMONICMEAN", "ON", "OFF", "MAXARRAY", "MINARRAY", "MAXARRAYINDEX", "MINARRAYINDEX", "POPULATIONVARIANCE", "SAMPLEVARIANCE", "SUMARRAY", "SUMSQUAREARRAY", "COPYARRAY", "ZEROARRAY", "Z", ""}; // Z must be always the last to be tested

void Context::executeCommand(QString command){
    command = command.replace(";","");
    command = command.replace(" ","");
    command = command.toUpper();

    if(command == "")
        return;

    uint typeCommand = 0;
    for(typeCommand = 0; typeCommand < sizeof(commands); typeCommand++){
        if(command.contains(commands[typeCommand])){
            break;
        }
    }

    QString p1 = command.section(commands[typeCommand], 1, 1);
    QList<QString> args;

    for(int i = 0; i < p1.count(",") + 1; i++) {
        QString arg = p1.section(",",i,i);
        args.push_back(arg);
    }

    QString eq;
    QString value;
    float sum = 0;
    float aux1 = 0;
    float aux2 = 0;
    int initial, end;
        switch(typeCommand){
            case 0: //ADD
                for(int i = 0; i < args.size(); i++) {
                    eq = args.at(i);
                    addVarIfNotExists(eq); //add var if dont exists;
                    setValue(eq, getValue(eq) + 1);
                }
            break;
            case 1: //SUB
                for(int i = 0; i < args.size(); i++) {
                    eq = args.at(i);
                    addVarIfNotExists(eq); //add var if dont exists;
                    setValue(eq, getValue(eq) - 1);
                }
            break;
            case 3: //SET
                for(int i = 0; i < args.size(); i++) {
                    eq = args.at(i).section("=", 0, 0);
                    value = args.at(i).section("=", 1, 1);
                    addVarIfNotExists(eq); //add var if dont exists;
                    setValue(eq, getValue(value));
                    //printf("New value %s: %f", eq), getValue(eq));
                }
            break;

            case 6: //RANDI
                for(int i = 0; i < args.size(); i++) {
                    eq = args.at(i).section("=", 0, 0);
                    value = args.at(i).section("=", 1, 1);
                    addVarIfNotExists(eq); //add var if dont exists;
                    value+= "("+QString::number(randInt(0, arraysMap.value(value)->size()-1))+")";
                    //printf("Random value: %s", value));
                    setValue(eq, getValue(value));
                }
            break;
            case 7: //LIST
                for(int i = 0; i < args.size(); i++) {
                    eq = args.at(i).section("=", 0, 0);
                    value = args.at(i).section("=", 1, 1);
                    addVarIfNotExists(eq); //add var if dont exists;
                    setValue(eq, getValue(value)); //TODO
                }
            break;
            case 8: //  ARITHMETICMEAN
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    sum = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        sum += arraysMap.value(args.at(0))->at(i);
                    }

                    sum /= (end - initial);

                    setValue(eq, sum); //TODO
            break;
            case 9: // GEOMETRICMEAN
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    sum = 1;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        sum *= arraysMap.value(args.at(0))->at(i);
                    }

                    sum /= (end - initial);

                    setValue(eq, sum); //TODO
            break;
            case 10: // HARMONICMEAN
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    sum = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        sum += 1/arraysMap.value(args.at(0))->at(i);
                    }

                    sum = (end - initial) / sum;

                    setValue(eq, sum); //TODO
            break;
            case 11: //ON
                for(int i = 0; i < args.size(); i++) {
                    eq = args.at(i);
                    GPIO::gpio_set_value(gpioOutputs[(int)getValue(eq) - 1], 1);
                }
            break;
            case 12: //OFF
                for(int i = 0; i < args.size(); i++) {
                    eq = args.at(i);
                    GPIO::gpio_set_value(gpioOutputs[(int)getValue(eq) - 1  ], 0);
                }
            break;
            case 13: // MAXARRAY
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    aux1 = std::numeric_limits<float>::min();
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        if(arraysMap.value(args.at(0))->at(i) > aux1){
                            aux1 = arraysMap.value(args.at(0))->at(i);
                        }
                    }

                    setValue(eq, aux1); //TODO
            break;
            case 14: // MINARRAY
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    aux1 = std::numeric_limits<float>::max();
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        if(arraysMap.value(args.at(0))->at(i) < aux1){
                            aux1 = arraysMap.value(args.at(0))->at(i);
                        }
                    }
                    setValue(eq, aux1); //TODO
            break;
            case 15: // MAXARRAYINDEX
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    aux1 = std::numeric_limits<float>::min();
                    aux2 = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        if(arraysMap.value(args.at(0))->at(i) > aux1){
                            aux1 = arraysMap.value(args.at(0))->at(i);
                            aux2 = i;
                        }
                    }

                    setValue(eq, aux2); //TODO
            break;
            case 16: // MINARRAYINDEX
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    aux1 = std::numeric_limits<float>::max();
                    aux2 = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        if(arraysMap.value(args.at(0))->at(i) < aux1){
                            aux1 = arraysMap.value(args.at(0))->at(i);
                            aux2 = i;
                        }
                    }
                    setValue(eq, aux2); //TODO
            break;
            case 17: //  POPULATIONVARIANCE
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));

                    //get mean
                    sum = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        sum += arraysMap.value(args.at(0))->at(i);
                    }

                    sum /= (end - initial); //mean

                    aux1 = 0;
                    //compute population variance
                    for(int i = initial; i <= end; i++) {
                        aux1 += ((arraysMap.value(args.at(0))->at(i) - sum) * (arraysMap.value(args.at(0))->at(i) - sum)); //sqrt
                    }

                    aux1 /= (end - initial); //o2 - population variance

                    setValue(eq, aux1); //TODO
            break;
            case 18: //  SAMPLEVARIANCE
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));

                    //compute mean
                    sum = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        sum += arraysMap.value(args.at(0))->at(i);
                    }

                    sum /= (end - initial); //mean

                    aux1 = 0;
                    //compute population variance
                    for(int i = initial; i <= end; i++) {
                        aux1 += ((arraysMap.value(args.at(0))->at(i) - sum) * (arraysMap.value(args.at(0))->at(i) - sum)); //sqrt
                    }

                    aux1 /= (end - initial) - 1; //s2 - sample variance

                    setValue(eq, aux1); //TODO
            break;
            case 19: //  SUMARRAY
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    sum = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        sum += arraysMap.value(args.at(0))->at(i);
                    }

                    setValue(eq, sum); //TODO
            break;
            case 20: //  SUMSQUAREARRAY
                    eq = args.at(0).section("=", 0, 0);
                    addVarIfNotExists(eq); //add var if dont exists;

                    args.replace(0, args.at(0).section("=", 1, 1));
                    sum = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        sum += arraysMap.value(args.at(0))->at(i) * arraysMap.value(args.at(0))->at(i);
                    }

                    setValue(eq, sum); //TODO
            break;
            case 21: //  COPYARRAY //TODO see if is possible create a new array with this command
                    sum = 0;
                    initial = getValue(args.at(1));
                    end = getValue(args.at(2));
                    for(int i = initial; i <= end; i++) {
                        arraysMap.value(args.at(1))->replace(i, arraysMap.value(args.at(0))->at(i));
                    }
            break;
            case 22: //  ZEROARRAY
                    arraysMap.value(args.at(0))->fill(0);
            break;

            case ((sizeof(commands)/sizeof(QString))-2): //Z%d
                eq = args.at(0);
                zpulseMap[eq.toInt()] = true;
            break;
            case 5: //RANDD
            case 4: //BIN
            case 2: //LIMIT
            default:
                 qCritical("Command %d - %s not suppported", typeCommand, qPrintable(command));
            break;
        }
}

bool Context::isTimePassed(qint64 timeBase, uint time){
    if(systemTime - timeBase >= time) {
        return true;
    }
    return false;
}

bool Context::isZPulseRequested(int index){
    return zpulseMap.value(index);
}

bool Context::isZPulseRequested(){
    QList<bool> list = zpulseMap.values();
    for(int i=0; i < list.size(); i++) {
        if(list.at(i)){
            return true;
        }
    }
    return false;
}

void Context::clearZPulseRequests(){
    QList<int> list = zpulseMap.keys();

    for(int i=0; i < list.size(); i++) {
        zpulseMap[list.at(i)] = false; //TODO see that
    }
}

void Context::setSystemTime(qint64 time) {
    this->systemTime = time;
}

void Context::addEventPort(int pin){
    inputsEvents.push_back(pin);
}

bool Context::isEventHappened(int pin){
    return inputsEvents.contains(pin);
}

void Context::clearEvents(){
    inputsEvents.clear();
}

qint64 Context::getSystemTime() {
    return this->systemTime;
}

qint64 Context::getCurrentTimeSystem() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    qint64 milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    return milliseconds;
}


QString Context::toString(){
    QString functionString("");

    QList<QString> constKeys = constsMap.keys();
    for(int i = 0; i < constKeys.size(); i++) {
        QString line = constKeys.at(i);
        line = line.replace("^","_");
        functionString.push_back("var "+line+" = "+QString::number(constsMap.value(constKeys.at(i)))+";");
    }

    QList<QString> varKeys = variablesMap.keys();
    for(int i = 0; i < varKeys.size(); i++) {
        QString line = varKeys.at(i);
        functionString.push_back("var "+line+" = "+QString::number(variablesMap.value(varKeys.at(i)))+";");
    }

    QList<QString> listKeys = arraysMap.keys();
    //put all arrays
    for(int i = 0; i < listKeys.size(); i++) {
        QVector<float> * array = arraysMap.value(listKeys.at(i));
        QString arrayString = "";
        int j = 0;
        for(j = 0; j < array->size() - 1; j++) {
            arrayString.push_back(QString::number(array->at(j))+",");
        }
        arrayString.push_back(QString::number(array->at(j)));

        QString newVar("var "+listKeys.at(i)+" = ["+arrayString+"];");

        functionString.push_back(newVar);
    }

    return functionString;
}
