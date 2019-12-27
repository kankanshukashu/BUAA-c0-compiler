#include "tokenizer.h"
#include "analyser.h"

using namespace std;

string inputFile;
long long inputSize;
long long index = 0;
vector<Token> tokenList;
long long listIndex = 0;
long long listSize;

bool outToken(Token t);

int main(int argc, char *argv[])
{
    string h = "-h", s = "-s", c = "-c";
    if (argc == 1 || (argc == 2 && argv[1] == h)) {
        cout << "Usage:" << endl;
        cout << "  cc0 [options] input [-o file]" << endl;
        cout << "or" << endl;
        cout << "  cc0 [-h]" << endl;
        cout << "Options:" << endl;
        cout << "  -s        将输入的 c0 源代码翻译为文本汇编文件" << endl;
        cout << "  -c        将输入的 c0 源代码翻译为二进制目标文件" << endl;
        cout << "  -h        显示关于编译器使用的帮助" << endl;
        cout << "  -o file   输出到指定的文件 file" << endl;
        cout << endl;
        cout << "不提供任何参数时，默认为 -h" << endl;
        cout << "提供 input 不提供 -o file 时，默认为 -o out" << endl;
    }
    else if ((argc == 3 || argc == 5) && (argv[1] == s || argv[1] == c)) {
        ifstream in(argv[2]);
        if (!in.is_open()) {
            cout << "找不到该文件" << endl;
            in.close();
        }
        else {
            stringstream buf;
            char ch;
            while (buf && in.get(ch))
                buf << ch;
            in.close();
            inputFile = buf.str();
            inputSize = inputFile.size();
            while (index < inputSize) {
                Token tmp = nextToken();
                if (outToken(tmp))
                    return 0;
                tokenList.push_back(tmp);
            }
            listSize = tokenList.size();
            string err = analyseProgram();
            if (err != "ok") {
                cout << err << endl;
                return 0;
            }
            else
                cout << "完成力" << endl;
            if (argv[1] == s) {
                if (argc == 3)
                    sout("out");
                else
                    sout(argv[4]);
            }
            else {
                if (argc == 3)
                    oout("out");
                else
                    oout(argv[4]);
            }
        }
    }
    else {
        cout << "这是什么命令" << endl;
    }
    return 0;
}

bool outToken(Token t) {
    switch (t._type) {
    case UNACCEPTABLE_ERROR:
        cout << "不可接受字符" << endl;
        return true;
    case DIGIT_ALPHA_ERROR:
        cout << "不合法的标识符" << endl;
        return true;
    case INTEGER_OVERFLOW_ERROR:
        cout << "整数字面量溢出" << endl;
        return true;
    case UNEQUAL_SIGN_ERROR:
        cout << "!后面只能是=" << endl;
        return true;
    case EXCEPTED_ERROR:
        cout << "啊哦" << endl;
        return true;
    }
    return false;
}