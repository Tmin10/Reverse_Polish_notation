#include <iostream>
#include "string"
#include <boost/lexical_cast.hpp>
#include <cctype>

using boost::lexical_cast;
using boost::bad_lexical_cast;

using namespace std;

int prior(char a)
{
  switch(a)
  {
    case '*':
    case '/':
         return 3;
         break;
    case '-':
    case '+':
         return 2;
         break;
    case '(':
         return 1;
         break;
    case ')':
        return 0;
        break;
    default:
      return -1;
  }
}

bool valid(string str)
{
    bool chars=true;
    if(!isalpha(str[0])) //Если первый элемент не буква
        return false;
    for (int i=1; i<str.length(); i++)
    {
        if(isdigit(str[i])&&chars) //если элемент число, переключим
        {
            chars=false;
        }
        if (isalpha(str[i])&&!chars)
        {
            return false;
        }
    }
    return true;
}

string get_first(string str)
{
    string ret;
    for (int i=0; i<str.length(); i++)
    {
        if (isalpha(str[i]))
            ret+=str[i];
        else
            return ret;
    }
}

int get_second(string str)
{
    string ret;
    for (int i=1; i<str.length(); i++)
    {
        if (isdigit(str[i]))
            ret+=str[i];
    }
    try {
        int ii = lexical_cast<int> (ret);
        return ii;
    }
    catch (bad_lexical_cast&) {
        cerr << "Not a number!\n";
    }
}

int calc(string str)
{
    if (valid(str))
        return 1;//calculate(get_first(str), get_second(str));
    else
        try {
            int ii = lexical_cast<int> (str);
            return ii;
        }
        catch (bad_lexical_cast&) {
            cerr <<str<<", not a number\n";
        }
}

int main()
{
    /*string str ("FFF2777");
    if (valid(str))
        cout<<get_first(str)<<", "<<get_second(str)<<endl;
    */

    string in, stack;
    string out[200], real_out[200]; //200 операций хватит всем, если больше, то будет плохо...
    int i_out=0; //Счётчик выходного массива строк
    cin>>in;
    //просматриваем посимвольно, если операции, то что то делаем
    int l = in.length();

    for (int i=0; i<l; i++)
        if (prior(in[i])==-1)
            out[i_out]+=in[i];
        else
        {
            //if ((prior(in[i])!=0)&&(prior(in[i])!=1))
                i_out++;
            if (stack.length()==0)
            {
                stack=in[i];
                //cout<<"Стек пуст, добавляем операцию "<<in[i]<<endl;
            }
            else
            {
                int stack_l=stack.length();
                if (prior(in[i])==1)
                    stack=in[i]+stack;
                else
                {
                    if (prior(in[i])==0)
                    {
                        for (int j=0; j<stack_l; j++)
                        {
                            if (prior(stack[j])!=1)
                            {
                                out[i_out]=stack[j];
                                i_out++;
                                stack.erase(j,1);
                                stack_l--;
                                j--;
                            }
                            else
                            {
                                stack.erase(j,1);
                                break;
                            }
                        }
                    }
                    else
                    {
                        //Выталкиваем всё в вывод, если приоритет больше или равен
                        if (prior(stack[0])>=prior(in[i]))
                            for (int j=0; j<stack_l; j++)
                                if (prior(stack[j])>=prior(in[i]))
                                {
                                    out[i_out]=stack[j];
                                    i_out++;
                                    stack.erase(j,1);
                                    stack_l--;
                                    j--;
                                }
                        //Ну и добавляем в стек
                        stack=in[i]+stack;
                    }
                }
             }

        }
    for (int i=0; i<stack.length(); i++)
    {
        i_out++;
        out[i_out]=stack[i];
    }
    int r_i=0;
    for (int i=0; i<=i_out; i++)
    {
        if (out[i]!="")
        {
            real_out[r_i]=out[i];//готовая польская запись
            //cout<<out[i]<<endl;
            r_i++;
        }
    }

    //Вычислим по польской записи...
    string st[200];
    int st_i=0;
    for (int i=0; i<=i_out; i++)
    {
        //Если нам попалась бинарная операция, выполним её с двумя предыдущими элементами стека
        if (real_out[i]=="*")
        {
            st[st_i-2]=lexical_cast<string> (calc(st[st_i-2])*calc(st[st_i-1]));
            st[st_i-1]="";
            st_i=st_i-2;
        }
        else
            if (real_out[i]=="/")
            {
                st[st_i-2]=lexical_cast<string> (calc(st[st_i-2])/calc(st[st_i-1]));
                st[st_i-1]="";
                st_i=st_i-2;
            }
            else
                if (real_out[i]=="+")
                {
                    st[st_i-2]=lexical_cast<string> (calc(st[st_i-2])+calc(st[st_i-1]));
                    st[st_i-1]="";
                    st_i=st_i-2;
                }
                else
                    if (real_out[i]=="-")
                    {
                        st[st_i-2]=lexical_cast<string> (calc(st[st_i-2])-calc(st[st_i-1]));
                        st[st_i-1]="";
                        st_i=st_i-2;
                     }
                     else
                        st[st_i]=real_out[i];
        st_i++;
    }

    cout<<st[0]<<endl;
    return 0;
}
