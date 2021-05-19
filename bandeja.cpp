#include "bandeja.h"
bandeja::bandeja()
{
    cargaIP();
    cargaIS();
}
bandeja::~bandeja()
{
    //dtor
}
void bandeja::menu()
{
    int opc;
    do
    {
        system("CLS");
        cout<<"\tSimulador de lector de correo electronico"<<endl<<endl;
        cout<<"Acciones a realizar:"<<endl;
        cout<<"1)Escribir correo"<<endl;
        cout<<"2)Leer correo"<<endl;
        cout<<"3)Modificar correo"<<endl;
        cout<<"4)Eliminar correo"<<endl;
        cout<<"5)Exportar csv"<<endl;
        cout<<"6)Importar csv"<<endl;
        cout<<"7)Modificar de csv"<<endl;
        cout<<"8)Eliminar de csv"<<endl;
        cout<<"9)Exportar formato propietario"<<endl;
        cout<<"10)Modificar en el archivo propietario"<<endl;
        cout<<"11)Eliminar en el archivo propietario"<<endl;
        cout<<"12)Menu de busquedas eficientes"<<endl;
        cout<<"13)Encriptado de informacion"<<endl;
        cout<<"14)Desencriptado de informacion"<<endl;
        cout<<"15)Crear comprimido"<<endl;
        cout<<"16)Descomprimir"<<endl;
        cout<<"17)Salir"<<endl;
        cout<<"Ingrese una opcion: ";
        cin>>opc;
        switch(opc)
        {
        case 1:
            crearCorreo();
            break;
        case 2:
            leerCorreo();
            break;
        case 3:
            modificar();
            break;
        case 4:
            eliminar();
            break;
        case 5:
            csv();
            break;
        case 6:
            csvImport();
            break;
        case 7:
            csvModificar();
            break;
        case 8:
            csvEliminar();
            break;
        case 9:
            propExp();
            break;
        case 10:
            propModificar();
            break;
        case 11:
            propEliminar();
            break;
        case 12:
            int opSrch;
            system("CLS");
            cout<<"\tBusquedas Eficientes"<<endl<<endl;
            cout<<"Tipos:"<<endl;
            cout<<"1)Busqueda en RAM"<<endl;
            cout<<"2)Busqueda en Indice Primario"<<endl;
            cout<<"3)Busqueda en Indice Secundario"<<endl;
            cout<<"4)Activar Indices Paginados"<<endl;
            cout<<"5)Cargar tabla hash"<<endl;
            cout<<"6)Busqueda en tabla hash"<<endl;
            cout<<"Ingrese una opcion: ";
            cin>>opSrch;
            switch(opSrch)
            {
            case 1:
                busquedaEficiente();
                break;
            case 2:
                int llave;
                cout<<"Ingrese ID del correo: ";
                cin>>llave;
                busquedaIndicePrim(llave);
                break;
            case 3:
                busquedaIndiceSec();
                break;
            case 4:
                cargarIPP();
                cout<<"Indices paginados cargados exitosamente"<<endl;
                system("Pause");
                break;
            case 5:
                if(bandHash==false)
                {
                    cargarTablaHash();
                    cout<<"Tabla Hash cargada exitosamente"<<endl;
                    bandHash=true;
                }
                else
                {
                    cout<<"La tabla ya esta cargada"<<endl;
                }
                system("Pause");
                break;
            case 6:
                if(bandHash==true)
                {
                    buscarTablaHash();
                }
                else
                {
                    cout<<"La tabla no esta cargada"<<endl;
                    system("Pause");
                }
                break;
            default:
                cout<<"Opcion invalida"<<endl;
                break;
            }
            break;
        case 13:
            cifrado();
            break;

        case 14:
            descifrado();
            break;
        case 15:
            compresion();
            break;
        case 16:
            descompresion();
            break;
        case 17:
            guardarIP();
            if(bandHash==true)
            {
                guardarTablaHash();
            }
            system("pause");
            cout<<"Adios!"<<endl;
            exit(0);
        default:
            cout<<"Opcion invalida"<<endl;
            break;
        }

    }
    while(opc!=17);
}

void bandeja::crearCorreo()
{
    rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        int id;
        do
        {
            id=0;
            cout<<"Ingrese Id: ";
            cin>>id;
        }
        while(valID(id)==false);
        c.id=id;
        rwFile.close();
        rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );

        char f_h[10];

        time_t date = time(nullptr);
        struct tm *tlocal= localtime(&date);

        strftime(f_h,128,"%d/%m/%y",tlocal);
        strcpy(c.fecha,f_h);

        strftime(f_h,128,"%H:%M:%S",tlocal);
        strcpy(c.hora,f_h);

        cin.ignore();
        cout<<"Ingrese Remitente: ";
        cin.getline(c.from,100);
        cout<<"Ingrese Destinatario: ";
        cin.getline(c.to,100);
        cout<<"Ingrese CC: ";
        cin.getline(c.cc,100);
        cout<<"Ingrese BCC: ";
        cin.getline(c.bcc,100);
        cout<<"Ingrese Asunto: ";
        cin.getline(c.asunto,100);
        cout<<"Ingrese Contenido(fin del mensaje '|'): "<<endl;
        cin.getline(c.contenido,300,'|');

        int pos = ((c.id - 1) * sizeof(struct Correos));
        rwFile.seekp(pos);
        rwFile.write((char*)&c, sizeof(struct Correos));

        ip.setLlavePrim(c.id);
        ip.setReferencia(pos);
        arbolIndice.insertData(ip);

        Hash h;
        int posTabla;
        strcpy(ipst.llave,to_string(c.id).c_str());
        ipst.referencia=pos;
        posTabla=h.hashing(ipst.llave);
        tablaHash[posTabla].push_back(ipst);
        contHash++;

        indicePFile.open( "indice_primario.txt",  ios::out | ios::binary );
        indicePFile.seekp(0);
        ip.setLlavePrim(1);
        indicePFile.write((char*)&ip, sizeof(indicePrim));
        indicePFile.close();

        indiceSec *aux, isR, isD;
        isR.setLlaveSec(c.from);
        TreeNode<indiceSec>*& aver = arbolIndiceSecRem.findData(isR);

        if(aver!=nullptr)
        {
            aux=arbolIndiceSecRem.retrieve(aver);
            aux->setListRef(c.id);
            arbolIndiceSecRem.insertData(*aux);
            arbolIndiceSecRem.deleteData(aver);
        }
        else
        {
            isR.setListRef(c.id);
            arbolIndiceSecRem.insertData(isR);
        }

        isD.setLlaveSec(c.to);
        TreeNode<indiceSec>*& aver2 = arbolIndiceSecDes.findData(isD);

        if(aver2!=nullptr)
        {
            aux=arbolIndiceSecDes.retrieve(aver2);
            aux->setListRef(c.id);
            arbolIndiceSecDes.insertData(*aux);
            arbolIndiceSecDes.deleteData(aver2);
        }
        else
        {
            isD.setListRef(c.id);
            arbolIndiceSecDes.insertData(isD);
        }


        rwFile.close();
    }
}

void bandeja::leerCorreo()
{

    rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        int opc;
        cout<<"\t1)Leer por ID"<<endl;
        cout<<"\t2)Leer por remitente"<<endl;
        cout<<"\tIngrese una opcion: ";
        cin>>opc;
        switch(opc)
        {
        case 1:
        {
            int id;

            cout<<"Ingrese Id: ";
            cin>>id;

            rwFile.close();
            rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );

            int pos=((id-1)*sizeof(struct Correos));
            rwFile.seekg(pos, ios::beg);
            rwFile.read((char*)&c, sizeof(struct Correos));

            cout<<"ID: "<<c.id<<endl;
            cout<<"Fecha: "<<c.fecha<<endl;
            cout<<"Hora: "<<c.hora<<endl;
            cout<<"Remitente: "<<c.from<<endl;
            cout<<"Destinatario: "<<c.to<<endl;
            cout<<"CC: "<<c.cc<<endl;
            cout<<"BCC: "<<c.bcc<<endl;
            cout<<"Asunto: "<<c.asunto<<endl;
            cout<<"Contenido: "<<endl<<c.contenido<<endl;


        }
        break;


        case 2:
        {
            char remit[100];

            if(bandSrch==false)
            {
                cout<<"BUSQUEDA SECUENCIAL"<<endl;
                cout<<"Ingrese remitente a buscar: ";
                cin.ignore();
                cin.getline(remit,100);
                int i=0;
                int tam=sizeof(struct Correos);
                int pos=0;
                while(!rwFile.eof())
                {
                    rwFile.seekg(pos, ios::beg);
                    rwFile.read((char*)&c, sizeof(struct Correos));

                    if(rwFile.eof())
                        break;

                    if(strcmp(c.from,remit)==0)
                    {
                        cout<<"Correo #"<<i+1<<endl;
                        cout<<"ID: "<<c.id<<endl;
                        cout<<"Fecha: "<<c.fecha<<endl;
                        cout<<"Hora: "<<c.hora<<endl;
                        cout<<"Remitente: "<<c.from<<endl;
                        cout<<"Destinatario: "<<c.to<<endl;
                        cout<<"CC: "<<c.cc<<endl;
                        cout<<"BCC: "<<c.bcc<<endl;
                        cout<<"Asunto: "<<c.asunto<<endl;
                        cout<<"Contenido: "<<endl<<c.contenido<<endl<<endl;
                        i++;
                    }
                    pos+=tam;
                }
                if(i==0)
                {
                    cout<<"No se encontro ningun correo"<<endl;
                }
            }

            else
            {
                cout<<"BUSQUEDA RAPIDA"<<endl;
                cout<<"Ingrese remitente a buscar: ";
                cin.ignore();
                cin.getline(remit,100);
                bool flag=false;
                int inf, sup, mitad;
                inf = 0;
                sup = sizeArray;
                while(inf <= sup)
                {
                    mitad = (inf + sup)/2;

                    if (strcmp(correosRam[mitad].from, remit) == 0)
                    {
                        flag = true;
                        break;
                    }
                    if (strcmp(correosRam[mitad].from, remit) > 0)
                    {
                        sup = mitad;
                        mitad = (inf + sup)/2;
                    }
                    if(strcmp(correosRam[mitad].from, remit) < 0)
                    {
                        inf = mitad;
                        mitad = (inf + sup)/2;
                    }
                }

                if (flag == true)
                {
                    cout << "Correo encontrado: "<< endl;
                    cout << correosRam[mitad].id << endl;
                    cout << correosRam[mitad].fecha << endl;
                    cout << correosRam[mitad].hora << endl;
                    cout << correosRam[mitad].from << endl;
                    cout << correosRam[mitad].to << endl;
                    cout << correosRam[mitad].cc << endl;
                    cout << correosRam[mitad].bcc << endl;
                    cout << correosRam[mitad].asunto << endl;
                    cout << correosRam[mitad].contenido << endl;
                }
                else
                {
                    cout<<"No se encontro ningun correo"<<endl;
                }
            }
        }
        break;

        default:
            cout<<"Opcion invalida"<<endl;
            break;
        }

        rwFile.close();
    }

    system("pause");

}

void bandeja::modificar()
{


    rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        int opc;
        cout<<"\t1)Modificar por ID"<<endl;
        cout<<"\t2)Modificar por remitente"<<endl;
        cout<<"\tIngrese una opcion: ";
        cin>>opc;
        switch(opc)
        {
        case 1:
        {
            int id;

            cout<<"Ingrese Id: ";
            cin>>id;

            if(valID(id)==false)
            {
                rwFile.close();
                rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
                cin.ignore();
                int pos=((id-1)*sizeof(struct Correos));
                rwFile.seekg(pos, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                cout<<"ID: "<<c.id<<endl;
                cout<<"Fecha: "<<c.fecha<<endl;
                cout<<"Hora: "<<c.hora<<endl;
                cout<<"Remitente: "<<c.from<<endl;
                cout<<"Destinatario: "<<c.to<<endl;
                cout<<"CC: "<<c.cc<<endl;
                cout<<"BCC: "<<c.bcc<<endl;
                cout<<"Asunto: "<<c.asunto<<endl;
                cout<<"Contenido: "<<endl<<c.contenido<<endl<<endl;

                cout<<"Ingrese Nuevo Remitente: ";
                cin.getline(c.from,100);
                cout<<"Ingrese Nuevo Destinatario: ";
                cin.getline(c.to,100);
                cout<<"Ingrese Nuevo CC: ";
                cin.getline(c.cc,100);
                cout<<"Ingrese Nuevo BCC: ";
                cin.getline(c.bcc,100);
                cout<<"Ingrese Nuevo Asunto: ";
                cin.getline(c.asunto,100);
                cout<<"Ingrese Nuevo Contenido: "<<endl;
                cin.getline(c.contenido,300,'|');

                rwFile.seekp(pos);
                rwFile.write((char*)&c, sizeof(struct Correos));
            }

            else
            {
                cout<<"Lo sentimos, el ID no fue encontrado"<<endl;
                system("PAUSE");
            }

        }
        break;

        case 2:
        {
            char remit[100];
            cout<<"Ingrese remitente a modificar: ";
            cin.ignore();
            cin.getline(remit,100);
            int i=0;
            int tam=sizeof(struct Correos);
            int pos=0;
            int posDef=0;
            while(!rwFile.eof())
            {
                rwFile.seekg(pos, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                if(rwFile.eof())
                    break;

                if(strcmp(c.from,remit)==0)
                {
                    cout<<"Correo #"<<i+1<<endl;
                    cout<<"ID: "<<c.id<<endl;
                    cout<<"Fecha: "<<c.fecha<<endl;
                    cout<<"Hora: "<<c.hora<<endl;
                    cout<<"Remitente: "<<c.from<<endl;
                    cout<<"Destinatario: "<<c.to<<endl;
                    cout<<"CC: "<<c.cc<<endl;
                    cout<<"BCC: "<<c.bcc<<endl;
                    cout<<"Asunto: "<<c.asunto<<endl;
                    cout<<"Contenido: "<<endl<<c.contenido<<endl<<endl;
                    i++;
                    posDef=pos;
                }
                pos+=tam;
            }
            rwFile.close();
            rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
            if(i==0)
            {
                cout<<"No se encontro ningun correo"<<endl;
            }
            else
            {
                if(i>1)
                {
                    int id;
                    cout<<"Se encontro mas de un correo"<<endl;
                    cout<<"Elija el id a modificar: ";
                    cin>>id;
                    cin.ignore();
                    posDef=((id-1)*sizeof(struct Correos));
                    cout<<"Correo seleccionado:"<<endl<<endl;
                    cout<<"ID: "<<c.id<<endl;
                    cout<<"Fecha: "<<c.fecha<<endl;
                    cout<<"Hora: "<<c.hora<<endl;
                    cout<<"Remitente: "<<c.from<<endl;
                    cout<<"Destinatario: "<<c.to<<endl;
                    cout<<"CC: "<<c.cc<<endl;
                    cout<<"BCC: "<<c.bcc<<endl;
                    cout<<"Asunto: "<<c.asunto<<endl;
                    cout<<"Contenido: "<<endl<<c.contenido<<endl<<endl;
                }

                rwFile.seekg(posDef, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                cout<<"Ingrese Nuevo Remitente: ";
                cin.getline(c.from,100);
                cout<<"Ingrese Nuevo Destinatario: ";
                cin.getline(c.to,100);
                cout<<"Ingrese Nuevo CC: ";
                cin.getline(c.cc,100);
                cout<<"Ingrese Nuevo BCC: ";
                cin.getline(c.bcc,100);
                cout<<"Ingrese Nuevo Asunto: ";
                cin.getline(c.asunto,100);
                cout<<"Ingrese Nuevo Contenido: "<<endl;
                cin.getline(c.contenido,300,'|');
                rwFile.seekp(posDef);
                rwFile.write((char*)&c, sizeof(struct Correos));
            }
        }
        break;

        default:
            cout<<"Opcion invalida"<<endl;
            break;
        }
        rwFile.close();
    }
}

void bandeja::eliminar()
{
    rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        int opc;
        cout<<"\t1)Eliminar por ID"<<endl;
        cout<<"\t2)Eliminar por remitente"<<endl;
        cout<<"\tIngrese una opcion: ";
        cin>>opc;
        switch(opc)
        {
        case 1:
        {
            int id;
            cout<<"Ingrese Id: ";
            cin>>id;

            rwFile.close();
            rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
            cin.ignore();
            int pos=((id-1)*sizeof(struct Correos));
            rwFile.seekg(pos, ios::beg);
            rwFile.read((char*)&c, sizeof(struct Correos));

            cout<<"ID: "<<c.id<<endl;
            cout<<"Fecha: "<<c.fecha<<endl;
            cout<<"Hora: "<<c.hora<<endl;
            cout<<"Remitente: "<<c.from<<endl;
            cout<<"Destinatario: "<<c.to<<endl;
            cout<<"CC: "<<c.cc<<endl;
            cout<<"BCC: "<<c.bcc<<endl;
            cout<<"Asunto: "<<c.asunto<<endl;
            cout<<"Contenido: "<<endl<<c.contenido<<endl<<endl;

            ip.setLlavePrim(c.id);
            TreeNode<indicePrim>*& aver = arbolIndice.findData(ip);
            arbolIndice.deleteData(aver);

            indicePFile.open( "indice_primario.txt",  ios::out | ios::binary );
            indicePFile.seekp(0);
            ip.setLlavePrim(1);
            indicePFile.write((char*)&ip, sizeof(indicePrim));
            indicePFile.close();

            Hash h;
            LSL<ipStr> *l;
            int posTabla;
            strcpy(ipst.llave,to_string(c.id).c_str());
            ipst.referencia=pos;
            posTabla=h.hashing(ipst.llave);
            *l=tablaHash[posTabla];
            if(l->size()>0)
            {
                for(int i=0; i<l->size(); i++)
                {
                    if(strcmp(ipst.llave,to_string(c.id).c_str())==0)
                    {
                        l->erase(i);
                        contHash--;
                    }
                }
            }

            c.id=NULL;
            strcpy(c.fecha," ");
            strcpy(c.hora," ");
            strcpy(c.from," ");
            strcpy(c.to," ");
            strcpy(c.cc," ");
            strcpy(c.bcc," ");
            strcpy(c.asunto," ");
            strcpy(c.contenido," ");

            rwFile.seekp(pos);
            rwFile.write((char*)&c, sizeof(struct Correos));
            cout<<"Correo eliminado"<<endl;

            system("PAUSE");
        }
        break;

        case 2:
        {
            char remit[100];
            cout<<"Ingrese remitente a eliminar: ";
            cin.ignore();
            cin.getline(remit,100);
            int i=0;
            int tam=sizeof(struct Correos);
            int pos=0;
            int posDef=0;
            while(!rwFile.eof())
            {
                rwFile.seekg(pos, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                if(rwFile.eof())
                    break;

                if(strcmp(c.from,remit)==0)
                {
                    cout<<"Correo #"<<i+1<<endl;
                    cout<<"ID: "<<c.id<<endl;
                    cout<<"Fecha: "<<c.fecha<<endl;
                    cout<<"Hora: "<<c.hora<<endl;
                    cout<<"Remitente: "<<c.from<<endl;
                    cout<<"Destinatario: "<<c.to<<endl;
                    cout<<"CC: "<<c.cc<<endl;
                    cout<<"BCC: "<<c.bcc<<endl;
                    cout<<"Asunto: "<<c.asunto<<endl;
                    cout<<"Contenido: "<<endl<<c.contenido<<endl<<endl;
                    i++;
                    posDef=pos;
                }
                pos+=tam;
            }
            rwFile.close();
            rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
            if(i==0)
            {
                cout<<"No se encontro ningun correo"<<endl;
            }
            else
            {
                if(i>1)
                {
                    int id;
                    cout<<"Se encontro mas de un correo"<<endl;
                    cout<<"Elija el id a elimiar: ";
                    cin>>id;
                    cin.ignore();
                    posDef=((id-1)*sizeof(struct Correos));
                    cout<<"Correo seleccionado:"<<endl<<endl;
                    cout<<"ID: "<<c.id<<endl;
                    cout<<"Fecha: "<<c.fecha<<endl;
                    cout<<"Hora: "<<c.hora<<endl;
                    cout<<"Remitente: "<<c.from<<endl;
                    cout<<"Destinatario: "<<c.to<<endl;
                    cout<<"CC: "<<c.cc<<endl;
                    cout<<"BCC: "<<c.bcc<<endl;
                    cout<<"Asunto: "<<c.asunto<<endl;
                    cout<<"Contenido: "<<endl<<c.contenido<<endl<<endl;
                }

                rwFile.seekg(posDef, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                ip.setLlavePrim(c.id);
                TreeNode<indicePrim>*& aver = arbolIndice.findData(ip);
                arbolIndice.deleteData(aver);

                indicePFile.open( "indice_primario.txt",  ios::out | ios::binary );
                indicePFile.seekp(0);
                ip.setLlavePrim(1);
                indicePFile.write((char*)&ip, sizeof(indicePrim));
                indicePFile.close();

                Hash h;
                LSL<ipStr> *l;
                int posTabla;
                strcpy(ipst.llave,to_string(c.id).c_str());
                ipst.referencia=posDef;
                posTabla=h.hashing(ipst.llave);
                *l=tablaHash[posTabla];
                if(l->size()>0)
                {
                    for(int i=0; i<l->size(); i++)
                    {
                        if(strcmp(ipst.llave,to_string(c.id).c_str())==0)
                        {
                            l->erase(i);
                            contHash--;
                        }
                    }
                }

                c.id=NULL;
                strcpy(c.fecha," ");
                strcpy(c.hora," ");
                strcpy(c.from," ");
                strcpy(c.to," ");
                strcpy(c.cc," ");
                strcpy(c.bcc," ");
                strcpy(c.asunto," ");
                strcpy(c.contenido," ");

                rwFile.seekp(posDef);
                rwFile.write((char*)&c, sizeof(struct Correos));
                cout<<"Correo eliminado"<<endl;
                system("PAUSE");
            }
        }
        break;

        default:
            cout<<"Opcion invalida"<<endl;
            break;
        }
    }
    rwFile.close();
}

void bandeja::csv()
{
    csvFile.open( "scopy.csv", ios::out);
    if(!csvFile.is_open())
    {
        cout << "Error al abrir el archivo..." << endl;
        csvFile.open("scopy.csv", ios::out);

        if(!csvFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            csvFile.close();
        }
    }
    else
    {
        rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
        if( !rwFile.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            rwFile.open("correos.txt", ios::out);

            if(!rwFile.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                rwFile.close();
            }
        }
        else
        {
            int tam=sizeof(struct Correos);
            int pos=0;
            while(!rwFile.eof())
            {
                rwFile.seekg(pos, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                if(rwFile.eof())
                    break;

                if(pos==0)
                    csvFile<<"ID,"
                           <<"FECHA,"
                           <<"HORA,"
                           <<"REMITENTE,"
                           <<"DESTINATARIO,"
                           <<"CC,"
                           <<"BCC,"
                           <<"ASUNTO,"
                           <<"CONTENIDO\n";

                if(c.id!=NULL)
                {
                    csvFile<<c.id<<",";
                    csvFile<<c.fecha<<",";
                    csvFile<<c.hora<<",";
                    csvFile<<c.from<<",";
                    csvFile<<c.to<<",";
                    csvFile<<c.cc<<",";
                    csvFile<<c.bcc<<",";
                    csvFile<<c.asunto<<",";
                    csvFile<<'"';
                    for(int i=0; i<strlen(c.contenido); i++)
                    {
                        if(c.contenido[i]=='"')
                            csvFile<<'"';
                        csvFile.put(c.contenido[i]);
                    }
                    csvFile<<'"'<<"\n";
                }
                pos+=tam;
            }
            rwFile.close();
        }
        csvFile.close();
    }

}

void bandeja::csvImport()
{
    csvFile.open( "scopy.csv", ios::in);
    if(!csvFile.is_open())
    {
        cout << "Error al abrir el archivo..." << endl;
        csvFile.open("scopy.csv", ios::in);

        if(!csvFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            csvFile.close();
        }
    }
    else
    {
        rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
        if( !rwFile.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            rwFile.open("correos.txt", ios::out);

            if(!rwFile.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                rwFile.close();
            }
        }
        else
        {
            int sn;
            cout<<"Se sobreescribiran todos los datos ya existentes en el archivo de correos"<<endl;
            cout<<"Seguro que desea hacer la importacion?"<<endl;
            cout<<"(1=SI, 2=NO): ";
            cin>>sn;
            if(sn==1)
            {
                string id="", fecha="", hora="", from="", to="", cc="", bcc="", asunto="", contenido;
                getline(csvFile, id, ',');
                getline(csvFile, fecha, ',');
                getline(csvFile, hora, ',');
                getline(csvFile, from, ',');
                getline(csvFile, to, ',');
                getline(csvFile, cc, ',');
                getline(csvFile, bcc, ',');
                getline(csvFile, asunto, ',');
                getline(csvFile, contenido, '\n');
                while(!csvFile.eof())
                {
                    if(csvFile.eof())
                        break;

                    getline(csvFile, id, ',');
                    getline(csvFile, fecha, ',');
                    getline(csvFile, hora, ',');
                    getline(csvFile, from, ',');
                    getline(csvFile, to, ',');
                    getline(csvFile, cc, ',');
                    getline(csvFile, bcc, ',');
                    getline(csvFile, asunto, ',');
                    int i=0,aux=0;
                    bool flag=false;
                    bool flag2=false;
                    char letra;
                    contenido="";
                    while(true)
                    {
                        csvFile.get(letra);
                        if(letra=='"'&&!flag2)
                        {
                            flag2=true;
                        }
                        else
                        {
                            if(!flag&&letra=='"')
                                flag=true;

                            else
                            {
                                if(flag2&&flag&&letra=='\n')
                                    break;

                                else
                                {
                                    flag=false;
                                    contenido+=letra;
                                }
                                i++;
                                if(csvFile.eof())
                                    break;
                            }

                        }

                    }


                    c.id=atoi(id.c_str());
                    if(c.id!=0)
                    {
                        strcpy(c.fecha,fecha.c_str());
                        strcpy(c.hora,hora.c_str());
                        strcpy(c.from,from.c_str());
                        strcpy(c.to,to.c_str());
                        strcpy(c.cc,cc.c_str());
                        strcpy(c.bcc,bcc.c_str());
                        strcpy(c.asunto,asunto.c_str());
                        strcpy(c.contenido,contenido.c_str());

                        rwFile.close();
                        rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
                        int pos = ((c.id-1) * sizeof(struct Correos));
                        rwFile.seekp(pos);
                        rwFile.write((char*)&c, sizeof(struct Correos));
                    }

                }
                cout<<"Importe realizado con exito"<<endl;
            }
            else if(sn==2)
            {
                cout<<"Importe suspendido"<<endl;
            }
            else
            {
                cout<<"Opcion invalida"<<endl;
                cout<<"Importe suspendido"<<endl;
            }

            system("Pause");
            rwFile.close();
        }
        csvFile.close();
    }
}

void bandeja::csvModificar()
{
    csvFile.open("scopy.csv", ios::in);

    if(!csvFile.is_open())
    {
        cout << "Error al abrir el archivo..." << endl;
        csvFile.open("scopy.csv", ios::in);
        if(!csvFile.good())
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            csvFile.close();
        }
    }
    else
    {
        csvTemp.open("scopyTemp.csv", ios::out);
        if(!csvTemp.is_open())
        {
            cout << "Error al abrir el archivo..." << endl;
            csvTemp.open("scopyTemp.csv", ios::out);

            if(!csvTemp.good())
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                csvTemp.close();
            }
        }
        else
        {
            string id, fecha, hora, from, to, cc, bcc, asunto, contenido;

            string idUser;

            cout<<"Ingrese el id del correo a modificar: ";
            cin.ignore();
            getline(cin,idUser);

            getline(csvFile, id, ',');
            getline(csvFile, fecha, ',');
            getline(csvFile, hora, ',');
            getline(csvFile, from, ',');
            getline(csvFile, to, ',');
            getline(csvFile, cc, ',');
            getline(csvFile, bcc, ',');
            getline(csvFile, asunto, ',');
            getline(csvFile, contenido, '\n');

            csvTemp<<id<<',';
            csvTemp<<fecha<<',';
            csvTemp<<hora<<',';
            csvTemp<<from<<',';
            csvTemp<<to<<',';
            csvTemp<<cc<<',';
            csvTemp<<bcc<<',';
            csvTemp<<asunto<<',';
            csvTemp<<contenido<<'\n';

            while(!csvFile.eof())
            {
                if(csvFile.eof())
                    break;

                getline(csvFile, id, ',');
                getline(csvFile, fecha, ',');
                getline(csvFile, hora, ',');
                getline(csvFile, from, ',');
                getline(csvFile, to, ',');
                getline(csvFile, cc, ',');
                getline(csvFile, bcc, ',');
                getline(csvFile, asunto, ',');
                int i=0,aux=0;
                bool flag=false;
                bool flag2=false;
                char letra;
                contenido="";
                while(true)
                {
                    csvFile.get(letra);
                    if(letra=='"'&&!flag2)
                    {
                        flag2=true;
                    }
                    else
                    {
                        if(!flag&&letra=='"')
                            flag=true;

                        else
                        {
                            if(flag2&&flag&&letra=='\n')
                                break;

                            else
                            {
                                flag=false;
                                contenido+=letra;
                            }
                            i++;
                            if(csvFile.eof())
                                break;
                        }

                    }

                }

                if(id==idUser)
                {
                    if(csvFile.eof())
                    {
                        break;
                    }
                    cout<<"Ingrese Nuevo Remitente: ";
                    getline(cin,from);
                    cout<<"Ingrese Nuevo Destinatario: ";
                    getline(cin,to);
                    cout<<"Ingrese Nuevo CC: ";
                    getline(cin,cc);
                    cout<<"Ingrese Nuevo BCC: ";
                    getline(cin,bcc);
                    cout<<"Ingrese Nuevo Asunto: ";
                    getline(cin,asunto);
                    cout<<"Ingrese Nuevo Contenido: "<<endl;
                    getline(cin,contenido, '|');
                }
                if(csvFile.eof())
                {
                    break;
                }
                csvTemp<<id<<',';
                csvTemp<<fecha<<',';
                csvTemp<<hora<<',';
                csvTemp<<from<<',';
                csvTemp<<to<<',';
                csvTemp<<cc<<',';
                csvTemp<<bcc<<',';
                csvTemp<<asunto<<',';
                csvTemp<<'"';
                for(int i=0; i<contenido.size(); i++)
                {
                    if(contenido[i]=='"')
                        csvTemp<<'"';
                    csvTemp.put(contenido[i]);
                }
                csvTemp<<'"'<<"\n";
            }
            system("Pause");
            csvTemp.close();
        }
        csvFile.close();
    }
    remove("scopy.csv");
    rename("scopyTemp.csv","scopy.csv");
}

void bandeja::csvEliminar()
{
    csvFile.open("scopy.csv", ios::in);

    if(!csvFile.is_open())
    {
        cout << "Error al abrir el archivo..." << endl;
        csvFile.open("scopy.csv", ios::in);
        if(!csvFile.good())
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            csvFile.close();
        }
    }
    else
    {
        csvTemp.open("scopyTemp.csv", ios::out);
        if(!csvTemp.is_open())
        {
            cout << "Error al abrir el archivo..." << endl;
            csvTemp.open("scopyTemp.csv", ios::out);

            if(!csvTemp.good())
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                csvTemp.close();
            }
        }
        else
        {

            string id, fecha, hora, from, to, cc, bcc, asunto, contenido;

            string idUser;

            cout<<"Ingrese el id del correo a eliminar: ";
            cin.ignore();
            getline(cin,idUser);

            getline(csvFile, id, ',');
            getline(csvFile, fecha, ',');
            getline(csvFile, hora, ',');
            getline(csvFile, from, ',');
            getline(csvFile, to, ',');
            getline(csvFile, cc, ',');
            getline(csvFile, bcc, ',');
            getline(csvFile, asunto, ',');
            getline(csvFile, contenido, '\n');

            csvTemp<<id<<',';
            csvTemp<<fecha<<',';
            csvTemp<<hora<<',';
            csvTemp<<from<<',';
            csvTemp<<to<<',';
            csvTemp<<cc<<',';
            csvTemp<<bcc<<',';
            csvTemp<<asunto<<',';
            csvTemp<<contenido<<'\n';

            while(!csvFile.eof())
            {
                if(csvFile.eof())
                    break;

                getline(csvFile, id, ',');
                getline(csvFile, fecha, ',');
                getline(csvFile, hora, ',');
                getline(csvFile, from, ',');
                getline(csvFile, to, ',');
                getline(csvFile, cc, ',');
                getline(csvFile, bcc, ',');
                getline(csvFile, asunto, ',');
                int i=0,aux=0;
                bool flag=false;
                bool flag2=false;
                char letra;
                contenido="";
                while(true)
                {
                    csvFile.get(letra);
                    if(letra=='"'&&!flag2)
                    {
                        flag2=true;
                    }
                    else
                    {
                        if(!flag&&letra=='"')
                            flag=true;

                        else
                        {
                            if(flag2&&flag&&letra=='\n')
                                break;

                            else
                            {
                                flag=false;
                                contenido+=letra;
                            }
                            i++;
                            if(csvFile.eof())
                                break;
                        }

                    }

                }

                if(id==idUser)
                {
                    cout<<"Correo Eliminado"<<endl;
                }
                else
                {
                    if(csvFile.eof())
                    {
                        break;
                    }
                    csvTemp<<id<<',';
                    csvTemp<<fecha<<',';
                    csvTemp<<hora<<',';
                    csvTemp<<from<<',';
                    csvTemp<<to<<',';
                    csvTemp<<cc<<',';
                    csvTemp<<bcc<<',';
                    csvTemp<<asunto<<',';
                    csvTemp<<'"';
                    for(int i=0; i<contenido.size(); i++)
                    {
                        if(contenido[i]=='"')
                            csvTemp<<'"';
                        csvTemp.put(contenido[i]);
                    }
                    csvTemp<<'"'<<"\n";
                }

            }
            system("Pause");
            csvTemp.close();
        }
        csvFile.close();
    }
    remove("scopy.csv");
    rename("scopyTemp.csv","scopy.csv");
}

void bandeja::propExp()
{
    propFile.open( "propietary.dat", ios::out | ios::binary);
    if(!propFile.is_open())
    {
        cout << "Error al abrir el archivo..." << endl;
        propFile.open("propietary.dat", ios::out);

        if(!propFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            propFile.close();
        }
    }
    else
    {
        rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
        if( !rwFile.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            rwFile.open("correos.txt", ios::out);

            if(!rwFile.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                rwFile.close();
            }
        }
        else
        {
            int tam=sizeof(struct Correos);
            int pos=0;
            int numBytes;
            while(!rwFile.eof())
            {
                rwFile.seekg(pos, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                if(rwFile.eof())
                    break;

                if(c.id!=NULL)
                {
                    numBytes=strlen((char*)&c.id);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.id;

                    numBytes=strlen(c.fecha);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.fecha;

                    numBytes=strlen(c.hora);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.hora;

                    numBytes=strlen(c.from);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.from;

                    numBytes=strlen(c.to);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.to;

                    numBytes=strlen(c.cc);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.cc;

                    numBytes=strlen(c.bcc);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.bcc;

                    numBytes=strlen(c.asunto);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.asunto;

                    numBytes=strlen(c.contenido);
                    propFile.write((char*)&numBytes, sizeof(int));
                    propFile<<c.contenido<<endl;
                }
                pos+=tam;
            }
            rwFile.close();
        }
        propFile.close();
    }
}

void bandeja::propModificar()
{
    propTemp.open( "propietaryTemp.dat", ios::out);
    if(!propTemp.is_open())
    {
        cout << "Error al abrir el archivo..." << endl;
        propTemp.open("propietaryTemp.dat", ios::out);

        if(!propTemp.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            propTemp.close();
        }
    }
    else
    {
        propFile.open( "propietary.dat", ios::in);
        if( !propFile.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            propFile.open("propietary.dat", ios::out);

            if(!propFile.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                propFile.close();
            }
        }
        else
        {
            char id[10], fecha[10], hora[10], from[100], to[100], cc[100], bcc[100], asunto[100], contenido[300];
            char idUser[10];
            int numBytes;

            cout<<"Ingrese el id del correo a modificar: ";
            cin.sync();
            cin >> idUser;

            while(!propFile.eof())
            {

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&id, numBytes);
                id[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&fecha, numBytes);
                fecha[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&hora, numBytes);
                hora[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&from, numBytes);
                from[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&to, numBytes);
                to[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&cc, numBytes);
                cc[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&bcc, numBytes);
                bcc[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&asunto, numBytes);
                asunto[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&contenido, numBytes+1);
                contenido[numBytes]='\0';

                if(strcmp(id, idUser) == 0)
                {
                    cout<<"Ingrese Nuevo Remitente: ";
                    cin >> from;
                    cout<<"Ingrese Nuevo Destinatario: ";
                    cin >> to;
                    cout<<"Ingrese Nuevo CC: ";
                    cin >> cc;
                    cout<<"Ingrese Nuevo BCC: ";
                    cin >> bcc;
                    cout<<"Ingrese Nuevo Asunto: ";
                    cin >> asunto;
                    cout<<"Ingrese Nuevo Contenido: "<<endl;
                    cin.getline(contenido,300,'|');
                }
                if(propFile.eof())
                    break;

                numBytes=strlen(id);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<id;

                numBytes=strlen(fecha);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<fecha;

                numBytes=strlen(hora);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<hora;

                numBytes=strlen(from);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<from;

                numBytes=strlen(to);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<to;

                numBytes=strlen(cc);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<cc;

                numBytes=strlen(bcc);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<bcc;

                numBytes=strlen(asunto);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<asunto;

                numBytes=strlen(contenido);
                propTemp.write((char*)&numBytes, sizeof(int));
                propTemp<<contenido;
                propTemp<<endl;

                if(propFile.eof())
                    break;
            }

            propFile.close();
            propTemp.close();
            remove("propietary.dat");
            rename("propietaryTemp.dat","propietary.dat");
        }

    }

}

void bandeja::propEliminar()
{
    propTemp.open( "propietaryTemp.dat", ios::out);
    if(!propTemp.is_open())
    {
        cout << "Error al abrir el archivo..." << endl;
        propTemp.open("propietaryTemp.dat", ios::out);

        if(!propTemp.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            propTemp.close();
        }
    }
    else
    {
        propFile.open( "propietary.dat", ios::in);
        if( !propFile.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            propFile.open("propietary.dat", ios::out);

            if(!propFile.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                propFile.close();
            }
        }
        else
        {
            char id[10], fecha[10], hora[10], from[100], to[100], cc[100], bcc[100], asunto[100], contenido[300];
            char idUser[10];
            int numBytes;

            cout<<"Ingrese el id del correo a eliminar: ";
            cin.sync();
            cin >> idUser;

            while(!propFile.eof())
            {

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&id, numBytes);
                id[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&fecha, numBytes);
                fecha[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&hora, numBytes);
                hora[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&from, numBytes);
                from[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&to, numBytes);
                to[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&cc, numBytes);
                cc[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&bcc, numBytes);
                bcc[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&asunto, numBytes);
                asunto[numBytes]='\0';

                propFile.read((char*)&numBytes, sizeof(int));
                propFile.read((char*)&contenido, numBytes+1);
                contenido[numBytes]='\0';

                if(strcmp(id, idUser) == 0)
                {
                    cout<<"Correo eliminado"<<endl;
                    system("pause");
                }
                else
                {
                    if(propFile.eof())
                        break;

                    numBytes=strlen(id);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<id;

                    numBytes=strlen(fecha);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<fecha;

                    numBytes=strlen(hora);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<hora;

                    numBytes=strlen(from);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<from;

                    numBytes=strlen(to);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<to;

                    numBytes=strlen(cc);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<cc;

                    numBytes=strlen(bcc);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<bcc;

                    numBytes=strlen(asunto);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<asunto;

                    numBytes=strlen(contenido);
                    propTemp.write((char*)&numBytes, sizeof(int));
                    propTemp<<contenido;
                    propTemp<<endl;
                }

                if(propFile.eof())
                    break;
            }

            propFile.close();
            propTemp.close();
            remove("propietary.dat");
            rename("propietaryTemp.dat","propietary.dat");
        }

    }
}

void bandeja::busquedaEficiente()
{
    rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        int tam=sizeof(struct Correos);
        int pos=0;
        while(!rwFile.eof())
        {
            rwFile.seekg(pos, ios::beg);
            rwFile.read((char*)&c, sizeof(struct Correos));

            if(rwFile.eof())
                break;

            if(c.id!=NULL)
            {
                correosRam[sizeArray]=c;
                sizeArray++;
            }
            pos+=tam;
        }
        rwFile.close();

        for (int gap = sizeArray/2; gap > 0; gap /= 2)
        {
            for (int i = gap; i < sizeArray; i += 1)
            {
                cTemp = correosRam[i];

                int j;
                for (j = i; j >= gap && (strcmp(correosRam[j - gap].from,cTemp.from)>0); j -= gap)
                    correosRam[j] = correosRam[j - gap];

                correosRam[j] = cTemp;
            }
        }
        bandSrch=true;
        cout<<"Busqueda eficiente habilitada"<<endl;
        system("pause");
    }
}

void bandeja::cargaIP()
{
    indicePFile.open( "indice_primario.txt", ios::in | ios::binary );
    if( !indicePFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        indicePFile.open("indice_primario.txt", ios::out);

        if(!indicePFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            indicePFile.close();
        }
    }
    else
    {
        int pos=0;
        int tam=sizeof(indicePrim);

        indicePFile.seekg(pos);
        indicePFile.read((char*)&ip,sizeof(indicePrim));
        pos+=tam;

        while(!indicePFile.eof())
        {
            indicePFile.seekg(pos);
            indicePFile.read((char*)&ip,sizeof(indicePrim));
            if(indicePFile.eof())
                break;
            arbolIndice.insertData(ip);
            pos+=tam;
        }

        indicePFile.close();
    }
}

void bandeja::busquedaIndicePrim(int llave)
{
    rwFile.open( "correos.txt", ios::in | ios::out | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        if(bandIPP==false)
        {
            indicePrim *aux;

            ip.setLlavePrim(llave);

            TreeNode<indicePrim>*& aver = arbolIndice.findData(ip);
            if (aver != nullptr)
            {
                aux=arbolIndice.retrieve(aver);
                ip.setReferencia(aux->getReferencia());
                rwFile.seekg(ip.getReferencia(), ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));
                cout<<"Encontrado:"<<endl;
                cout<<"ID: "<<c.id<<endl;
                cout<<"Fecha: "<<c.fecha<<endl;
                cout<<"Hora: "<<c.hora<<endl;
                cout<<"Remitente: "<<c.from<<endl;
                cout<<"Destinatario: "<<c.to<<endl;
                cout<<"CC: "<<c.cc<<endl;
                cout<<"BCC: "<<c.bcc<<endl;
                cout<<"Asunto: "<<c.asunto<<endl;
                cout<<"Contenido: "<<endl<<c.contenido<<endl;
            }
            else
            {
                cout << "No encontrado" << endl;
            }
        }

        else
        {
            indicePrimPag *aux;

            ipp.setLlavePrim(llave);

            TreeNode<indicePrimPag>*& aver = arbolIndicePag.findData(ipp);
            if (aver != nullptr)
            {
                aux=arbolIndicePag.retrieve(aver);
                ipp.setReferencia(aux->getReferencia());
                rwFile.seekg(ipp.getReferencia(), ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));
                cout<<"Encontrado:"<<endl;
                cout<<"ID: "<<c.id<<endl;
                cout<<"Fecha: "<<c.fecha<<endl;
                cout<<"Hora: "<<c.hora<<endl;
                cout<<"Remitente: "<<c.from<<endl;
                cout<<"Destinatario: "<<c.to<<endl;
                cout<<"CC: "<<c.cc<<endl;
                cout<<"BCC: "<<c.bcc<<endl;
                cout<<"Asunto: "<<c.asunto<<endl;
                cout<<"Contenido: "<<endl<<c.contenido<<endl;

                aux->setCont(aux->getCont()+1);
                cout<<"Contador: "<<aux->getCont()<<endl;
                cout<<"Exito de pagina"<<endl;
            }
            else
            {
                bool encontrado=false;

                indicePFile.open( "indice_primario.txt", ios::in | ios::binary );
                if( !indicePFile.is_open() )
                {
                    cout << "Error al abrir el archivo..." << endl;
                    indicePFile.open("indice_primario.txt", ios::out);

                    if(!indicePFile.good())
                    {
                        cout << "Error al crear el archivo... abortando" << endl;
                    }

                    else
                    {
                        cout<<"Archivo creado"<<endl;
                        indicePFile.close();
                    }
                }
                else
                {
                    int pos=0;
                    int tam=sizeof(indicePrim);

                    indicePFile.seekg(pos);
                    indicePFile.read((char*)&ip,sizeof(indicePrim));
                    pos+=tam;

                    while(!indicePFile.eof())
                    {
                        indicePFile.seekg(pos);
                        indicePFile.read((char*)&ip,sizeof(indicePrim));
                        if(indicePFile.eof())
                            break;
                        if(ip.getLlavePrim()==llave)
                        {
                            ipFound.setLlavePrim(ip.getLlavePrim());
                            ipFound.setReferencia(ip.getReferencia());
                            encontrado=true;
                            break;
                        }
                        pos+=tam;
                    }

                    indicePFile.close();
                }

                if(encontrado==true)
                {
                    string regs, llaveStr, refStr, contStr;
                    bool b1=false;
                    bool b2=false;
                    bool b3=false;
                    regs=arbolIndicePag.parseInOrder();

                    ippMin.setCont(1000);

                    for(int i=0; i<=regs.size(); i++)
                    {
                        if(b1==false)
                        {
                            if(regs[i]==',')
                                b1=true;

                            else
                                llaveStr+=regs[i];
                        }
                        else if(b2==false)
                        {
                            if(regs[i]==',')
                                b2=true;

                            else
                                refStr+=regs[i];
                        }
                        else if(b3==false)
                        {
                            if(regs[i]=='\n')
                                b3=true;

                            else
                                contStr+=regs[i];
                        }
                        else
                        {
                            ipp.setLlavePrim(atoi(llaveStr.c_str()));
                            ipp.setReferencia(atoi(refStr.c_str()));
                            ipp.setCont(atoi(contStr.c_str()));


                            if(ipp.getCont()<=ippMin.getCont());
                            {
                                ippMin.setLlavePrim(ipp.getLlavePrim());
                                ippMin.setReferencia(ipp.getReferencia());
                                ippMin.setCont(ipp.getCont());
                            }

                            llaveStr.clear();
                            refStr.clear();
                            contStr.clear();
                            b1=false;
                            b2=false;
                            b3=false;
                            i--;
                        }
                    }

                    TreeNode<indicePrimPag>*&aver = arbolIndicePag.findData(ippMin);
                    arbolIndicePag.deleteData(aver);
                    ipp.setLlavePrim(ipFound.getLlavePrim());
                    ipp.setReferencia(ipFound.getReferencia());
                    ipp.setCont(1);
                    arbolIndicePag.insertData(ipp);

                    rwFile.seekg(ipp.getReferencia(), ios::beg);
                    rwFile.read((char*)&c, sizeof(struct Correos));
                    cout<<"Encontrado:"<<endl;
                    cout<<"ID: "<<c.id<<endl;
                    cout<<"Fecha: "<<c.fecha<<endl;
                    cout<<"Hora: "<<c.hora<<endl;
                    cout<<"Remitente: "<<c.from<<endl;
                    cout<<"Destinatario: "<<c.to<<endl;
                    cout<<"CC: "<<c.cc<<endl;
                    cout<<"BCC: "<<c.bcc<<endl;
                    cout<<"Asunto: "<<c.asunto<<endl;
                    cout<<"Contenido: "<<endl<<c.contenido<<endl;

                    cout<<"Contador: "<<ipp.getCont()<<endl;
                    cout<<"Cambio de pagina"<<endl;
                }
                else
                {
                    cout << "No encontrado" << endl;
                }

            }
        }

        rwFile.close();
        system("pause");
    }
}

void bandeja::guardarIP()
{
    indicePFile.open( "indice_primario.txt", ios::in | ios::binary );
    if( !indicePFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        indicePFile.open("indice_primario.txt", ios::out);

        if(!indicePFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            indicePFile.close();
        }
    }
    else
    {

        int pos=0;
        int tam=sizeof(indicePrim);

        indicePFile.seekg(pos);
        indicePFile.read((char*)&ip,sizeof(indicePrim));
        pos+=tam;

        if(ip.getLlavePrim()!=0)
        {
            indicePFileTemp.open( "indice_primario_temp.txt", ios::out | ios::binary );
            if( !indicePFileTemp.is_open() )
            {
                cout << "Error al abrir el archivo..." << endl;
                indicePFileTemp.open("indice_primario_temp.txt", ios::out);

                if(!indicePFileTemp.good() )
                {
                    cout << "Error al crear el archivo... abortando" << endl;
                }

                else
                {
                    cout<<"Archivo creado"<<endl;
                    indicePFileTemp.close();
                }
            }
            else
            {
                string regs,llaveS,refS;
                int zero=0;
                regs=arbolIndice.parseInOrder();
                bool b1=false;
                bool b2=false;

                pos=0;

                ip.setLlavePrim(zero);
                ip.setReferencia(zero);

                indicePFileTemp.seekp(pos);
                indicePFileTemp.write((char*)&ip,sizeof(indicePrim));
                pos+=tam;

                for(int i=0; i<=regs.size(); i++)
                {
                    if(b1==false)
                    {
                        if(regs[i]==',')
                            b1=true;

                        else
                            llaveS+=regs[i];
                    }
                    else if(b2==false)
                    {
                        if(regs[i]=='\n')
                            b2=true;

                        else
                            refS+=regs[i];
                    }
                    else
                    {
                        ip.setLlavePrim(atoi(llaveS.c_str()));
                        ip.setReferencia(atoi(refS.c_str()));
                        indicePFileTemp.seekp(pos);
                        indicePFileTemp.write((char*)&ip,sizeof(indicePrim));
                        llaveS.clear();
                        refS.clear();
                        b1=false;
                        b2=false;
                        i--;
                        pos+=tam;
                    }
                }

                cout<<"Se modifico el archivo de indices"<<endl;

                indicePFileTemp.close();
                indicePFile.close();
                remove("indice_primario.txt");
                rename("indice_primario_temp.txt","indice_primario.txt");
            }
        }

        else
        {
            cout<<"No se modifico el archivo de indices"<<endl;
            indicePFile.close();
        }
    }
}

void bandeja::cargaIS()
{
    rwFile.open( "correos.txt", ios::in | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        indiceSec *aux;
        int tam=sizeof(struct Correos);
        int pos=0;
        while(!rwFile.eof())
        {
            indiceSec isR,isD;
            rwFile.seekg(pos, ios::beg);
            rwFile.read((char*)&c, sizeof(struct Correos));

            if(rwFile.eof())
                break;

            if(c.id!=NULL)
            {
                isR.setLlaveSec(c.from);
                TreeNode<indiceSec>*& aver = arbolIndiceSecRem.findData(isR);

                if(aver!=nullptr)
                {
                    aux=arbolIndiceSecRem.retrieve(aver);
                    aux->setListRef(c.id);
                }
                else
                {
                    isR.setListRef(c.id);
                    arbolIndiceSecRem.insertData(isR);
                }

                isD.setLlaveSec(c.to);
                TreeNode<indiceSec>*& aver2 = arbolIndiceSecDes.findData(isD);

                if(aver2!=nullptr)
                {
                    aux=arbolIndiceSecDes.retrieve(aver2);
                    aux->setListRef(c.id);
                }
                else
                {
                    isD.setListRef(c.id);
                    arbolIndiceSecDes.insertData(isD);
                }

            }
            pos+=tam;
        }
        rwFile.close();
    }
}

void bandeja::busquedaIndiceSec()
{
    indiceSec *aux;
    string remdes;
    cout<<"Ingrese remitente/destinatario de correos: ";
    cin.ignore();
    getline(cin,remdes);
    is.setLlaveSec(remdes);
    LSL <int> refISR,refISD;
    TreeNode<indiceSec>*& aver = arbolIndiceSecRem.findData(is);
    if(aver!=nullptr)
    {
        aux=arbolIndiceSecRem.retrieve(aver);
        refISR=aux->getListRef();
        for(int i=0; i<refISR.size(); i++)
        {
            busquedaIndicePrim(refISR[i]);
        }
    }
    else
    {
        cout<<"No encontrado en los remitentes"<<endl;
        system("pause");
    }

    TreeNode<indiceSec>*& aver2 = arbolIndiceSecDes.findData(is);
    if(aver2!=nullptr)
    {
        aux=arbolIndiceSecDes.retrieve(aver2);
        refISD=aux->getListRef();
        for(int i=0; i<refISD.size(); i++)
        {
            busquedaIndicePrim(refISD[i]);
        }
    }
    else
    {
        cout<<"No encontrado en los destinatarios"<<endl;
        system("pause");
    }
}

void bandeja::cargarIPP()
{
    indicePFile.open( "indice_primario.txt", ios::in | ios::binary );
    if( !indicePFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        indicePFile.open("indice_primario.txt", ios::out);

        if(!indicePFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            indicePFile.close();
        }
    }
    else
    {
        int pos=0;
        int tam=sizeof(indicePrim);
        int contPag=0;

        indicePFile.seekg(pos);
        indicePFile.read((char*)&ip,sizeof(indicePrim));
        pos+=tam;

        while(!indicePFile.eof()&&contPag<10)
        {
            indicePFile.seekg(pos);
            indicePFile.read((char*)&ip,sizeof(indicePrim));
            if(indicePFile.eof())
                break;

            ipp.setLlavePrim(ip.getLlavePrim());
            ipp.setReferencia(ip.getReferencia());
            ipp.setCont(0);
            arbolIndicePag.insertData(ipp);
            pos+=tam;
            contPag++;
        }

        indicePFile.close();
        bandIPP=true;
    }
}

void bandeja::cargarTablaHash()
{
    indicePFile.open( "indice_primario.txt", ios::in | ios::binary );
    if( !indicePFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        indicePFile.open("indice_primario.txt", ios::out);

        if(!indicePFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            indicePFile.close();
        }
    }
    else
    {
        Hash h;
        LSL<ipStr> l;
        int pos=0, posTabla;
        int tam=sizeof(indicePrim);

        indicePFile.seekg(pos);
        indicePFile.read((char*)&ip,sizeof(indicePrim));
        pos+=tam;

        while(!indicePFile.eof())
        {
            indicePFile.seekg(pos);
            indicePFile.read((char*)&ip,sizeof(indicePrim));
            if(indicePFile.eof())
                break;
            strcpy(ipst.llave,to_string(ip.getLlavePrim()).c_str());
            ipst.referencia=ip.getReferencia();
            posTabla=h.hashing(ipst.llave);
            tablaHash[posTabla].push_back(ipst);
            l=tablaHash[posTabla];
            cout<<posTabla<<" "<<ip.getLlavePrim()<<" "<<l[0].referencia<<endl;
            contHash++;
            pos+=tam;
        }
        indicePFile.close();
    }
}

void bandeja::buscarTablaHash()
{
    char claveUser[10];
    Hash h;
    LSL<ipStr> l;
    int posHash;
    cout<<"Ingrese la llave: ";
    cin.ignore();
    cin.getline(claveUser,10);
    posHash=h.hashing(claveUser);
    if(tablaHash[posHash].size()>0)
    {

        for(int i=0; i<tablaHash[posHash].size(); i++)
        {
            l=tablaHash[posHash];
            strcpy(ipst.llave,l[i].llave);
            ipst.referencia=l[i].referencia;
            if(strcmp(ipst.llave,claveUser)==0)
            {
                rwFile.open( "correos.txt", ios::in | ios::binary );
                if( !rwFile.is_open() )
                {
                    cout << "Error al abrir el archivo..." << endl;
                    rwFile.open("correos.txt", ios::out);

                    if(!rwFile.good() )
                    {
                        cout << "Error al crear el archivo... abortando" << endl;
                    }

                    else
                    {
                        cout<<"Archivo creado"<<endl;
                        rwFile.close();
                    }
                }
                else
                {

                    rwFile.seekg(ipst.referencia);
                    rwFile.read((char*)&c, sizeof(struct Correos));
                    cout<<"Encontrado:"<<endl;
                    cout<<"ID: "<<c.id<<endl;
                    cout<<"Fecha: "<<c.fecha<<endl;
                    cout<<"Hora: "<<c.hora<<endl;
                    cout<<"Remitente: "<<c.from<<endl;
                    cout<<"Destinatario: "<<c.to<<endl;
                    cout<<"CC: "<<c.cc<<endl;
                    cout<<"BCC: "<<c.bcc<<endl;
                    cout<<"Asunto: "<<c.asunto<<endl;
                    cout<<"Contenido: "<<endl<<c.contenido<<endl;
                    rwFile.close();
                }
            }
        }
    }
    else
    {
        cout<<"No se encontro el correo"<<endl;
    }
    system("PAUSE");
}

void bandeja::guardarTablaHash()
{
    rwFile.open( "correos.txt", ios::in | ios::binary );
    if( !rwFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        rwFile.open("correos.txt", ios::out);

        if(!rwFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            rwFile.close();
        }
    }
    else
    {
        tablaHashFile.open( "tabla_hash.txt", ios::out | ios::trunc );
        if( !tablaHashFile.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            tablaHashFile.open("tabla_hash.txt", ios::out);

            if(!tablaHashFile.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                tablaHashFile.close();
            }
        }
        else
        {
            int tam=sizeof(struct Correos);
            int pos=0, posHash;
            Hash h;
            LSL<ipStr> l;
            while(!rwFile.eof())
            {
                rwFile.seekg(pos, ios::beg);
                rwFile.read((char*)&c, sizeof(struct Correos));

                if(rwFile.eof())
                    break;

                if(c.id!=NULL)
                {
                    strcpy(ipst.llave,to_string(c.id).c_str());
                    ipst.referencia=pos;
                    posHash=h.hashing(ipst.llave);
                    l=tablaHash[posHash];
                    for(int i=0;i<l.size();i++)
                    {
                        if(strcmp(ipst.llave,to_string(c.id).c_str())==0)
                        {
                            tablaHashFile<<ipst.llave<<","<<ipst.referencia<<endl;
                        }
                    }
                }
                pos+=tam;
            }
            tablaHashFile.close();
        }
        rwFile.close();
    }
}

void bandeja::cifrado()
{
    csvFile.open( "scopy.csv", ios::in);
    if( !csvFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        csvFile.open("scopy.txt", ios::out);

        if(!csvFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            csvFile.close();
        }
    }
    else
    {
        encryptedFile.open( "encrypt.txt", ios::out | ios::trunc );
        if( !encryptedFile.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            encryptedFile.open("encrypt.txt", ios::out);

            if(!encryptedFile.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                encryptedFile.close();
            }
        }
        else
        {
            string password;
            char letra;
            cout<<"Ingrese la contraseña para encriptar el archivo: ";
            cin.ignore();
            getline(cin,password);

            string trash;
            getline(csvFile, trash, ',');
            getline(csvFile, trash, ',');
            getline(csvFile, trash, ',');
            getline(csvFile, trash, ',');
            getline(csvFile, trash, ',');
            getline(csvFile, trash, ',');
            getline(csvFile, trash, ',');
            getline(csvFile, trash, ',');
            getline(csvFile, trash, '\n');

            for(int i=0;i<password.size();i++)
            {
                letra=password[i];
                letra=letra-3;
                encryptedFile<<letra;
            }
            encryptedFile<<endl;

            while(!csvFile.eof())
            {
                csvFile.get(letra);
                letra=letra-3;
                encryptedFile.put(letra);
            }
            cout<<"Archivo encriptado correctamente"<<endl;
            encryptedFile.close();
        }
        csvFile.close();
    }
    system("PAUSE");
}

void bandeja::descifrado()
{
    bool goodpass=false;
    encryptedFile.open( "encrypt.txt", ios::in);
    if( !encryptedFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        encryptedFile.open("encrypt.txt", ios::out);

        if(!encryptedFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            encryptedFile.close();
        }
    }
    else
    {
        encryptedTemp.open( "temp.txt", ios::out);
        if( !encryptedTemp.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            encryptedTemp.open("temp.txt", ios::out);

            if(!encryptedTemp.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                encryptedTemp.close();
            }
        }
        else
        {
            string password,check;
            char letra;
            cout<<"Ingrese la contraseña para encriptar el archivo: ";
            cin.ignore();
            getline(cin,password);
            getline(encryptedFile,check,'\n');

            for(int i=0;i<check.size();i++)
            {
                letra=check[i];
                check[i]=letra+3;
            }

            if(password==check)
            {
                while(!encryptedFile.eof())
                {
                    encryptedFile.get(letra);
                    letra=letra+3;
                    encryptedTemp.put(letra);
                }
                goodpass=true;
                cout<<"Archivo desencriptado"<<endl;

            }
            else
            {
                cout<<"Contraseña incorrecta"<<endl;
            }
            encryptedTemp.close();
        }
        encryptedFile.close();
    }
    if(goodpass==true)
    {
        remove("encrypt.txt");
        rename("temp.txt","encrypt.txt");
    }
    else
    {
        remove("temp.txt");
    }
    system("PAUSE");
}

void bandeja::compresion()
{
    csvFile.open( "scopy.csv", ios::in);
    if( !csvFile.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        csvFile.open("scopy.txt", ios::out);

        if(!csvFile.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            csvFile.close();
        }
    }
    else
    {
        string cadenota;
        char letra;
        string trash;
        getline(csvFile, trash, ',');
        getline(csvFile, trash, ',');
        getline(csvFile, trash, ',');
        getline(csvFile, trash, ',');
        getline(csvFile, trash, ',');
        getline(csvFile, trash, ',');
        getline(csvFile, trash, ',');
        getline(csvFile, trash, ',');
        getline(csvFile, trash, '\n');
        while(!csvFile.eof())
        {
            csvFile.get(letra);
            if(csvFile.eof())
                break;
            cadenota+=letra;
        }
        csvFile.close();
        RLE(cadenota);
    }
    cout<<"Archivo Comprimido"<<endl;
    system("pause");
}

void bandeja::descompresion()
{
    comprimido.open("winrar.txt", ios::in);
    if( !comprimido.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        comprimido.open("winrar.txt", ios::out);

        if(!comprimido.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            comprimido.close();
        }
    }
    else
    {
        comprimidoTemp.open("temp.txt", ios::out);
        if( !comprimidoTemp.is_open() )
        {
            cout << "Error al abrir el archivo..." << endl;
            comprimido.open("temp.txt", ios::out);

            if(!comprimidoTemp.good() )
            {
                cout << "Error al crear el archivo... abortando" << endl;
            }

            else
            {
                cout<<"Archivo creado"<<endl;
                comprimidoTemp.close();
            }
        }
        else
        {
            while(!comprimido.eof())
            {
                int n;
                char letra;
                comprimido.read((char*)&n, sizeof(int));
                comprimido.get(letra);
                if(comprimido.eof())
                    break;
                for(int i=0;i<n;i++)
                {
                    comprimidoTemp<<letra;
                }
            }
            comprimidoTemp.close();
        }
        comprimido.close();
    }
    cout<<"Archivo Descomprimido"<<endl;
    system("pause");
    remove("winrar.txt");
    rename("temp.txt","winrar.txt");
}

void bandeja::RLE(string cadenota)
{
    comprimido.open("winrar.txt", ios::out|ios::trunc);
    if( !comprimido.is_open() )
    {
        cout << "Error al abrir el archivo..." << endl;
        comprimido.open("winrar.txt", ios::out);

        if(!comprimido.good() )
        {
            cout << "Error al crear el archivo... abortando" << endl;
        }

        else
        {
            cout<<"Archivo creado"<<endl;
            comprimido.close();
        }
    }
    else
    {
        int n = cadenota.length();
        for (int i=0;i<n;i++)
        {
            int cont=1;
            while(i<n-1&&cadenota[i]==cadenota[i+1])
            {
                cont++;
                i++;
            }
            comprimido.write((char*)&cont, sizeof(int));
            comprimido<<cadenota[i];
        }
        comprimido.close();
    }
}


bool bandeja::valID(int id)
{
    char cmp=NULL;
    int pos=((id-1)*sizeof(struct Correos));
    rwFile.seekg(pos);
    rwFile.put(cmp);
    if(cmp==NULL)
        return true;
    else
        return false;
}

