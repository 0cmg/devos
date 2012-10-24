/**
 *	Koala Operating System
 *	Copyright (C) 2010 - 2011 Samy Pess�
 *	
 *	This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundatn 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
#include <os.h>

/*
 *	Definis la structure d'une variable d'environnement
 *	chaque variable est un fichier stoqu� dans le dossier virtuel /sys/env
 */

/*
 *	Destructeur de la variable
 */
Variable::~Variable(){
	if (value!=NULL)
		kfree(value);
}

/* 
 *	Constructeur :
 *		n : nom
 *		v : valeur
 */
Variable::Variable(char* n,char* v) : File(n,TYPE_FILE)
{
	fsm.addFile("/sys/env/",this);
	if (v!=NULL){
		io.print("env: create %s (%s) \n",n,v);
		value=(char*)kmalloc(strlen(v)+1);
		memcpy(value,v,strlen(v)+1);
		setSize(strlen(v)+1);
	}
	else{
		value=NULL;
	}
}

u32	Variable::open(u32 flag){
	return RETURN_OK;
}

u32	Variable::close(){
	return RETURN_OK;
}

/* lecture de la valeur dans buffer */
u32	Variable::read(u32 pos,u8* buffer,u32 size){
	if (value==NULL)
		return NOT_DEFINED;
	else{
		strncpy((char*)buffer,value,size);
		return size;		
	}
}

/* ecriture de buffer dans la variable */
u32	Variable::write(u32 pos,u8* buffer,u32 size){
	if (value!=NULL)
		kfree(value);
	value=(char*)kmalloc(size+1);
	memset((char*)value,0,size+1);
	memcpy(value,(char*)buffer,size+1);
	value[size]=0;	//to make sure it's a string
	setSize(size+1);
	return size;
}

/* controle de la variable (TODO) */
u32	Variable::ioctl(u32 id,u8* buffer){
	return NOT_DEFINED;
}

/* destruction de la variable */
u32	Variable::remove(){
	delete this;
	return NOT_DEFINED;
}

/* seulement pour les dossiers */
void Variable::scan(){

}

