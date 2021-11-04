#include <iostream>
#include "gdxlanguage/gdxlexer.h"

using namespace std;

int main()
{
	string *input = new string("<ClickButton><Button on_pressed=on_btn_pressed/>");

	GDXLexer *lexer = new GDXLexer();
	lexer->set_source(input);

	try
	{
		while (!lexer->cursor()->eof)
		{
			Token *t = lexer->get_token();

			if (t)
			{
				ImportToken *import = dynamic_cast<ImportToken *>(t);
				VariableToken *variable = dynamic_cast<VariableToken *>(t);
				TagToken *tag = dynamic_cast<TagToken *>(t);

				if (import != nullptr)
				{
					cout << "Found import token:" << endl;
					cout << "  Class name: " << import->class_name << endl;
					cout << "  Path: " << import->relative_path << endl;
				}
				if (variable != nullptr)
				{
					cout << "Found variable token:" << endl;
					cout << "  Var name: " << variable->name << endl;
					cout << "  Type: " << variable->type << endl;
					cout << "  Value: " << variable->value << endl;
				}
				if (tag != nullptr)
				{
					cout << "Found tag:" << endl;
					cout << "  Class name: " << tag->class_name << endl;
					cout << "  Type: " << tag->type << endl;
					if (tag->properties.size() > 0)
					{
						cout << "  Props: " << endl;
						for (map<string, string>::iterator iter = tag->properties.begin(); iter != tag->properties.end(); ++iter)
						{
							cout << "    " << iter->first << ": " << iter->second << endl;
						}
					}
				}
			}
			else
			{
				lexer->cursor()->walk();
			}
		}
	}
	catch (ParseException *e)
	{
		cout << e->msg << endl;
	}

	return 0;
}