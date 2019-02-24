//@author José ALfredo Ferreira Pace
//@id V-26765212
//@author Cherry Gota Figueroa
//@id V-25560023 

#include <cassert>
#include <cstring>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <tuple>
#include <stdexcept>
#include <string>
#include <sstream>

#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

#include <list.H>
#include <stack.H>

using namespace Designar;

#include <helpers.H>
#include <expnode-sol.H>

#include <dictnode.H>

string get_prompt(size_t i)
{
  stringstream s;
  s << i << " $ ";
  return s.str();
}

Cnode *root;

void add_words_reserved(Cnode *root)
{
  /* Words Reserved*/

  root->insert_word("add");
  root->insert_word("call");
  root->insert_word("closure");
  root->insert_word("def"); // Esta declara variables
  root->insert_word("divMod");
  root->insert_word("exp");
  root->insert_word("fun"); // Esta declara una funcion
  root->insert_word("fst");
  root->insert_word("int");
  root->insert_word("isVoid");
  root->insert_word("ifGreater");
  root->insert_word("let");
  root->insert_word("neg");
  root->insert_word("mul");
  root->insert_word("pair");
  root->insert_word("snd");
  root->insert_word("var");
  root->insert_word("void");
  //root->insert_word("Enviroment"); //Este no se si va aqui
}

char *words_reserved(const char *str, int est)
{
 // cout << "\n( str:" << str << "; e: " << est  << ")\n";
  static DynArray<string> words;
  static int i = 0;

  if (est == 0)
  {
    i = 0;
    words.clear();

    if (str[0] == '\0')
      words = root->words(); //All Words
    else
    {

      try
      {
        auto prefix = root->search_prefix(str);

        // if (get<0>(prefix) == root)
        //   return nullptr;
        string cad = get<1>(prefix);
        if (cad != "\0")
          return nullptr;

        // cout << "key " << get<0>(prefix)->get_key() << endl;
        // cout <<endl<< str << endl;

        auto w = get<0>(prefix)->words();

        for (int c = 0; c < w.size(); c++)
          words.append(str + w[c]);
      }
      catch (std::underflow_error &er)
      {

        // words.append(str);
        //cout << endl<< er.what() << endl;
        return nullptr;
      }
    }
  }

  if (i < words.size())
    return strdup(words[i++].c_str());

  return nullptr;
}

int main()
{
  cout << "Command line for PR3 programming language\n"
       << "If yo want to quit, type exit and enter\n\n";

  size_t counter = 0;

  root = new Cnode('-');
  add_words_reserved(root);

  //  auto prefix = root->search_prefix("va");

  //    cout << endl << get<0>(prefix)->get_key()<< endl;
  // cout << "palabra: ";
  //    get<0>(prefix)->print_words();

  // return 0;

  rl_completion_entry_function = words_reserved;

  Enviroment env;

  while (true)
  {
    string prompt = get_prompt(counter);

    char *line = readline(prompt.c_str());

    if (line == nullptr)
      break;

    string prog = remove_whites(line);

    if (prog == "exit")
      break;

    Exp *expr = nullptr;

    try
    {
      expr = parse(prog);

      if (expr)
      {
        int i = prog.find("def");
        string nom;

        if (i == -1)
          i = prog.find("fun");
        
          
        //cout << "i: " << i<< endl;

        if (i != -1)
        {
          i += 4;

          while (prog[i] != ',')
            nom.push_back(prog[i++]);

          // cout << endl
          //      << "nombre: " << nom << endl;

          root->insert_word(nom.c_str());
        }
   
        
      }

      if (expr == nullptr)
        continue;

      Exp *result = expr->eval(env);
      cout << result->to_string() << endl;

      delete result;
      delete expr;
      ++counter;
    }
    catch (const domain_error &e)
    {
      delete expr;
      cout << "Error: " << e.what() << endl;
    }
    catch (const logic_error &e)
    {
      cout << "Error: " << e.what() << endl;
    }

    add_history(line);
    free(line);
  }
  root->destroy();
  delete root;

  return 0;
};
