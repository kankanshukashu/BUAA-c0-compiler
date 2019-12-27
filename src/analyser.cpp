#include "analyser.h"

using namespace std;

bool G = true;
vector<pair<string, bool> > GDT;
vector<pair<string, bool> > LDT;

vector<funcInfo> funcList;
int paramNum;

vector<string> startCode;
vector<vector<string> > funcCode;

//主程序
string analyseProgram()
{
    if (listSize < 6)
        return "dai ma ken ding bu dui";
    string err = analyseVariableDeclaration();
    if (err != "ok")
        return err;
    G = false;
    err = analyseFunctionDeclaration();
    if (err != "ok")
        return err;
    if (listIndex < listSize)
        if (tokenList[listIndex]._type != EOF_ERROR)
            return "dai ma hou mian you qi guai de dong xi";

    int i = 0;
    bool exist = false;
    for (i = 0; i < funcList.size(); i++) {
        if (funcList[i].name == "main") {
            exist = true;
            break;
        }
    }
    if (!exist)
        return "mei you main han shu";

    return "ok";
}

//变量声明
string analyseVariableDeclaration()
{
    while (true) {
        if ((listIndex + 2) < listSize && tokenList[listIndex + 2]._type == LEFT_BRACKET)
            return "ok";

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";

        if (tokenList[listIndex]._type == CONST) {
            listIndex++;
            string err = analyseConstVariableDeclaration();
            if (err != "ok")
                return err;
            if (!(listIndex < listSize))
                return "xiao lao di ni zen me hui shi?";
            if (tokenList[listIndex]._type != SEMICOLON)
                return "wei shen me bu xie ';'?";
            listIndex++;
        }

        else if (tokenList[listIndex]._type == INT) {
            listIndex++;
            string err = analyseIntVariableDeclaration();
            if (err != "ok")
                return err;
            if (!(listIndex < listSize))
                return "xiao lao di ni zen me hui shi?";
            if (tokenList[listIndex]._type != SEMICOLON)
                return "wei shen me bu xie ';'??";
            listIndex++;
        }
        else
            break;
    }
    return "ok";
}

//const常量声明
string analyseConstVariableDeclaration()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != INT)
        return "CONST hou mian bi xu shi INT";
    listIndex++;

    while (true) {
        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != IDENTIFIER)//done
            return "bian liang ming ne?";

        if (G) {
            if (findDT(tokenList[listIndex], GDT))
                return "chong fu ding yi de quan ju chang liang";
            else
                GDT.push_back(pair<string, bool>(tokenList[listIndex]._value, true));
            //startCode.push_back("ipush 0");
        }
        else {
            if (findDT(tokenList[listIndex], LDT))
                return "chong fu ding yi de ju bu chang liang";
            else
                LDT.push_back(pair<string, bool>(tokenList[listIndex]._value, true));
            //funcCode.back().push_back("ipush 0");
        }

        listIndex++;

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != EQUAL_SIGN)
            return "const bi xu xian shi chu shi hua";
        listIndex++;

        string err = analyseExpression();
        if (err != "ok")
            return err;

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type == COMMA) {
            listIndex++;
            continue;
        }
        else
            break;
    }
    return "ok";
}

//int变量声明
string analyseIntVariableDeclaration()
{
    while (true) {
        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != IDENTIFIER)//done
            return "bian liang ming ne?";

        if (G) {
            if (findDT(tokenList[listIndex], GDT))
                return "chong fu ding yi de quan ju bian liang";
            else
                GDT.push_back(pair<string, bool>(tokenList[listIndex]._value, false));
            //startCode.push_back("ipush 0");
        }
        else {
            if (findDT(tokenList[listIndex], LDT))
                return "chong fu ding yi de ju bu bian liang";
            else
                LDT.push_back(pair<string, bool>(tokenList[listIndex]._value, false));
            //funcCode.back().push_back("ipush 0");
        }

        listIndex++;

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type == EQUAL_SIGN) {
            listIndex++;
            string err = analyseExpression();
            if (err != "ok")
                return err;
        }
        else {
            if (G)
                startCode.push_back("ipush 0");
            else
                funcCode.back().push_back("ipush 0");
        }
            
        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type == COMMA) {
            listIndex++;
            continue;
        }
        else
            break;
    }
    return "ok";
}

//表达式
string analyseExpression()
{
    return analyseAdditiveExpression();
}

//加法表达式
string analyseAdditiveExpression()
{
    string err = analyseMultiplicativeExpression();
    if (err != "ok")
        return err;
    while (true) {
        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";

        int isAdd = 0;
        if (tokenList[listIndex]._type == PLUS_SIGN) {
            listIndex++;
            isAdd = 1;
        }
        else if (tokenList[listIndex]._type == MINUS_SIGN) {
            listIndex++;
        }
        else
            break;

        err = analyseMultiplicativeExpression();
        if (err != "ok")
            return err;

        if (G) {
            if (isAdd)
                startCode.push_back("iadd");
            else
                startCode.push_back("isub");
        }
        else {
            if (isAdd)
                funcCode.back().push_back("iadd");
            else
                funcCode.back().push_back("isub");
        }
    }
    return "ok";
}

//乘法表达式
string analyseMultiplicativeExpression()
{
    string err = analyseUnaryExpression();
    if (err != "ok")
        return err;
    while (true) {
        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";

        int isMul = 0;
        if (tokenList[listIndex]._type == MULTIPLICATION_SIGN) {
            listIndex++;
            isMul = 1;
        }
        else if (tokenList[listIndex]._type == DIVISION_SIGN) {
            listIndex++;
        }
        else
            break;

        err = analyseUnaryExpression();
        if (err != "ok")
            return err;

        if (G) {
            if (isMul)
                startCode.push_back("imul");
            else
                startCode.push_back("idiv");
        }
        else {
            if (isMul)
                funcCode.back().push_back("imul");
            else
                funcCode.back().push_back("idiv");
        }
    }
    return "ok";
}

//一元表达式
string analyseUnaryExpression()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";

    int isNeg = 0;
    if (tokenList[listIndex]._type == PLUS_SIGN) {
        listIndex++;
    }
    else if (tokenList[listIndex]._type == MINUS_SIGN) {
        isNeg = 1;
        listIndex++;
    }

    if (!(listIndex < listSize + 1))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type == LEFT_BRACKET) {
        listIndex++;

        string err = analyseExpression();
        if (err != "ok")
            return err;

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != RIGHT_BRACKET)
            return "wei shen me bu xie ')'?";
        listIndex++;
    }
    else if (tokenList[listIndex]._type == IDENTIFIER && tokenList[listIndex + 1]._type == LEFT_BRACKET) {//done

        if (findDT(tokenList[listIndex], LDT))
            return "bian liang ming bu neng na lai dan han shu diao yong";
        bool exist = false;
        bool isVoid = false;
        for (int i = 0; i < funcList.size(); i++) {
            if (tokenList[listIndex]._value == funcList[i].name) {
                isVoid = funcList[i].isVoid;
                exist = true;
                break;
            }
        }
        if (!exist)
            return "wei ding yi de han shu";
        else if (isVoid)
            return "void lei xing de han shu bu neng can yu yun suan";

        string err = analyseFunctionCall();
        if (err != "ok")
            return err;

    }
    else if (tokenList[listIndex]._type == IDENTIFIER) {//done

        if (findDT(tokenList[listIndex], LDT)) {
            int i = 0;
            for (i = 0; tokenList[listIndex]._value != LDT[i].first; i++);
            string tmp = "loada 0, " + to_string(i);
            funcCode.back().push_back(tmp);
            funcCode.back().push_back("iload");
        }
        else if (findDT(tokenList[listIndex], GDT)) {
            int i = 0;
            for (i = 0; tokenList[listIndex]._value != GDT[i].first; i++);
            if (G) {
                string tmp = "loada 0, " + to_string(i);
                startCode.push_back(tmp);
                startCode.push_back("iload");
            }
            else {
                string tmp = "loada 1, " + to_string(i);
                funcCode.back().push_back(tmp);
                funcCode.back().push_back("iload");
            }
        }
        else
            return "wei ding yi de bian liang";

        listIndex++;

    }
    else if (tokenList[listIndex]._type == UNSIGNED_INTEGER) {
        string tmp = "ipush " + tokenList[listIndex]._value;
        listIndex++;
        if (G) 
            startCode.push_back(tmp);
        else 
            funcCode.back().push_back(tmp);
    }
    else
        return "biao da shi mei xie wan";

    if (isNeg) {
        if (G) 
            startCode.push_back("ineg");
        else 
            funcCode.back().push_back("ineg");
    }
    return "ok";
}

//函数声明
string analyseFunctionDeclaration()
{
    while (true) {
        if (listIndex >= listSize)
            return "ok";
        if (tokenList[listIndex]._type == INT) {
            listIndex++;
            if (!(listIndex < listSize))
                return "xiao lao di ni zen me hui shi?";
            if (tokenList[listIndex]._type != IDENTIFIER)//done
                return "han shu ming ne?";

            if (findDT(tokenList[listIndex], GDT))
                return "biao shi fu yi bei sheng ming wei bian liang";
            bool exist = false;
            for (int i = 0; i < funcList.size(); i++) {
                if (tokenList[listIndex]._value == funcList[i].name) {
                    exist = true;
                    break;
                }
            }
            funcInfo tmp;
            if (exist)
                return "chong fu ding yi de han shu ming";
            else {
                tmp.name = tokenList[listIndex]._value;
                tmp.isVoid = false;
            }

            listIndex++;

            string err = analyseParameterClause();
            if (err != "ok")
                return err;
            tmp.paramSlot = paramNum;
            paramNum = 0;
            funcList.push_back(tmp);
            funcCode.push_back(vector<string>());

            err = analyseCompoundStatement();
            if (err != "ok")
                return err;

            funcCode.back().push_back("ipush 0");
            funcCode.back().push_back("iret");
        }
        else if (tokenList[listIndex]._type == VOID) {
            listIndex++;
            if (!(listIndex < listSize))
                return "xiao lao di ni zen me hui shi?";
            if (tokenList[listIndex]._type != IDENTIFIER)//done
                return "han shu ming ne?";

            if (findDT(tokenList[listIndex], GDT))
                return "biao shi fu yi bei sheng ming wei bian liang";
            bool exist = false;
            for (int i = 0; i < funcList.size(); i++) {
                if (tokenList[listIndex]._value == funcList[i].name) {
                    exist = true;
                    break;
                }
            }
            funcInfo tmp;
            if (exist)
                return "chong fu ding yi de han shu ming";
            else {
                tmp.name = tokenList[listIndex]._value;
                tmp.isVoid = true;
            }
            listIndex++;

            string err = analyseParameterClause();
            if (err != "ok")
                return err;
            tmp.paramSlot = paramNum;
            paramNum = 0;
            funcList.push_back(tmp);
            funcCode.push_back(vector<string>());

            err = analyseCompoundStatement();
            if (err != "ok")
                return err;

            funcCode.back().push_back("ret");
        }
        else
            break;
        vector<pair<string, bool>>().swap(LDT);
    }
    return "ok";
}

//函数的参数列表
string analyseParameterClause()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != LEFT_BRACKET)
        return "can shu lie biao de '('?";
    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET) {
        string err = analyseParameterDeclaration();
        if (err != "ok")
            return err;
    }

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET)
        return "wei shen me bu xie ')'??";
    listIndex++;

    return "ok";
}

//函数的参数声明
string analyseParameterDeclaration()
{
    while (true) {
        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type == CONST) {
            listIndex++;

            if (!(listIndex < listSize))
                return "xiao lao di ni zen me hui shi?";
            if (tokenList[listIndex]._type != INT)
                return "CONST hou mian bi xu shi INT";
            listIndex++;

            if (!(listIndex < listSize))
                return "xiao lao di ni zen me hui shi?";
            if (tokenList[listIndex]._type != IDENTIFIER)
                return "can shu ming ne?";

            LDT.push_back(pair<string, bool>(tokenList[listIndex]._value, true));
            paramNum++;

            listIndex++;
        }
        else if (tokenList[listIndex]._type == INT) {
            listIndex++;

            if (!(listIndex < listSize))
                return "xiao lao di ni zen me hui shi?";
            if (tokenList[listIndex]._type != IDENTIFIER)
                return "can shu ming ne?";

            LDT.push_back(pair<string, bool>(tokenList[listIndex]._value, false));
            paramNum++;

            listIndex++;
        }
        else
            return "can shu sheng ming cuo wu";

        if (tokenList[listIndex]._type != COMMA)
            break;
        listIndex++;
    }
    return "ok";
}

//函数体
string analyseCompoundStatement()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != LEFT_BRACE)
        return "han shu ti de '{'?";
    listIndex++;

    string err = analyseVariableDeclaration();
    if (err != "ok")
        return err;

    err = analyseStatementSeq();
    if (err != "ok")
        return err;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACE)
        return "wei shen me bu xie '}'?";
    listIndex++;

    return "ok";
}

//语句序列
string analyseStatementSeq()
{
    while (true) {
        string err = analyseStatement();
        if (err == "bu shi he fa de ju zi")
            break;
        if (err != "ok")
            return err;
    }
    return "ok";
}

//语句
string analyseStatement()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";

    if (tokenList[listIndex]._type == LEFT_BRACE) {
        listIndex++;

        string err = analyseStatementSeq();
        if (err != "ok")
            return err;

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != RIGHT_BRACE)
            return "wei shen me bu xie '}'?";
        listIndex++;
    }
    else if (tokenList[listIndex]._type == IF) {
        listIndex++;
        string err = analyseConditionStatement();
        if (err != "ok")
            return err;
    }
    else if (tokenList[listIndex]._type == WHILE) {
        listIndex++;
        string err = analyseLoopStatement();
        if (err != "ok")
            return err;
    }
    else if (tokenList[listIndex]._type == RETURN) {
        listIndex++;
        bool hasReturn = !(funcList.back().isVoid);
        string err = analyseJumpStatement(hasReturn);
        if (err != "ok")
            return err;
    }
    else if (tokenList[listIndex]._type == PRINT) {
        listIndex++;
        string err = analysePrintStatement();
        if (err != "ok")
            return err;
    }
    else if (tokenList[listIndex]._type == SCAN) {
        listIndex++;
        string err = analyseScanStatement();
        if (err != "ok")
            return err;
    }
    else if (tokenList[listIndex]._type == IDENTIFIER) {
        listIndex++;

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";

        if (tokenList[listIndex]._type == EQUAL_SIGN) {
            listIndex--;
            string err = analyseAssignmentExpression();
            if (err != "ok")
                return err;
        }
        else if (tokenList[listIndex]._type == LEFT_BRACKET) {
            listIndex--;

            if (findDT(tokenList[listIndex], LDT))
                return "bian liang ming bu neng na lai dan han shu diao yong";

            string err = analyseFunctionCall();
            if (err != "ok")
                return err;
        }
        else
            return "biao shi fu hou mian zen me mei dong xi a?";

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != SEMICOLON)
            return "wei shen me bu xie ';'???";
        listIndex++;
    }
    else if (tokenList[listIndex]._type == SEMICOLON) {
        listIndex++;
    }
    else
        return "bu shi he fa de ju zi";
    return "ok";
}

//if条件语句
string analyseConditionStatement()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != LEFT_BRACKET)
        return "'if' hou mian de '(' ne?";
    listIndex++;

    string err = analyseCondition();
    if (err != "ok")
        return err;
    long long conditionFinishIndex = funcCode.back().size() - 1;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET)
        return "wei shen me bu xie ')'???";
    listIndex++;
    
    err = analyseStatement();
    if (err != "ok")
        return err;

    funcCode.back().push_back("jmp ");
    funcCode.back()[conditionFinishIndex] = funcCode.back()[conditionFinishIndex] + to_string(funcCode.back().size());
    long long statementJumpIndex = funcCode.back().size() - 1;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type == ELSE) {
        listIndex++;
        err = analyseStatement();
        if (err != "ok")
            return err;
    }
    funcCode.back()[statementJumpIndex] = funcCode.back()[statementJumpIndex] + to_string(funcCode.back().size());
    
    return "ok";
}

//条件表达式
string analyseCondition()
{
    string err = analyseExpression();
    if (err != "ok")
        return err;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type == LESS_THAN_SIGN) {
        listIndex++;
        err = analyseExpression();
        if (err != "ok")
            return err;

        funcCode.back().push_back("icmp");
        funcCode.back().push_back("jge ");
    }
    else if (tokenList[listIndex]._type == LESS_THAN_OR_EQUAL_SIGN) {
        listIndex++;
        err = analyseExpression();
        if (err != "ok")
            return err;

        funcCode.back().push_back("icmp");
        funcCode.back().push_back("jg ");
    }
    else if (tokenList[listIndex]._type == GREATER_THAN_SIGN) {
        listIndex++;
        err = analyseExpression();
        if (err != "ok")
            return err;

        funcCode.back().push_back("icmp");
        funcCode.back().push_back("jle ");
    }
    else if (tokenList[listIndex]._type == GREATER_THAN_OR_EQUAL_SIGN) {
        listIndex++;
        err = analyseExpression();
        if (err != "ok")
            return err;

        funcCode.back().push_back("icmp");
        funcCode.back().push_back("jl ");
    }
    else if (tokenList[listIndex]._type == UNEQUAL_SIGN) {
        listIndex++;
        err = analyseExpression();
        if (err != "ok")
            return err;

        funcCode.back().push_back("icmp");
        funcCode.back().push_back("je ");
    }
    else if (tokenList[listIndex]._type == DOUBLE_EQUAL_SIGH) {
        listIndex++;
        err = analyseExpression();
        if (err != "ok")
            return err;

        funcCode.back().push_back("icmp");
        funcCode.back().push_back("jne ");
    }
    else {
        funcCode.back().push_back("ipush 0");
        funcCode.back().push_back("icmp");
        funcCode.back().push_back("je ");
    }
    return "ok";
}

//while循环语句
string analyseLoopStatement()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != LEFT_BRACKET)
        return "'while' hou mian de '(' ne?";
    listIndex++;

    long long conditionStartIndex = funcCode.back().size();
    string err = analyseCondition();
    if (err != "ok")
        return err;
    long long conditionFinishIndex = funcCode.back().size() - 1;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET)
        return "wei shen me bu xie ')'????";
    listIndex++;

    err = analyseStatement();
    if (err != "ok")
        return err;

    string tmp = "jmp " + to_string(conditionStartIndex);
    funcCode.back().push_back(tmp);
    funcCode.back()[conditionFinishIndex] = funcCode.back()[conditionFinishIndex] + to_string(funcCode.back().size());

    return "ok";
}

//return跳转语句
string analyseJumpStatement(bool hasReturn)
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type == SEMICOLON) {
        if (hasReturn)
            return "int han shu bi xu you fan hui zhi";
        listIndex++;
        funcCode.back().push_back("ret");
        return "ok";
    }
    else {
        if (!hasReturn)
            return "void han shu bu neng you fan hui zhi";
        string err = analyseExpression();
        if (err != "ok")
            return err;
    }

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != SEMICOLON)
        return "wei shen me bu xie ';'????";
    listIndex++;
    funcCode.back().push_back("iret");

    return "ok";
}

//print输出语句
string analysePrintStatement()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != LEFT_BRACKET)
        return "'print' hou mian de '(' ne?";
    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET) {
        string err = analysePrintableList();
        if (err != "ok")
            return err;
    }

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET)
        return "wei shen me bu xie ')'?????";
    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != SEMICOLON)
        return "wei shen me bu xie ';'?????";
    listIndex++;

    return "ok";
}

//输出列表
string analysePrintableList()
{
    while (true) {
        string err = analyseExpression();
        if (err != "ok")
            return err;

        funcCode.back().push_back("iprint");

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != COMMA)
            break;
        listIndex++;
        funcCode.back().push_back("ipush 32");
        funcCode.back().push_back("cprint");
    }
    funcCode.back().push_back("printl");
    return "ok";
}

//scan读入语句
string analyseScanStatement()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != LEFT_BRACKET)
        return "'scan' hou mian de '(' ne?";
    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != IDENTIFIER)//done
        return "'scan' de biao shi fu ne?";

    if (findDT(tokenList[listIndex], LDT)) {
        if (isConst(tokenList[listIndex], LDT))
            return "bu neng scan const";
        else {
            int i = 0;
            for (i = 0; tokenList[listIndex]._value != LDT[i].first; i++);
            string tmp = "loada 0, " + to_string(i);
            funcCode.back().push_back(tmp);
        }
    }
    else if (findDT(tokenList[listIndex], GDT)) {
        if (isConst(tokenList[listIndex], GDT))
            return "bu neng scan const";
        else {
            int i = 0;
            for (i = 0; tokenList[listIndex]._value != GDT[i].first; i++);
            string tmp = "loada 1, " + to_string(i);
            funcCode.back().push_back(tmp);
        }
    }
    else
        return "wei ding yi de bian liang";

    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET)
        return "wei shen me bu xie ')'??????";
    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != SEMICOLON)
        return "wei shen me bu xie ';'??????";
    listIndex++;

    funcCode.back().push_back("iscan");
    funcCode.back().push_back("istore");

    return "ok";
}

//赋值语句
string analyseAssignmentExpression()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != IDENTIFIER)//done
        return "fu zhi yu ju de biao shi fu ne?";

    if (findDT(tokenList[listIndex], LDT)) {
        if (isConst(tokenList[listIndex], LDT))
            return "bu neng gei const fu zhi";
        else {
            int i = 0;
            for (i = 0; tokenList[listIndex]._value != LDT[i].first; i++);
            string tmp = "loada 0, " + to_string(i);
            funcCode.back().push_back(tmp);
        }
    }
    else if (findDT(tokenList[listIndex], GDT)) {
        if (isConst(tokenList[listIndex], GDT))
            return "bu neng gei const fu zhi";
        else {
            int i = 0;
            for (i = 0; tokenList[listIndex]._value != GDT[i].first; i++);

            if (G) {
                string tmp = "loada 0, " + to_string(i);
                startCode.push_back(tmp);
            }
            else {
                string tmp = "loada 1, " + to_string(i);
                funcCode.back().push_back(tmp);
            }
        }
    }
    else
        return "wei ding yi de bian liang";

    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != EQUAL_SIGN)
        return "fu zhi yu ju de '=' ne?";
    listIndex++;

    string err = analyseExpression();
    if (err != "ok")
        return err;

    if (G) 
        startCode.push_back("istore");
    else 
        funcCode.back().push_back("istore");

    return "ok";
}

//函数调用语句
string analyseFunctionCall()
{
    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != IDENTIFIER)//done
        return "han shu ming ne?";

    bool exist = false;
    int i = 0;
    for (i = 0; i < funcList.size(); i++) {
        if (tokenList[listIndex]._value == funcList[i].name) {
            exist = true;
            break;
        }
    }
    if (!exist)
        return "wei ding yi de han shu";

    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != LEFT_BRACKET)
        return "han shu ming hou mian de '(' ne?";
    listIndex++;

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET) {
        int checkParam = 0;
        string err = analyseExpressionList(checkParam);
        if (err != "ok")
            return err;
        if (checkParam != funcList[i].paramSlot) 
            return "can shu lie biao bu dui ying";
    }
    else {
        if (funcList[i].paramSlot != 0)
            return "can shu lie biao bu dui ying";
    }

    if (!(listIndex < listSize))
        return "xiao lao di ni zen me hui shi?";
    if (tokenList[listIndex]._type != RIGHT_BRACKET)
        return "wei shen me bu xie ')'???????";
    listIndex++;

    string tmp = "call " + to_string(i);
    funcCode.back().push_back(tmp);

    return "ok";
}

//函数调用的参数列表
string analyseExpressionList(int &check)
{
    while (true) {
        string err = analyseExpression();
        if (err != "ok")
            return err;

        check++;

        if (!(listIndex < listSize))
            return "xiao lao di ni zen me hui shi?";
        if (tokenList[listIndex]._type != COMMA)
            break;
        listIndex++;
    }
    return "ok";
}

bool findDT(Token tk, vector<pair<string, bool> > dt)
{
    bool exist = false;
    for (int i = 0; i < dt.size(); i++) {
        if (tk._value == dt[i].first) {
            exist = true;
            break;
        }
    }
    return exist;
}

bool isConst(Token tk, vector<pair<string, bool> > dt)
{
    for (int i = 0; i < dt.size(); i++) {
        if (tk._value == dt[i].first) {
            return dt[i].second;
        }
    }
    return false;
}
