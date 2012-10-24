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
#ifndef FILE_H
#define FILE_H

#include <runtime/types.h>

enum{
	TYPE_FILE,
	TYPE_DIRECTORY,
	TYPE_DEVICE,
	TYPE_PROCESS,
	TYPE_LINK
};


class File
{
	public:
		File(char* n,u8 t);
		~File();
		
		virtual u32		open(u32 flag);
		virtual u32		close();
		virtual u32		read(u32 pos,u8* buffer,u32 size);
		virtual u32		write(u32 pos,u8* buffer,u32 size);
		virtual u32		ioctl(u32 id,u8* buffer);
		virtual u32		remove();
		virtual void	scan();
		
		
		void	checkName();
		
		u32		addChild(File* n);
		File*	createChild(char* n,u8 t);
		File* 	find(char* n);
		u32 	mmap(u32 sizee,u32 flags,u32 offset,u32 prot);
		
		void	setSize(u32 t);
		void	setType(u8 t);
		void	setParent(File* n);
		void	setChild(File* n);
		void	setNext(File* n);
		void	setPrec(File* n);
		void	setLink(File* n);
		void	setName(char* n);
		
		char*	getName();
		File*	getParent();
		File*	getChild();
		File*	getNext();
		File*	getPrec();
		File*	getLink();
		u8		getType();
		u32		getSize();
		u32		getInode();
		
		stat_fs stat();
		
	protected:
		static u32 inode_system;
	
		char*	map_memory;	/* to mmap */
		
		char*	name;		/* Nom du fichier	*/
		u32		size;		/* Taille du fichier */
		u8		type;		/* Type de fichier */
		u32		inode;		/* Inode du fichier */
		File*	dev;		/* the master device, example : /dev/hda */
		File*	link;		/* the real file, if this file is a link */
		
		
		File*	master;	/* processus maitre ou NULL */
		
		File*	parent;
		File*	child;
		File*	next;
		File*	prec;
		
		File*	device;		/* This file is the device master of the current file */
};

#endif
