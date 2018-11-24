//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: notenedit.h                           //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 12.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#ifndef NOTENEDIT_H
#define NOTENEDIT_H

#include <QtGui>
#include <QLineEdit>

class NotenEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit NotenEdit(int id, QWidget *parent = 0);
    void SetNote(int noten_nummer);
    
signals:
    void ChangeNote(int id, int note_nr);

public slots:
    void OnTextEdited(const QString & text);

private:
    int CalcNotenNummer(int vollnote,int hablkennenug, int oktave);
    int ID;

    int VollNote;
    int Halbkennung;
    int Oktave;

    int NotenNummer;
};

#endif // NOTENEDIT_H
