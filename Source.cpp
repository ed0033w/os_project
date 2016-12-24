#include<iostream>
#include<omp.h>
using namespace std;
omp_lock_t w;
omp_lock_t r;

class rw {

	char buf[10];
	int nr, nre, nwr;
public:
	rw() {
		nre = 0;
		nwr = 0;
		nr = 0;
	}
	int getreader() {
		return nre;
	}
	int getwriter() {
		return nwr;
	}
	void number() {
		cout << "\nEnter the number of readers and writers:-";
		cin >> nre >> nwr;
	}
	void initwrite() {
		cout << "Write something:-";
		cin >> buf;
	}
	void startread() {
		omp_set_lock(&r);
		nr++;
		if (nr == 1) {
			omp_set_lock(&w);
		}
		omp_unset_lock(&r);
	}


	void endread() {
		omp_set_lock(&r);
		nr--;
		if (nr == 0) {
			omp_unset_lock(&w);
		}
		omp_unset_lock(&r);
	}

	void startwrite() {
		omp_set_lock(&w);
	}

	void endwrite() {
		omp_unset_lock(&w);
	}

	void reader() {
		startread();
		cout << "reading content:- " << buf << "\n";
		endread();
	}
	void writer() {
		startwrite();
		cout << "\noverwrite something:-";
		cin >> buf;
		endwrite();
	}
};
int main()
{
	int i, id, numthr, nwr, nre;
	omp_set_num_threads(8);
	rw r1;
	r1.number();
	r1.initwrite();
	nwr = r1.getwriter();
	nre = r1.getreader();
	omp_init_lock(&w);
	omp_init_lock(&r);
#pragma omp parallel
	{
#pragma omp for

		for (i = 0; i<(nre + nwr); i++)
		{
			if (i<nre) {
				r1.reader();
			}
			else {
				r1.writer();
			}

		}
	}
	return 0;
}