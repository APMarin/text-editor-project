/**
    texteditor.cpp
    Propósito: Editor de texto sencillo que puede crear archivos, leer archivos de texto existentes
    aún y que no hayan sido creados por el programa, como copiar contenidos de un archivo a otro.

    @author Andrés Piñón Marín
    @author Tania Lizeth Yañez Aguilar
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

/** \brief Función gotoxy: Mueve el cursor en pantalla.
 *
 * \param column  Indica el valor de la columna en la que el puntero se encuentra en eje X
 * \param line  Indica el valor de la fila en la que el puntero se encuentra en eje Y.
 * \return Posición del puntero en pantalla detallada por otras funciones.
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

  /** \brief Cambia el color del texto y el fondo dependiendo de los valores dados al parámetro.
   *
   * \param color Valor que usará la función para cambiar el color del texto / fondo y/o una combinación de estos dos
   * \return Nuevo color en pantalla o texto
   *
   */
void setColor(unsigned short color)
{
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon,color);
}

/** \brief Función drawLinesMenu: Dibuja líneas en pantalla para delimitar la interfaz el menú
 *
 * \param x Es el valor máximo al cual las líneas serán dibujadas en el eje X
 * \param y Es el valor máximo al cual las líneas serán dibujadas en el eje Y
 * \param x1 Es el valor mínimo al cual las líneas serán dibujadas en el eje X
 * \param y1 Es el valor mínimo al cual las líneas serán dibujadas en el eje Y
 * \return Líneas del menú
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

/** \brief Función helpFunc: Muestra un mensaje de ayuda al usuario del programa
 *
 * \return Imprime un cuadro de texto en pantalla con información útil
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

/** \brief Función descMenu: Funcion que toma un hilo, que muestra descripción,
 *
 * \param menu Este parámetro tomado de la funcion menuSelection, toma valores de entre 6 a 9 dependiendo la función anterior
 * \return Despliega dscripción en pantalla de la opción seleccionada dependiendo del valor menu.
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

/** \brief Función menuSelection: Selección de opciones
 *
 * \param nav este parámetro indica el valor booleano de navegación dentro del menú, el menú sigue corriendo hasta que este sea verdadero.
 * \param exit este parámetro indica el valor booleano de salida, el programa se seguirá ejecutando hasta que este sea verdadero.
 * \param menu este parámetro es el valor del menú que la opción toma como referencia.
 * \return Despliega el menú para que el usuario pueda interactuar con el programa.
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

/** \brief Función subMenu: Sub menú de ayuda para usuario
 *
 * \return Despliega un submenú que contiene el día / mes / año y hora de inicio, así como el despliegue
 *  del menú ayuda.
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
/** \brief Función clsScreen: Borra parte de la pantalla al imprimir espacios.
 */
void clsScreen()
{
    for(int i = 0; i < 125; i++){
            for(int j = 5; j < 24;j++){
                gotoxy(i,j);cout<<" ";
            }
        }
}

/** \brief Función copyArchive: Indica al usuario qué archivo fuente y destino quiere abrir
 *
 * \param nav este parámetro indica el valor booleano de navegación dentro del menú, el menú sigue corriendo hasta que este sea verdadero.
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

/** \brief Función openArchive: Pregunta al usuario cuál archivo desea leer.
 *
 * \param nav este parámetro indica el valor booleano de navegación dentro del menú, el menú sigue corriendo hasta que este sea verdadero.
 * \return Despliega en pantalla el contenido archivo del usuario sí y sólo sí este existe.
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

/** \brief Función crA:
 *
 * \param text Es la cadena ( string ) de texto que el usuario escribirá
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

/** \brief Función createArchive: Pide al usuario el nombre del archivo que se va a crear
 *
 * \param nav este parámetro indica el valor booleano de navegación dentro del menú, el menú sigue corriendo hasta que este sea verdadero.
 * \return Crea el archivo en cuestión y manda el nombre del archivo a la función crA para crear el contenido del mismo.
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

/**< Controla la operación del programa y despliegue del menú principal */
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
