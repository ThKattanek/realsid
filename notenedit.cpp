#include "notenedit.h"

const QString VollNoten[7]={"C","D","E","F","G","H","A"};
const int VollNotenPos[7]={0,2,4,5,7,9,11};
const bool    Halbnoten[7]={true,true,false,true,true,true,false};
const QString Oktaven[7]={"0","1","2","3","4","5","6"};

NotenEdit::NotenEdit(int id, QWidget *parent) :
    QLineEdit(parent)
{
    ID = id;
    connect(this,SIGNAL(textEdited(QString)),this,SLOT(OnTextEdited(QString)));
    this->setMaxLength(3);
}

void NotenEdit::OnTextEdited(const QString &text)
{
    bool found = false;

    QString input = text.toUpper();

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
                /// Fertige Note via emit senden
                QMessageBox::information(this,"Info","Note gefunden :" + QVariant(CalcNotenNummer(VollNote,Halbkennung,Oktave)).toString());
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
                /// Fertige Note via emit senden
                QMessageBox::information(this,"Info","Note gefunden :" + QVariant(CalcNotenNummer(VollNote,Halbkennung,Oktave)).toString());
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
