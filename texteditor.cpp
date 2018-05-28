/**
    texteditor.cpp
    Prop�sito: Editor de texto sencillo que puede crear archivos, leer archivos de texto existentes
    a�n y que no hayan sido creados por el programa, como copiar contenidos de un archivo a otro.

    @author Andr�s Pi��n Mar�n
    @author Tania Lizeth Ya�ez Aguilar
    @author Miguel Angel Bui Rosas

    @version 1.0 5/29/18
*/

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <conio.h>
#include <thread>
#include <windows.h>

using namespace std;

class ErrFile : public exception
{
public:
    const char *  what() const throw(){
        return "File already exists, do you want to overwrite it? [Y/N]";
    }
}ErrF;

class FileDont : public exception
{
public:
    const char *  what() const throw(){
        return "Your file does not exist, do you want to try again? [Y/N]";
    }
}FDont;

/** \brief Funci�n gotoxy: Mueve el cursor en pantalla.
 *
 * \param column  Indica el valor de la columna en la que el puntero se encuentra en eje X
 * \param line  Indica el valor de la fila en la que el puntero se encuentra en eje Y.
 * \return Posici�n del puntero en pantalla detallada por otras funciones.
 *
 */
void gotoxy( int column, int line )
  {
  COORD coord;
  coord.X = column;
  coord.Y = line;
  SetConsoleCursorPosition(
    GetStdHandle( STD_OUTPUT_HANDLE ),
    coord
    );
  }

  /** \brief Cambia el color del texto y el fondo dependiendo de los valores dados al par�metro.
   *
   * \param color Valor que usar� la funci�n para cambiar el color del texto / fondo y/o una combinaci�n de estos dos
   * \return Nuevo color en pantalla o texto
   *
   */
void setColor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,color);
}

/** \brief Funci�n drawLinesMenu: Dibuja l�neas en pantalla para delimitar la interfaz el men�
 *
 * \param x Es el valor m�ximo al cual las l�neas ser�n dibujadas en el eje X
 * \param y Es el valor m�ximo al cual las l�neas ser�n dibujadas en el eje Y
 * \param x1 Es el valor m�nimo al cual las l�neas ser�n dibujadas en el eje X
 * \param y1 Es el valor m�nimo al cual las l�neas ser�n dibujadas en el eje Y
 * \return L�neas del men�
 *
 */
void drawLinesMenu(int x, int y,int x1, int y1)
{
    int i = 0, j = 0;
    for(i = x1; i < x-1; i++){
        for(j = y1+1; j < y ; j++){
            gotoxy(x1,j); cout <<char(179);
            gotoxy(x,j); cout <<char(179);
        }
        gotoxy(i+1,y1); cout << char(196);
        gotoxy(i+1,y); cout << char(196);
    }
    gotoxy(x,y1);cout << char(191);
    gotoxy(x,y);cout << char(217);
    gotoxy(x1,y1);cout<< char(218);
    gotoxy(x1,y);cout<< char(192);
}

/** \brief Funci�n helpFunc: Muestra un mensaje de ayuda al usuario del programa
 *
 * \return Imprime un cuadro de texto en pantalla con informaci�n �til
 *
 */
void helpFunc(){
    drawLinesMenu(60,19,20,15);
    gotoxy(22,16);
    cout << "Use the navigation arrows to choose";
    gotoxy(22,17);
    cout << "an option, press enter to confirm";
    gotoxy(26,18);
    cout << "> Use enter key to continue <";
    cin.get();
    for(int i = 20; i < 61; i++){
            for(int j = 15; j < 20;j++){
                gotoxy(i,j);cout<<" ";}
    }
}

/** \brief Funci�n descMenu: Funcion que toma un hilo, que muestra descripci�n,
 *
 * \param menu Este par�metro tomado de la funcion menuSelection, toma valores de entre 6 a 9 dependiendo la funci�n anterior
 * \return Despliega dscripci�n en pantalla de la opci�n seleccionada dependiendo del valor menu.
 *
 */
void descMenu(int menu)
{
        if(menu == 6)
        {
            for(int i = 29; i < 80; i++){for(int j = 6; j < 14;j++){gotoxy(i,j);cout<<" ";}}
            gotoxy(29,6);
            cout<< "Creates a new text document.";

        }
        if(menu == 7)
        {
            for(int i = 29; i < 80; i++){for(int j = 6; j < 14;j++){gotoxy(i,j);cout<<" ";}}
            gotoxy(29,6);
            cout<< "Allows the user to read existent archives.";
        }
        if(menu == 8)
        {
            for(int i = 29; i < 80; i++){for(int j = 6; j < 14;j++){gotoxy(i,j);cout<<" ";}}
            gotoxy(29,6);
            cout<< "This option copies an archive's text to another.";
        }
}

/** \brief Funci�n menuSelection: Selecci�n de opciones
 *
 * \param nav este par�metro indica el valor booleano de navegaci�n dentro del men�, el men� sigue corriendo hasta que este sea verdadero.
 * \param exit este par�metro indica el valor booleano de salida, el programa se seguir� ejecutando hasta que este sea verdadero.
 * \param menu este par�metro es el valor del men� que la opci�n toma como referencia.
 * \return Despliega el men� para que el usuario pueda interactuar con el programa.
 */
void menuSelection(bool &nav, bool &exit,int &menu){
    char key;
    while(!nav){
        setColor(286);
        key = getch ();
        switch(key)
        {
        case 72:
            gotoxy(4,menu);cout<<" "; menu--; if(menu < 6){menu = 6;} gotoxy(4,menu);cout <<">"; break;
        case 80:
            gotoxy(4,menu);cout<<" "; menu++; if(menu > 8){menu = 8;} gotoxy(4,menu);cout <<">"; break;
        case 13:
            nav = true; break;
        case 59:
        case 'h':
            helpFunc(); break;
        case 'Q':
        case 'q':
            gotoxy(3,16); cout <<"Terminating program..."; Sleep(1500);
            menu = 0;
            exit = true; nav = true; break;
        }
        thread des(descMenu,menu);
        des.join();
    }
}

/** \brief Funci�n subMenu: Sub men� de ayuda para usuario
 *
 * \return Despliega un submen� que contiene el d�a / mes / a�o y hora de inicio, as� como el despliegue
 *  del men� ayuda.
 */
void subMenu(){
        gotoxy(0,0);
        Sleep(150);
        setColor(432);
        for(int i = 0; i < 80; i++){
            gotoxy(i,0);cout<<" ";
        }
        time_t current = time(0);
        gotoxy(0,0);
        cout << "[307737 - 317807 - 320704] \t"<< ctime(&current)<< endl;
        gotoxy(65,0);
        cout << "Press [F1] for Help";
        setColor(287);
        Sleep(150);
        gotoxy(3,18);
        cout<<"[|H|elp]";
        gotoxy(72,18);
        cout<<"[|Q|uit]";
}
/** \brief Funci�n clsScreen: Borra parte de la pantalla al imprimir espacios.
 */
void clsScreen()
{
    for(int i = 0; i < 125; i++){
            for(int j = 5; j < 24;j++){
                gotoxy(i,j);cout<<" ";
            }
        }
}

/** \brief Funci�n copyArchive: Indica al usuario qu� archivo fuente y destino quiere abrir
 *
 * \param nav este par�metro indica el valor booleano de navegaci�n dentro del men�, el men� sigue corriendo hasta que este sea verdadero.
 * \return Copia los contenidos del archivo fuente al archivo destino.
 *
 */
void copyArchive(bool &nav)
{
    string name, text, ext = ".txt", f_name, f_name2;
    ifstream file_1;
    ofstream file_2;
    char opt, contents;
    bool bopt = false;
    gotoxy(3,6);
    while(!bopt){
        clsScreen();
        gotoxy(0,6);
        try{
            cout << "Type the source file name you want to copy: ";
            getline(cin,name);
            f_name = name + ext;
            file_1.open(f_name);
            if(!file_1)
            {
                throw FDont;
            }
            cout << "\nType target file name: ";
            getline(cin,name);
            f_name2 = name + ext;
            file_2.open(f_name2);
            if(!file_2)
            {
                throw FDont;
            }
            while(file_1.eof()== 0 )
            {

                file_1.get(contents);
                file_2 << contents;
            }
            cout <<"\n\nFile copied sucessfully . . . ! Press any key to return to main menu. . . " ;
            file_1.close();
            file_2.close();
            bopt = true;

            system("pause>NULL");
        }
        catch (FileDont &e){
            clsScreen();
            gotoxy(3,5);
            cout << "ERROR: " << e.what() << "\n" << endl;
            opt = getch ();
            switch (opt)
            {
            case 'Y':
            case 'y':
                break;
            case 'N':
            case 'n':
                bopt = true;
                cout << "Returning to main menu..." << endl;
                Sleep(500);
                break;
            }
        }
        catch (...){
            cout << "SYSTEM ERROR: FATAL EXCEPTION\n";
            system("pause");
        }
    }
    clsScreen();
    nav = false;

}

/** \brief Funci�n openArchive: Pregunta al usuario cu�l archivo desea leer.
 *
 * \param nav este par�metro indica el valor booleano de navegaci�n dentro del men�, el men� sigue corriendo hasta que este sea verdadero.
 * \return Despliega en pantalla el contenido archivo del usuario s� y s�lo s� este existe.
 */
void openArchive(bool &nav)
{
    string name, text, ext = ".txt", f_name, add_text;
    char opt;
    bool bopt = false;

    while(!bopt){
        clsScreen();
        gotoxy(3,6);
        try{
            cout << "Type the name of the archive you want to read: ";
            getline(cin,name);
            f_name = name + ext;
            fstream file(f_name);
            if(!file.good())
            {
                throw FDont;
            }
            else
            {
                ifstream read(f_name);
                gotoxy(3,5);
                setColor(432);
                cout <<"Success! Current archive: " << f_name << endl;
                setColor(287);
                gotoxy(0,8);
                cout << "Your archive's text:\n";
                if (read.is_open())
                    cout << read.rdbuf();
                cout << "\n\nYour file has been read, press any key to return to main menu. . .";
                system("pause>null");
                read.close();
                bopt = true;


            }
        }
        catch (FileDont &e){
            clsScreen();
            gotoxy(3,5);
            cout << "ERROR: " << e.what() << "\n" << endl;
            opt = getch ();
            switch (opt)
            {
            case 'Y':
            case 'y':
                break;
            case 'N':
            case 'n':
                bopt = true;
                cout << "Returning to main menu..." << endl;
                Sleep(500);
                break;
            }
        }
        catch (...){
            cout << "SYSTEM ERROR: FATAL EXCEPTION\n";
            system("pause");
        }
    }
    clsScreen();
    nav = false;
}

/** \brief Funci�n crA:
 *
 * \param text Es la cadena ( string ) de texto que el usuario escribir�
 * \param f_name es el nombre actual del archivo
 * \return La cadena text se guarda dentro del archivo por nombre dado por el usuario.
 *
 */
void crA(string text,string f_name)
{
    ofstream c_archive(f_name.c_str());
    gotoxy(0,7);
    cout<< "Enter your text here:\n";
    getline(cin,text);
    c_archive<<text;
    cout << "\n\n" <<f_name<<" has been saved. . . ";
    c_archive.close();
    system("pause");
}

/** \brief Funci�n createArchive: Pide al usuario el nombre del archivo que se va a crear
 *
 * \param nav este par�metro indica el valor booleano de navegaci�n dentro del men�, el men� sigue corriendo hasta que este sea verdadero.
 * \return Crea el archivo en cuesti�n y manda el nombre del archivo a la funci�n crA para crear el contenido del mismo.
 */
void createArchive(bool &nav)
{
    string name, text, ext = ".txt", f_name;
    char opt;
    bool bopt = false;
    while(!bopt){
        clsScreen();
        gotoxy(3,6);
        try{
            cout <<"Type the name of your text file here: ";
            getline(cin,name);
            f_name = name+ext;
            fstream file(f_name);
            if(file.good())
            {
                throw ErrF;
            }
            else
            {
                gotoxy(3,5);
                setColor(432);
                cout <<"Success! Current archive: " << f_name << endl;
                setColor(287);
                crA(text,f_name);
                bopt = true;
            }
        }
        catch (ErrFile &e){
            clsScreen();
            gotoxy(3,5);
            cout << "ERROR: " << e.what() << "\n" << endl;
            opt = getch ();
            switch (opt)
            {
            case 'Y':
            case 'y':
                clsScreen();
                crA(text,f_name);
                bopt = true;
                break;
            case 'N':
            case 'n':
                bopt = true;
                cout << "Returning to main menu..." << endl;
                Sleep(500);
                break;
            }
        }
        catch (...){
            cout << "SYSTEM ERROR: FATAL EXCEPTION\n";
            system("pause");
        }
    }
    clsScreen();
    nav = false;
}

/**< Controla la operaci�n del programa y despliegue del men� principal */
int main()
{
    bool nav = false;
    bool exit = false;
    int menu = 6;

    string opt[3] = {" CREATE NEW ARCHIVE"," READ ARCHIVE", " COPY ARCHIVE"};
    system("Color 10");
    setColor(287);

    while(!exit){
        while(!nav){
            thread date(&subMenu);
            drawLinesMenu(80,4,3,2); drawLinesMenu(26,14,3,5); drawLinesMenu(80,14,28,5);
            setColor(286);
            gotoxy(4,3); cout <<"\t\t\t\tTEXT EDITOR"<< endl;
            setColor(287);
            gotoxy(4,6); cout << opt[0]; gotoxy(4,7); cout<<opt[1]; gotoxy(4,8); cout<<opt[2];
            menuSelection(nav,exit,menu);
            setColor(287);
            date.join();
        }
        if(menu == 6)
            createArchive(nav);
        if(menu == 7)
            openArchive(nav);
        if(menu == 8)
            copyArchive(nav);
    }
    return 0;
}
