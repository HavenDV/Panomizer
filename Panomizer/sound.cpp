#include "sound.h"
#include <qfile.h>
#include <qdatastream.h>
#include <qdebug.h>

sound::sound(QObject *parent)
	: QObject(parent) {
}

sound::~sound() {
}

typedef short mytype;

void sound::sum(QString name1, QString name2, QString name3, QString name4, QString outName) {
	bool t=true;
	QFile f1(name1), f2(name2), f3(name3), f4(name4), outFile(outName);
	if (!outFile.open(QIODevice::WriteOnly))
        return;
	if (!f1.open(QIODevice::ReadOnly)||!f2.open(QIODevice::ReadOnly)||!f3.open(QIODevice::ReadOnly)||!f4.open(QIODevice::ReadOnly))
        return;
	QDataStream in1(&f1), out(&outFile);
	WAVHEADER header;
	f4.read((char*)&header, sizeof(WAVHEADER));
	f3.read((char*)&header, sizeof(WAVHEADER));
	f2.read((char*)&header, sizeof(WAVHEADER));
	f1.read((char*)&header, sizeof(WAVHEADER));
	qDebug() << "Sample rate: " << header.sampleRate
             << "\nChannels: " << header.numChannels
             << "\nBits per sample: " << header.bitsPerSample
			 << "\nmytype size: " << sizeof(mytype)
			 << "\nheader size: " << 4 + (8 + header.subchunk1Size) + ( 8 + header.subchunk2Size)
			 << "\nsub1 size: " << header.subchunk1Size
			 << "\nsub2 size: " << header.subchunk2Size
			 << "\nmyheader size: " << sizeof(WAVHEADER);
	int hsize=4 + (8 + header.subchunk1Size) + ( 8 + header.subchunk2Size);
	int need=hsize-sizeof(WAVHEADER);
	need=9999;  // :-( Fix for non standart header
	char buf[9999];
	if (need>0) {
		f1.read(buf, need);
		f2.read(buf, need);
		f3.read(buf, need);
		f4.read(buf, need);
	}

	QByteArray b1(f1.readAll()), b2(f2.readAll()), b3(f3.readAll()), b4(f4.readAll()), bout;
	int minsize=qMin(qMin(b1.size(), b2.size()), qMin(b3.size(), b4.size()));
	const int fsize=sizeof(mytype);
	for (int i=0; i<minsize; i=i+fsize) {
		char a1[fsize], a2[fsize], a3[fsize], a4[fsize], *ao;
		for (int j=0; j<fsize; ++j) {
			a1[j]=b1[i+j];
			a2[j]=b2[i+j];
			a3[j]=b3[i+j];
			a4[j]=b4[i+j];
		}
		mytype* f1, *f2, *f3, *f4, fo;
		f1=reinterpret_cast<mytype*>(a1);
		f2=reinterpret_cast<mytype*>(a2);
		f3=reinterpret_cast<mytype*>(a3);
		f4=reinterpret_cast<mytype*>(a4);
		fo=*f1+*f2+*f3+*f4;
		ao=reinterpret_cast<char*>(&fo);
		
		for (int j=0; j<fsize; ++j) {
			bout.append(ao[j]);
		}
	}
	bout.append('0');
 
	outFile.write((char*)&header, sizeof(WAVHEADER));
	outFile.write(buf, need);
	outFile.write(bout);
} 
