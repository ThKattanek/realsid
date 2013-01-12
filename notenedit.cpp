//////////////////////////////////////////////////
//						//
// realSID                                      //
// von Thorsten Kattanek			//
//                                              //
// #file: notenedit.cpp                         //
//						//
// Geistiges Eigentum von Thorsten Kattanek	//
//						//
// Letzte Änderung am 12.01.2013		//
//      					//
//						//
//////////////////////////////////////////////////

#include "notenedit.h"

const QString VollNoten[7]={"C","D","E","F","G","A","H"};
const int VollNotenPos[7]={0,2,4,5,7,9,11};
const bool    Halbnoten[7]={true,true,false,true,true,true,false};
const QString Oktaven[7]={"0","1","2","3","4","5","6"};
const QString NotenString[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","H"};

NotenEdit::NotenEdit(int id, QWidget *parent) :
    QLineEdit(parent)
{
    ID = id;
    connect(this,SIGNAL(textEdited(QString)),this,SLOT(OnTextEdited(QString)));
    this->setMaxLength(3);
}

void NotenEdit::SetNote(int noten_nummer)
{
    if(noten_nummer > 83 && noten_nummer != 255) return;
        NotenNummer = noten_nummer;

    if(noten_nummer == 255)
    {
        this->setText("");
        return;
    }

    Oktave = noten_nummer / 12;       // Nachkomma des Ergebnisses wird hier nicht gespeichert !

    this->setText(NotenString[noten_nummer - (Oktave * 12)]+QVariant(Oktave).toString());
}

void NotenEdit::OnTextEdited(const QString &text)
{
    bool found = false;

    QString input = text.toUpper();

    if(text == "")
    {
        NotenNummer = 255;
        emit ChangeNote(ID,NotenNummer);
    }

    switch(text.length())
    {
    case 1:
        for(int i=0;i<7;i++)
            if(input == VollNoten[i])
            {
                VollNote = i;
                found = true;
            }

        if(found)
            this->setText(input);
        else
        {
            VollNote = -1;
            this->setText("");
        }
        break;

    case 2:
        if((input.mid(1,1) == "#") && (Halbnoten[VollNote]) ==  true)
        {
            Halbkennung = true;
            this->setText(input);
        }
        else
        {
            Halbkennung = false;
            for(int i=0;i<7;i++)
                if(input.mid(1,1) == Oktaven[i])
                {
                    found = true;
                    Oktave = i;
                }
            if(found)
            {
                this->setText(input);
                NotenNummer = CalcNotenNummer(VollNote,Halbkennung,Oktave);
                /// Fertige Note via emit senden
                emit ChangeNote(ID,NotenNummer);
            }
            else this->setText(input.left(1));
        }

        break;

    case 3:
        if(Halbkennung)
        {
            for(int i=0;i<7;i++)
                if(input.mid(2,1) == Oktaven[i])
                {
                    found = true;
                    Oktave = i;
                }
            if(found)
            {
                this->setText(input);
                NotenNummer = CalcNotenNummer(VollNote,Halbkennung,Oktave);
                /// Fertige Note via emit senden
                emit ChangeNote(ID,NotenNummer);
            }
            else this->setText(input.left(2));
        }
        else this->setText(input.left(2));
        break;
    default:
        break;
    }

    if(text.length() == 1)
    {
        /// Prüfe auf Gültige Vollnote
    }
}

int NotenEdit::CalcNotenNummer(int vollnote,int halbkennung, int oktave)
{
    return VollNotenPos[vollnote] + ((halbkennung)? 1 : 0)  + 12*oktave;
}
