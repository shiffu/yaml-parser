#include "parser.h"
#include "nodes.h"

#include <vector>
#include <string>

using namespace std;

int main()
{
    Parser p;
    p.parse("../test.yml");

    Document d{};
    SeqNode seq;
    seq.data = { new ValueNode("value1") };

    cout << "seq[0] = " << seq[0] << endl;


    return 0;
}