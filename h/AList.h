#ifndef _PCBList_h_
#define _PCBList_h_

class PCB;

class List {

protected:
	struct Elem{
			void *pcb;
			Elem *next;
			Elem(void *pcb, Elem *next = 0){
				this->pcb = pcb;
				this->next = next;
			}

			~Elem(){
				pcb = 0;
				next = 0;
			}
		};
	Elem* first, *last;
	friend class PCB;
public:
	List();
	~List();
	void* get(); //samo uzima prvi element (pop i put kombinacija)
	void* pop(); //uklanja prvi i vraca ga
	void put(void* pcb);
	void remove(void* pcb);
};

#endif
