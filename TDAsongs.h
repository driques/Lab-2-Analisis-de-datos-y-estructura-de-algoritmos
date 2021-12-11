

/*------------- estructura de datos -------------*/
//Generacion de struct para crear un array de structs con
//la información de cada canción, struct songs va orientado 
//al archivo que contiene las canciones, mientras que playContent 
//al que contiene la información necesaria para la playlist.
typedef struct songs songs;
typedef struct playContent playContent;
struct songs {
    char nameSong[25];
    int genre;
    char duration[9];
    char artist[25];
};


struct playContent{
    char listGenre[25];
    char listDuration[9]; 
};


typedef struct nodoGenerico
{
  songs dato;
  struct nodoGenerico* siguiente;
}nodo;

typedef struct listaGenerica
{
  nodo* inicio;
}TDAlista;



/*------------- operaciones -------------*/
//Genera una lista vacia, no tiene entradas.
TDAlista* crearListaVacia()
{
  TDAlista * lista=(TDAlista*)malloc(sizeof(TDAlista));
  lista->inicio=NULL;
  return lista;
}
//Funcion de pertenencia que pregunta si es una lista vacia o no.
int esListaVacia(TDAlista* lista)
{
  if (lista->inicio == NULL)
    return 1;
  else
    return 0;
}


//Funcion que imprime los nombres de las canciones de una lista de canciones,
void recorrerLista(TDAlista* lista)
{
  if (!esListaVacia(lista))
  {
    nodo * auxiliar=lista->inicio;
    while (auxiliar!=NULL)
    {
      songs actualSong=auxiliar->dato;

      printf("%s\n",actualSong.nameSong);
      auxiliar=auxiliar->siguiente;
    }
    printf("\n");
  }
  else
    printf("La lista está vacía\n");
}

//recorrerListaGenero permite obtener la cantidad de canciones pertenencientes a un mismo genero.
int recorrerListaGenero(TDAlista* lista, int genero)
{
  int amountSongs=0;
  if (!esListaVacia(lista))
  {
    nodo * auxiliar=lista->inicio;
    while (auxiliar!=NULL)
    {
      songs actualSong=auxiliar->dato;
       if (actualSong.genre==genero){
            amountSongs++;
        }
      auxiliar=auxiliar->siguiente;
    }
    return amountSongs;
  }
  else
    return 0;
}


//Funcion que inserta dato al inicio de la lista, tiene como entrada la lista
//y el dato
void insertarInicio(TDAlista* lista, songs dato)
{
  nodo * nuevo=(nodo*)malloc(sizeof(nodo));
  nuevo->dato=dato;
  nuevo->siguiente = lista->inicio;
  lista->inicio=nuevo;
}


//Funcion que elimina el inicio de la lista, tiene como entrada la lista.
void eliminarInicio(TDAlista* lista)
{
  nodo * auxiliar;
  if(!esListaVacia(lista))
  {
    auxiliar=lista->inicio;
    lista->inicio=lista->inicio->siguiente;
    free(auxiliar);
  }
}
//Funcion que elimina un nodo con un dato en especifico, en este caso el nombre del artista
//tiene como entrada la lista, y datos songs.
void eliminarNodoDato (TDAlista* lista, songs dato){
   
    nodo *indicado = lista->inicio;
    nodo * anterior = NULL;
   
    while (strcmp(indicado->dato.artist,dato.artist)!=0){
        anterior=indicado;
        indicado=indicado->siguiente;
    }
    if(anterior==NULL){
        eliminarInicio(lista);
    }
    else{
    anterior->siguiente = indicado->siguiente;
    }
    //free(indicado);

}

//Funcion que libera nodo.
void liberaNodos( nodo *cabeza){
    nodo *temporal;
    while( cabeza!=NULL){
        temporal=cabeza;
        cabeza = cabeza->siguiente;
        free(temporal);
    }
}


//Adicionales////


//Funcion que ordena la lista obtenida en orden de autores, tiene como entrada la lista.
void ordenAutor(TDAlista* lista){

    nodo * raiz=lista->inicio;
    nodo * actual=NULL;
      
    songs temp;
    int i=0;
   
    while (raiz!=NULL){
       
        actual = raiz->siguiente;


        while (actual!=NULL){
            songs firstSong = raiz->dato;  
            songs actualSong = actual->dato;  
            
            int order=strcmp(firstSong.artist,actualSong.artist);

            if (order>0){
                temp=raiz->dato;
                raiz->dato = actual->dato;
                actual->dato = temp;
            }
            actual = actual->siguiente;
      
        }
        
        raiz=raiz->siguiente;  
        i++;

    }

   


}


//Funcion que ordena la lista obtenida en orden de canciones, tiene como entrada la lista.

void ordenNombreCancion(TDAlista* lista){

   
    nodo * raiz=lista->inicio;
    nodo * actual=NULL;
      
    songs temp;
    int i=0;
   
    while (raiz!=NULL){
       
        actual = raiz->siguiente;


        while (actual!=NULL){
            songs firstSong = raiz->dato;  
            songs actualSong = actual->dato;  
            int order=strcmp(firstSong.nameSong,actualSong.nameSong);

            if (order>0){
                temp=raiz->dato;
                raiz->dato = actual->dato;
                actual->dato = temp;
            }
            actual = actual->siguiente;
      
        }
        raiz=raiz->siguiente;  
        i++;

    }
}







