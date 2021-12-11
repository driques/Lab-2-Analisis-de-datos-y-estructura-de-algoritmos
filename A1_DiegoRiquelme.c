/*
Diego Riquelme
20.595.397-3
Profesora Violeta Chang A1
Analisis de datos y estructura de algoritmos
*/

////////////////////////////////////////////////////////////////
#include <time.h>
////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "TDAsongs.h"


//funcion que nos entrega el tamaño del archivo, retorna el tamaño, tiene como entrada un 
//array con el nombre del archivo a revisar.
int fileSize(char fileName[]){
	FILE *dataFile;
	char num[9];
	dataFile = fopen(fileName,"r");
	fgets(num,9,dataFile);
	int initNum=atoi(num);
    fclose(dataFile);
	return initNum;
}


//Función que abre el archivo del tipo listado, tiene como entrada el tamaño del listado y el nombre 
//Del archivo, retorna un array struct con los datos correspondientes al mismo.
TDAlista *openFile(int fileSize,char fileName[]){
   
    TDAlista * listSongs=crearListaVacia();


    char space[2] = " ";
    char *zeros="00:0";
    char *token;
    char charData[255];
    char dato;
    FILE *dataFile;
	dataFile = fopen(fileName,"r");
	int i=0;
	int k=0;
	while (i<=fileSize){
	 	fgets(charData,100,dataFile);
        
 		if (i>0){

            songs fileSong;
            strtok(charData,"\n");
            token = strtok(charData,space);
            int j=0;
            while( token != NULL ){
                if (j==0){
                   
                    //stpcpy(arraySongs[k].nameSong,token);
                    stpcpy(fileSong.nameSong,token);
                }
                if (j==1){
                   
                    //arraySongs[k].genre=atoi(token);
                    fileSong.genre=atoi(token);
                   
                }
                if (j==2){
                    /*Las siguientes lineas permiten transformar del 
                    MM:SS a HH:MM:SS*/
                    char newConcat[9]="";
                    
                    strcat(newConcat,zeros);
                   
                    strcat(newConcat,token);
                    
            
                    
                    strcpy(fileSong.duration,newConcat);

                    
                }
                if (j==3){
                    
                    strcpy(fileSong.artist,token);   
                }
                
                j++;
                token = strtok(NULL, space);
            }
            insertarInicio(listSongs,fileSong);
            k=k+1;
	 	}
	 	i=i+1;
    }
    fclose(dataFile);
    return listSongs; 
    
   
}


/*Función que permite obtener los datos desde el archivo destinado a la playlist
Tiene como entrada el tamaño del archivo, el nombre y el puntero de un id para guardar 
el genero de la lista a crear, retorna un array struct del tipo playContent*/

playContent *playCapture(int size,char fileName[], int *idPlayList){
    playContent * content =  (playContent *)malloc(sizeof(playContent)*(size+1));
    FILE *dataFile;
    char charData[255];
    char *token;
    dataFile = fopen(fileName,"r");
    int genre;
    int i=0;
    while (i<=size){
        fgets(charData,100,dataFile);
        if (i==0){
            char duration[7];
            genre=atoi(&charData[2]);
            for (int j = 0; j < 8; j++){
                duration[j]=charData[4+j];
            }
            strtok(duration," ");
            
            strcpy(content[i].listDuration,duration);
        }
        else{
            token = strtok(charData," ");
            int j=0;
            while( token != NULL ){
                if (j==1){
                    strcpy(content[i-1].listGenre,token);
                }
                
                token = strtok(NULL," ");
                j++;   
            }
            
        }
        
        i++;
    }
    *idPlayList=genre;
    fclose(dataFile);
    return content;



}
/*Función que permite transformar un array del tipo HH:MM:SS a segundos con el fin de
trabajar más facilmente el calculo de la mejor lista, tiene como entrada el array en cuestion, y retorna 
int segundos.*/
int toSeconds(char hours[]){
    char hoursCopy[8];
    strcpy(hoursCopy,hours);
    char *token;
    int seconds=0;
    token = strtok(hoursCopy,":");
    int j=0;
    while( j<3){
        
        if (j==0){
            seconds=seconds+(atoi(token)*3600);
        }
        if (j==1){
            seconds=seconds+(atoi(token)*60);
        }
        if (j==2){
            seconds=seconds+(atoi(token));
            
            return seconds;
        }
        j++;
        token = strtok(NULL, ":");
    }
   
}

/*La funcion toHours ademas de transformar los segundos al formato HH:MM:SS, exporta
los datos al archivo .out con la playlist lista. */
void toHours(int sec,int quantSongs,char genre[],TDAlista *playlist,char salida[]){
    FILE *playlistFile = fopen(salida,"w");
    int hr=0, min=0;
    char *hourReturn;
    hr = sec /3600;
    sec = sec % 3600;
    min = sec / 60;
    sec %= 60;
    strtok(genre,"\n");
    //Creacion del archivo
    fprintf(playlistFile,"%d %d:%d:%d %s\n",quantSongs,hr,min,sec,genre);
    
    nodo * auxiliar=playlist->inicio;
    while (auxiliar!=NULL)
    {
      songs actualSong=auxiliar->dato;
      fprintf(playlistFile,"%s ",actualSong.nameSong);
        fprintf(playlistFile,"%s ",actualSong.duration);
        fprintf(playlistFile,"%s\n",actualSong.artist);
       
      auxiliar=auxiliar->siguiente;
    }

    fclose(playlistFile);

    
    
    
    

}

/*Funcion que verifica que no se encuentre una canción ya seleccionada, tiene como entrada el struct array de canciones generadas y su largo, junto con
la canción a comparar, y retorna un 0 o un 1 si es que se encuentra o no dentro de este.*/

int notInSongs(songs sameGenre[],char song[], int sizeSameGenre){
    for (int i = 0; i < sizeSameGenre; i++){
        if (strcmp(song,sameGenre[i].nameSong)==0){
            return 1;
        }    
    }
    return 0;
    
}
   



/*Función creadora de la playlist, tiene como entrada el genero de la playlist, la lista de structs de canciones, el tamanio de todas
y 2 punteros para guardar el tamanio de la playlist generada, y el tiempo de esta*/

songs* makePlayList(int genre,TDAlista *arrayOfSongs,  int *lenPlaylist, int * timePlaylist){
   
    songs *sameGenre;
    
    //Se verifica cuantas canciones pertenecen a un mismo genero
    int amountSongs = recorrerListaGenero(arrayOfSongs, genre);

    printf("Cantidad mismas canciones: %d\n",amountSongs);

    //Se genera un array struct para guardar la playlist del mismo genero.
    //Luego se procede a ordenar.
    sameGenre= (songs *)malloc(sizeof(songs)*amountSongs);
    int k=0;
    nodo * auxiliar=arrayOfSongs->inicio;
    while (auxiliar!=NULL)
    {
      songs actualSong=auxiliar->dato;
       if (actualSong.genre==genre && notInSongs(sameGenre,actualSong.nameSong,amountSongs)==0){
                    sameGenre[k]=actualSong;
                    k++;
                
            }
      auxiliar=auxiliar->siguiente;
    }
    amountSongs=k;


    printf("Cantidad mismas canciones sin repetir: %d\n",amountSongs);
       //Bubble Sort
    for (int i = 0; i < amountSongs; i++){
           for (int j = 0; j < amountSongs-1-i; j++){
                if (toSeconds(sameGenre[j].duration)<toSeconds(sameGenre[j+1].duration)){
                        songs temporal=sameGenre[j];
                        sameGenre[j]=sameGenre[j+1];
                        sameGenre[j+1]=temporal;
                    
            }
       }
       
    }

    int totalSeconds=0;
    for (int i = 0; i < amountSongs; ++i){
       
        totalSeconds=totalSeconds+toSeconds(sameGenre[i].duration);
    }
    *lenPlaylist=amountSongs;
    *timePlaylist=totalSeconds;
    return sameGenre;
       
}



/*Funcion que busca coincidencias entre la lista de reclamos y la playlist generada, tiene como entrada una lista de reclamos, la playlist
y 2 enteros para calcular el tiempo y el largo de la nueva playlist.*/

void coincidenciasReclamos (TDAlista * reclamos, TDAlista * playlist,  int *lenPlaylist, int * timePlaylist){
       
    nodo * rclms=reclamos->inicio;
    nodo * pList=playlist->inicio;

    TDAlista *listaNueva=crearListaVacia();
   //Se buscan las canciones con copyRight
    while (pList!=NULL){
        
        nodo * aux=rclms;
        songs pListAutor = pList->dato; 

        while (aux!=NULL){
            songs rclmAutor = aux->dato;
            char *copyRightArtist = pListAutor.artist;
            
            char* token= strtok(pListAutor.artist,"\n");
            
            if (strcmp(token,rclmAutor.nameSong)==0){
               insertarInicio(listaNueva,pListAutor);
            }
            aux = aux->siguiente;
      
        }
        pList=pList->siguiente;
    }  
    TDAlista *listaSinCopyright=crearListaVacia();
    nodo * nodoFinal = listaNueva->inicio;
    
    int amountSongs=0;
    //Se eliminan las canciones con copyRight.
    while (nodoFinal!=NULL){
        eliminarNodoDato(playlist,nodoFinal->dato);
        nodoFinal = nodoFinal->siguiente;
        amountSongs++;
    }
    int totalSeconds=0;
    nodo * playlistFinal = playlist->inicio;
   //Calculo tiempo
    while(playlistFinal!=NULL){
        totalSeconds=totalSeconds+toSeconds((playlistFinal->dato).duration);
        playlistFinal=playlistFinal->siguiente;
    }
    *lenPlaylist=amountSongs;
    *timePlaylist=totalSeconds;



}


/*Funcion driver*/

int main(int argc, char** argv){
    //////////////////////////////////////////////
    int num=0;
	int fiboi=0;
	int fibor=0;
	time_t tiniIt, tfinIt, tiniRe, tfinRe;
	clock_t ciniIt, cfinIt, ciniRe, cfinRe;
    ciniIt=clock();
	//inicio tiempo




    //Generacion de variables a utilizar
    char lista[]="listado.in";
    char play[]="play.in";
    int genrePlayList=0;
    char *strGenre;
    int totalSecondsPlaylist;
    int lenPlaylist=0;
    int len = fileSize(argv[1]);
    int lenReclamos=fileSize(argv[3]);

    //Invocacion de funciones
    TDAlista *dataList= openFile(len,argv[1]);
    playContent * dataPlaylist=playCapture(len,argv[2],&genrePlayList);    
    songs* playlist=makePlayList(genrePlayList,dataList, &lenPlaylist,&totalSecondsPlaylist);
    TDAlista *songsFinal=crearListaVacia();

    TDAlista *reclamos = openFile(lenReclamos,argv[3]);


    for(int i=0;i<lenPlaylist;i++)
    {
      insertarInicio(songsFinal,playlist[i]);
    }
  //Aqui implementación de orden de guardado (A C D)
  //Sea D o cualquier cosa, o sin este parametro, se ordenará de la misma forma.   
    if (argc>4)
    {
           
    if (strcmp(argv[4],"A")==0)
    {
        ordenAutor(songsFinal);
    }
      else if(strcmp(argv[4],"C")==0)
    {
        ordenNombreCancion(songsFinal);
    }
    }
    


    
    //Guardado
    toHours(totalSecondsPlaylist,lenPlaylist,dataPlaylist[genrePlayList-1].listGenre, songsFinal,"playlistLinea.out");
    printf("Playlist generada con exito en el archivo playlistLinea.out\n");


    int lenCopyright = 0;
    int secondsCopyright = 0;

    coincidenciasReclamos(reclamos,songsFinal,&lenCopyright,&secondsCopyright);

    
    toHours(secondsCopyright,(lenPlaylist-lenCopyright),dataPlaylist[genrePlayList-1].listGenre, songsFinal,"playlistTienda.out");
    printf("Playlist sin copyRight generada con exito en el archivo playlistTienda.out\n");

    liberaNodos(songsFinal->inicio);
    liberaNodos(dataList->inicio);
    free(dataPlaylist);
    free(playlist);
    //////////////////////////////////////////////
    
  
    cfinIt=clock();
    printf("Tiempo utilizado generacion playList:  %f segundos.\n",(double)(cfinIt-ciniIt)/CLOCKS_PER_SEC);
    return 0;
    
}