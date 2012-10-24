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
#ifndef VMM_H
#define VMM_H

#include <runtime/types.h>
#include <runtime/list.h>
#include <runtime/alloc.h>
#include <x86.h>


extern "C" {

struct page {
		char *v_addr;
		char *p_addr;
		list_head list;
};

struct page_directory {
		page *base;
		list_head pt;
};

struct vm_area {
		char *vm_start;	
		char *vm_end;	/* exclude */
		list_head list;
};

typedef page_directory proc_memory;

	/* Pointe sur le sommet du heap noyau */
	extern char *kern_heap;

	/* Pointe sur le debut de la liste des pages libres du noyau */
	extern list_head kern_free_vm;


	extern u32 *pd0;
	extern u8 mem_bitmap[];

	extern u32 kmalloc_used;



	/* Marque une page comme utilisee / libre dans le bitmap */
	#define set_page_frame_used(page)	mem_bitmap[((u32) page)/8] |= (1 << (((u32) page)%8))
	#define release_page_frame(p_addr)	mem_bitmap[((u32) p_addr/PAGESIZE)/8] &= ~(1 << (((u32) p_addr/PAGESIZE)%8))

	/* Selectionne une page libre dans le bitmap */
	char *get_page_frame(void);

	/* Selectionne / libere une page libre dans le bitmap et l'associe a une page
	 * virtuelle libre du heap */
	struct page *get_page_from_heap(void);
	int release_page_from_heap(char *);

	/* Initialise les structures de donnees de gestion de la memoire */
	void Memory_init(u32 high_mem);

	/* Cree un repertoire de page pour un processus */
	struct page_directory *pd_create(void);
	int pd_destroy(struct page_directory *);
	struct page_directory *pd_copy(struct page_directory * pdfather);
	
	
	/* Ajoute une entree dans l'espace du noyau */
	int pd0_add_page(char *, char *, int);

	/* Ajoute / enleve une entree dans le repertoire de pages courant */
	int pd_add_page(char *, char *, int, struct page_directory *);
	int pd_remove_page(char *);

	/* Retourne l'adresse physique associee a une adresse virtuelle */
	char *get_p_addr(char *);

	
	#define KMALLOC_MINSIZE		16

	struct kmalloc_header {
		unsigned long size:31;	/* taille totale de l'enregistrement */
		unsigned long used:1;
	} __attribute__ ((packed));

}

class Vmm
{
	public:
		void			init(u32 high);
		proc_memory*	createPM();					/* Create page directory for a process */
		void			switchPM(proc_memory* ad);	/* Switch page directory for a process */
		void			map(proc_memory* ad,u32 phy,u32 adr);	/* map a physical page memory in virtual space */
		
		void			kmap(u32 phy,u32 virt);
		
};

extern Vmm vmm;

#endif
