#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define N 105
#define M 370
#define TRUE 1
#define FALSE 0

FILE *inici,*pre,*ope,*sobre,*reser,*conta,*repor,*sobreout;

int piso,d,m,a,bandi=0,cant_hab;
float presu; //Presupuesto del hotel
int num_habi[N];//Cantidad de habitaciones por piso

typedef struct
{
	char cad[30];
}cadena;
typedef struct
{
	int num_habitacion; 
	char hab_tipo[30];
	int ocupada_dia;//logicos
	int c_titular;
	int res_canceladas,res_efectivas,cont_adul,cont_nino;
	int uso_piso;
	float ingr_cama,ingr_caja;
}habitacion_res;
typedef struct
{
	char tipo[50];
	int disponible;//logicos
	float costo;
	int dias,limd,c_titular;
}habitacion;
typedef struct
{
	cadena nom[N][4];
	float preci[N][4];
	int habi,adi,comi; ////limites en j 0-habi,1-adi,2-comi
}precios;

precios p;
habitacion_res habi_reser[N][M];

void bandera_reser(int *band)
{
	if((*band)==0)
		(*band)=1;
	else
		fprintf(reser,"\n");
}

void bandera_repor(int *band)
{
	if((*band)==0)
		(*band)=1;
	else
		fprintf(repor,"\n");
}

int numero_habitacion(int i,int j)
{
	int num;
	num=((i+1)*1000)+j+1;
	return num;
}

int cont_dias(int di,int mi,int ai,int df,int mf,int af)
{
	int x,i;
	int cont=0,diasm,diasi,diasf;
	diasm=0;
	cont=0;
	for(i=ai;i>0;i--)
		if(((i%100==0)&&(i%400==0))||((i%4==0)&&(i%100!=0)))
			cont++;
	i=mi-1;
	while(i>=1)
	{
		switch(i)
		{
			case 1: x=31;
			break;
			case 2: x=28;
			break;
			case 3: x=31;
			break;
			case 4: x=30;
			break;
			case 5: x=31;
			break;
			case 6: x=30;
			break;
			case 7: x=31;
			break;
			case 8: x=31;
			break;
			case 9: x=30;
			break;
			case 10: x=31;
			break;
			case 11: x=30;
			break;
			case 12: x=21;
			break;
		}
		diasm=diasm+x;
		i=i-1;
	}
	diasi=(ai-1)*365+cont+diasm+di;
	diasm=0;
	cont=0;
	for(i=af;i>0;i--)
		if(((i%100==0)&&(i%400==0))||((i%4==0)&&(i%100!=0)))
			cont++;
	i=mf-1;
	while(i>=1)
	{
		switch(i)
		{
			case 1: x=31;
			break;
			case 2: x=28;
			break;
			case 3: x=31;
			break;
			case 4: x=30;
			break;
			case 5: x=31;
			break;
			case 6: x=30;
			break;
			case 7: x=31;
			break;
			case 8: x=31;
			break;
			case 9: x=30;
			break;
			case 10: x=31;
			break;
			case 11: x=30;
			break;
			case 12: x=21;
			break;
		}
		diasm=diasm+x;
		i=i-1;
	}
	diasf=(af-1)*365+cont+diasm+df;
	diasf=diasf-diasi+1;
	return diasf;
}

int fecha_a_dias(int diass,int mes,int an)
{
	int feb,x,i,diasm=0,diasss=0;
	if(((an%100==0)&&(an%400==0))||((an%4==0)&&(an%100!=0)))
		feb=29;
	else
		feb=28;
	i=mes-1;
	while(i>=1)
	{
		switch(i)
		{
			case 1: x=31;
			break;
			case 2: x=feb;
			break;
			case 3: x=31;
			break;
			case 4: x=30;
			break;
			case 5: x=31;
			break;
			case 6: x=30;
			break;
			case 7: x=31;
			break;
			case 8: x=31;
			break;
			case 9: x=30;
			break;
			case 10: x=31;
			break;
			case 11: x=30;
			break;
			case 12: x=21;
			break;
		}
		diasm=diasm+x;
		i--;
	}
	diasss=diasm+diass;
	if(feb==29)
		diasss++;
	return diasss;
}

int cuenta_digito(float n)
{
	float i;
	int cont=0;
	i=n;
	while(i>0.1)
	{
		i=i/10;
		cont++;
	}
return cont-1;
}

void imprimir_dia()
{
	int band=0;
	if((m<10)&&(d<10))
	{
		fprintf(conta,"0%d/0%d/%d |",d,m,a);
		band=1;
	}
	if((d<10)&&(band==0))
		fprintf(conta,"0%d/%d/%d |",d,m,a);
	if((m<10)&&(band==0))
		fprintf(conta,"%d/0%d/%d |",d,m,a);
	else
		if(band==0)
			fprintf(conta,"%d/%d/%d  |",d,m,a);
}

void imprimir_fecha_repor(int di,int mi,int ai)
{
	if(di<10)
	{
		if(mi<10)
			fprintf(repor,"0%d/0%d/%d",di,mi,ai);
		if(mi>9)
			fprintf(repor,"0%d/%d/%d",di,mi,ai);
	}
	if(di>9)
	{
		if(mi<10)
			fprintf(repor,"%d/0%d/%d",di,mi,ai);
		if(mi>9)
			fprintf(repor,"%d/%d/%d",di,mi,ai);
	}
}

void imprimir_fecha_reser(int di,int mi,int ai)
{
	if(di<10)
	{
		if(mi<10)
			fprintf(reser,"0%d/0%d/%d",di,mi,ai);
		if(mi>9)
			fprintf(reser,"0%d/%d/%d",di,mi,ai);
	}
	if(di>9)
	{
		if(mi<10)
			fprintf(reser,"%d/0%d/%d",di,mi,ai);
		if(mi>9)
			fprintf(reser,"%d/%d/%d",di,mi,ai);
	}
}

void imprimir_hora(int hora,int min)
{
	int band=0;
	if((hora<10)&&(min<10))
	{
		band=1;
		fprintf(reser,"  %0d:%0d)\n",hora,min);
	}
	if((hora<10)&&(band==0))
		fprintf(reser,"  %0d:%d)\n",hora,min);
	if((min<10)&&(band==0))
		fprintf(reser,"  %d:0%d)\n",hora,min);
	else
		fprintf(reser,"  %d:%d)\n",hora,min);
}

void imprimir_debe(float pago,int x)
{
	if(pago>0.00)
	{
		int hab,i;
		fprintf(conta,"\n");
		imprimir_dia();
		hab=cuenta_digito(pago);
		for(i=0;i<12-hab;i++)
		{
			fprintf(conta," ");
		}
		fprintf(conta,"%.2f|",pago);
		fprintf(conta,"               |");
		presu+=pago;
		hab=cuenta_digito(presu);
		for(i=0;i<12-hab;i++)
		{
			fprintf(conta," ");
		}
		switch(x)
		{
			case 1: {fprintf(conta,"%.2f|Restaurant",presu);} break;
			case 2: {fprintf(conta,"%.2f|Check-Out",presu);} break;
			case 3: {fprintf(conta,"%.2f|Servicios",presu);} break;
		}
	}
	
}

void cargar_hab_res()//inicializar numeros de habitaciones en [][0]
{
	int i,j,k=0;
	for(i=0;i<piso;i++)
	{
		for(j=0;j<num_habi[i];j++)
		{
			habi_reser[k][0].num_habitacion=numero_habitacion(i,j);
			k++;
		}
	}
			
}

void cargar_matriz_reser(int x,int y,int di,int mi,int ai,int df,int mf,int af,int titular,char tipo_hab[30])//cargar la reservacion en matriz
{
	int i=0,j,band=0,num,diai,diaf;
	num=numero_habitacion(x,y);
	while((i<N)&&(band==0))
	{
		if(num==habi_reser[i][0].num_habitacion)
				band=1;
		i++;
	}
	i--;
	diai=fecha_a_dias(di,mi,ai);
	diaf=fecha_a_dias(df,mf,af);
	for(j=diai;j<=diaf;j++)
	{
		habi_reser[i][j].ocupada_dia=TRUE;
		habi_reser[i][j].c_titular=titular;
		strcpy(habi_reser[i][j].hab_tipo,tipo_hab);
	}


}

void finalizar_dia(habitacion matriz[N][N]) //actualizar a tiempo real :3
{
	int hab,i,band=0,j;
	d++;
	switch(m)
	{
		case 1: if(d>31) { d=1; m++; band=1;} break;
		case 3: if(d>31) {d=1; m++; band=1;} break;
		case 4: if(d>30) {d=1; m++; band=1;} break;
		case 5: if(d>31) {d=1; m++; band=1;} break;
		case 6: if(d>30) {d=1; m++; band=1;} break;
		case 7: if(d>31) {d=1; m++; band=1;} break;
		case 8: if(d>31) {d=1; m++; band=1;} break;
		case 9: if(d>30) {d=1; m++; band=1;} break;
		case 10: if(d>31) {d=1; m++; band=1;} break;
		case 11: if(d>30) {d=1; m++; band=1;} break;
		case 12: if(d>31) {d=1; m=1; a++; band=1;} break;
	}
	if(m==2)
	{
		if(((a%100==0)&&(a%400==0))||((a%4==0)&&(a%100!=0)))
		{
			if(d>29)
			{
				d=1;
				m++;
			}
		}
		else
		{
			if(d>28)
			{
				d=1;
				m++;
			}
			
		}

	}
	if(d==15)
	{
		presu-=25000.00;
		if(m<10)
			fprintf(conta,"\n%d/0%d/%d |               |       25000.00|",d,m,a);
		else
			fprintf(conta,"\n%d/%d/%d |               |       25000.00|",d,m,a);
		hab=cuenta_digito(presu);
		for(i=0;i<12-hab;i++)
		{
			fprintf(conta," ");
		}
		fprintf(conta,"%.2f|Nomina",presu);
	}
	if(band==1)
	{
		presu-=25000.00;
		imprimir_dia();
		fprintf(conta,"               |       25000.00|");
		hab=cuenta_digito(presu);
		for(i=0;i<12-hab;i++)
		{
			fprintf(conta," ");
		}
		fprintf(conta,"%.2f|Nomina",presu);
	}
	for(i=0;i<piso;i++)
	{
		for(j=0;j<num_habi[i];j++)
		{
			if(matriz[i][j].disponible==FALSE)
			{
				matriz[i][j].dias+=1;
			}
		}
	}
}

int es_disponible(int x,int y,int di,int mi,int ai,int df,int mf,int af)
{
	int i=0,j,diasi,diasf,band=0,num;
	diasi=fecha_a_dias(di,mi,ai);
	diasf=fecha_a_dias(df,mf,af);
	num=numero_habitacion(x,y);
	while((i<N)&&(band==0))
	{
		if(habi_reser[i][0].num_habitacion==num)
			band=1;
		i++;
	}
	i--;
	for(j=diasi;j<diasf;j++)
	{
		if(habi_reser[i][j].ocupada_dia==1)
			return FALSE;
	}
	return TRUE;
	
}

int es_titular(int x,int y,int titular,int *aux,char auxd[30])
{
	int band=0,dia,i=0,num,k=0;
	dia=fecha_a_dias(d,m,a);
	num=numero_habitacion(x,y);
	while((i<N)&&(band==0))
	{
		if(num==habi_reser[i][0].num_habitacion)
			band=1;
		i++;
	}
	i--;
	band=0;
	k=dia;
	if(habi_reser[i][dia].c_titular==titular)
	{
		while(band==0)
		{
			if(habi_reser[i][k].c_titular!=titular)
				band=1;
			k++;
		}
		k--;
		strcpy(auxd,habi_reser[i][dia].hab_tipo);
		*aux=(k-dia)-1;
		return TRUE;
	}
	return FALSE;
}

void reservar(habitacion matriz[][N])
{
	int di,mi,ai,df,mf,af,diass,pos;
	char tipo_hab[30],adul_nin[3],nombre[50],apellido[50];
	int cedula,titular;
	int j=0,i=0,k=0,band=0,num_pers;
	fscanf(ope,"%d %d %d %d %d %d",&di,&mi,&ai,&df,&mf,&af);
	getc(ope);
	fscanf(ope,"%s",tipo_hab);
	fscanf(ope,"%d",&num_pers);
	getc(ope);
	while((i<piso)&&(band==0))
	{
		j=0;
		while((j<num_habi[i])&&(band==0))
		{
			if(strcmp(matriz[i][j].tipo,tipo_hab)==FALSE)
			{
				if(es_disponible(i,j,di,mi,ai,df,mf,af)==TRUE)
				{	
					for(k=0;k<num_pers;k++)
					{
						fscanf(ope,"%s",adul_nin);
						if(adul_nin[0]=='A')
						{
							if(k==0)
							{
								//fscanf(ope,"%d ",&matriz[i][j].c_titular);
								//titular=matriz[i][j].c_titular;
								fscanf(ope,"%d ",&titular);
							}
							else
								fscanf(ope,"%d ",&cedula);
							fscanf(ope,"%s %s",nombre,apellido);
							
							pos=fecha_a_dias(d,m,a);//cuantos adultos fueron atendidos en el dia
							habi_reser[0][pos].cont_adul++;
						}
						else
						{
							fscanf(ope,"%s %s",nombre,apellido);
							pos=fecha_a_dias(d,m,a);//cuantos niños fueron atendidos en el dia
							habi_reser[0][pos].cont_nino++;
						}
					}
					band=1;
					cargar_matriz_reser(i,j,di,mi,ai,df,mf,af,titular,tipo_hab);
					diass=cont_dias(di,mi,ai,df,mf,af);
					//matriz[i][j].limd=0;
					//matriz[i][j].dias=0;
					bandera_reser(&bandi);
					fprintf(reser,"RESERVACIÓN--(");
					imprimir_fecha_reser(d,m,a);
					fprintf(reser,")\n");
					fprintf(reser,"\tTitular: %d\n",titular);
					fprintf(reser,"\tHabitación %s del ",tipo_hab);
					imprimir_fecha_reser(di,mi,ai);
					fprintf(reser," al ");
					imprimir_fecha_reser(df,mf,af);
					fprintf(reser," (%d días)",diass);
					
					pos=fecha_a_dias(d,m,a);//llevar la cuenta de cuantas efectivas se hicieron en el dia
					habi_reser[0][pos].res_efectivas++;
				}
			}
			j++;
		}
		i++;
	}

	if(band==0)
	{
		//reservacion flexible
	}

}

void check_out(habitacion matriz[N][N],int xx,int h,int mm)
{
	int ced,i=0,j,hora,min,band=0,k;
	float pago;
	if(xx==0)
	{
		fscanf(ope,"%d",&ced);
		fscanf(ope,"%d",&hora);
		getc(ope);
		fscanf(ope,"%d",&min);
	}
	else
	{
		ced=xx;
		hora=h;
		min=mm;
	}
	while((i<piso)&&(band==0))
	{
		j=0;
		while((j<num_habi[i])&&(band==0))
		{
			if((matriz[i][j].c_titular==ced)&&(matriz[i][j].disponible==FALSE))
				band=1;
			j++;
		}
		i++;
	}
	
	if(band==1)
	{
		i--;
		j--;
		if(hora>12)
			matriz[i][j].dias=matriz[i][j].dias+1;
		if(matriz[i][j].dias<matriz[i][j].limd)
			pago=matriz[i][j].limd;
		else
			pago=matriz[i][j].dias;
		for(k=0;k<p.habi;k++)
		{
			if(strcmp(matriz[i][j].tipo,p.nom[k][0].cad)==0)
			{
				pago=pago*p.preci[k][0];
			}
		}
		imprimir_debe(pago,2);
		pago=pago+matriz[i][j].costo;
		matriz[i][j].disponible=TRUE;
		bandera_reser(&bandi);
		fprintf(reser,"CHECK-OUT--(");
		imprimir_fecha_reser(d,m,a);
		imprimir_hora(hora,min);
		fprintf(reser,"\tTitular: %d\n",ced);
		fprintf(reser,"\tCanceló: %.2f Bs.F.",pago);
	}

}

void check_out_forzado(habitacion matriz[N][N],int h,int mm,int posicion[3],char auxd[30])
{
	int band=0,i=0,j=0,ced,dia_max=1000,dia_actual,nume;
	while((i<piso)&&(band==0))
	{
		j=0;
		while((j<num_habi[i])&&(band==0))
		{
			if((matriz[i][j].dias==matriz[i][j].limd)&&(strcmp(matriz[i][j].tipo,auxd)==0))
			{
				posicion[0]=i;
				posicion[1]=j;
				band=1;
				ced=matriz[i][j].c_titular;
			}
			if((matriz[i][j].dias>matriz[i][j].limd)&&(band==0)&&(strcmp(matriz[i][j].tipo,auxd)==0))
			{
				if(matriz[i][j].dias<dia_max)
				{
					dia_max=matriz[i][j].dias;
					posicion[0]=i;
					posicion[1]=j;
					ced=matriz[i][j].c_titular;
				}
					
			}
			j++;
		}
		i++;
	}
	i=posicion[0];
	j=posicion[1];
	nume=numero_habitacion(i,j);
	dia_actual=fecha_a_dias(d,m,a);
	dia_actual=dia_actual-(matriz[i][j].dias-matriz[i][j].limd);
	band=i=0;
	while((i<N)&&(band==0))
	{
		if(nume==habi_reser[i][0].num_habitacion)
			band=1;
		i++;
	}
	i--;
	//ced=habi_reser[i][dia_actual].c_titular;
	//printf("%d",ced);
	check_out(matriz,ced,h,mm);
}

void check_in(habitacion matriz[N][N])
{
	int ced,i=0,j,hora,min,band=0,ij[3],aux;
	char auxd[30];
	fscanf(ope,"%d",&ced);
	fscanf(ope,"%d",&hora);
	getc(ope);
	fscanf(ope,"%d",&min);
	i=0;
	while((i<piso)&&(band==0))
	{
		j=0;
		while((j<num_habi[i])&&(band==0))
		{
			if((es_titular(i,j,ced,&aux,auxd)==TRUE)&&(matriz[i][j].disponible==TRUE))
			{
				matriz[i][j].disponible=FALSE;
				band=1;
				matriz[i][j].limd=aux;
				matriz[i][j].dias=0;
				matriz[i][j].c_titular=ced;
				bandera_reser(&bandi);
				fprintf(reser,"CHECK-IN--(");
				imprimir_fecha_reser(d,m,a);
				imprimir_hora(hora,min);
				fprintf(reser,"\tTitular: %d\n",ced);
				if(i==100)
					fprintf(reser,"\tHabitación: %d",((i+1)*1000)+j+1);
				if((i>9)&&(i<100))
					fprintf(reser,"\tHabitación: 0%d",((i+1)*1000)+j+1);
				if(i<10)
					fprintf(reser,"\tHabitación: 00%d",((i+1)*1000)+j+1);
			}
			j++;
		}
		i++;
	}
	if(band==0)
	{
		check_out_forzado(matriz,hora,min,ij,auxd);
		//matriz[ij[0]][ij[1]].disponible=FALSE;
		bandera_reser(&bandi);
		fprintf(reser,"CHECK-IN--(");
		imprimir_fecha_reser(d,m,a);
		imprimir_hora(hora,min);
		fprintf(reser,"\tTitular: %d\n",ced);
		if(i==100)
			fprintf(reser,"\tHabitación: %d",((i+1)*1000)+j+1);
		if((i>9)&&(i<100))
			fprintf(reser,"\tHabitación: 0%d",((i+1)*1000)+j+1);
		if(i<10)
			fprintf(reser,"\tHabitación: 00%d",((i+1)*1000)+j+1);
	}
}

void cancelar_reservacion(habitacion matriz[N][N])
{
	int ced,i=0,j,hora,min,band=0,pos;
	fscanf(ope,"%d",&ced);
	fscanf(ope,"%d",&hora);
	getc(ope);
	fscanf(ope,"%d",&min);
	while((i<piso)&&(band==0))
	{
		j=0;
		while((j<num_habi[i]&&(band==0)))
		{
			if(matriz[i][j].c_titular==ced)
				band=1;
			j++;
		}
		i++;
	}
	if(band==1)
	{
		i--;
		j--;
		matriz[i][j].disponible=TRUE;
		bandera_reser(&bandi);
		fprintf(reser,"CANCELACIÓN--(");
		imprimir_fecha_reser(d,m,a);
		imprimir_hora(hora,min);
		fprintf(reser,"\tTitular: %d",ced);
		pos=fecha_a_dias(d,m,a);//las reservaciones canceladas del dia en i=0 para no llevarlo en toda la matriz
		habi_reser[0][pos].res_canceladas++;
	}
	

}

float costos_adicional(char adici[30])
{
	int i;
	for(i=0;i<p.adi;i++)
	{
		if(strcmp(p.nom[i][1].cad,adici)==0)
			return p.preci[i][1];
	}
return 0;
}

void solicitar_servicios(habitacion matriz[N][N]) //revisar conteo con ivo xD
{
	int k,i=0,j=0,n,ced,band=0,pos;
	float pago,mi;
	char adici[30];
	fscanf(ope,"%d",&ced);
	while((i<piso)&&(band==0))
	{
		while((j<num_habi[i])&&(band==0))
		{
			if(matriz[i][j].c_titular==ced)
			{
				band=1;
				fscanf(ope,"%d",&n);
				for(k=0;k<n;k++)
				{
					fscanf(ope,"%s",adici);
					fscanf(ope,"%f",&mi);
					pago=(costos_adicional(adici)*mi)+pago;
					
					if(strcmp(adici,"CAM_A")==FALSE)
					{
						pos=fecha_a_dias(d,m,a);//ingresos por camas en el dia
						habi_reser[0][pos].ingr_cama+=(costos_adicional(adici)*mi);//revisar D:
					}
					else
					{
						pos=fecha_a_dias(d,m,a);//ingresos por camas en el dia
						habi_reser[0][pos].ingr_caja+=(costos_adicional(adici)*mi);//revisar D:
					}
				}
			}
			j++;
		}
		i++;
	}
	i--;
	j--;
	band=0;
	matriz[i][j].costo=pago;
	imprimir_debe(pago,3);
	
}

void restaurant()
{
	char comida[50];
	int cuantos,band=0,i,j;
	float total=0;
	fscanf(ope,"%d",&cuantos);
	for(i=0;i<cuantos;i++)
	{
		fscanf(ope,"%s",comida);
		j=band=0;
		while((j<p.comi)&&(band==0))
		{
			if(strcmp(comida,p.nom[j][2].cad)==0)
			{
				total+=p.preci[j][2];
				band=1;
			}
			j++;
		}
		
	}
	presu+=total;
	imprimir_debe(total,1);
}

void reporte(habitacion matriz[N][N])
{
	char opc[3];
	int di,mi,ai,df,mf,af,lim,i,k,pos1,pos2,j;
	int canceladas=0,efectivas=0,ninos=0,adultos=0,band=0,disponibles=0,ocupadas=0,reservadas=0;
	float camas=0,cajas=0;
	fscanf(ope,"%d",&lim);
	for(k=0;k<lim;k++)
	{
		fscanf(ope,"%s",opc);

		switch(opc[0])
		{
			case 'a': //Numero de reservaciones canceladas
					fscanf(ope,"%d %d %d %d %d %d ",&di,&mi,&ai,&df,&mf,&af);
					
					pos1=fecha_a_dias(di,mi,ai);
					pos2=fecha_a_dias(df,mf,af);
					
					for(i=pos1;i<=pos2;i++)
						canceladas+=habi_reser[0][i].res_canceladas;
					
					bandera_repor(&band);
					fprintf(repor,"Del ");
					imprimir_fecha_repor(di,mi,ai);
					fprintf(repor," al ");
					imprimir_fecha_repor(df,mf,af);
					
					if(canceladas<10)
						fprintf(repor," 0%d           Reservaciones canceladas",canceladas);
					else
						fprintf(repor," %d           Reservaciones canceladas",canceladas);
					
			break;
			
			case 'b': //Numero de reservaciones efectivas
					fscanf(ope,"%d %d %d %d %d %d ",&di,&mi,&ai,&df,&mf,&af);
					pos1=fecha_a_dias(di,mi,ai);
					pos2=fecha_a_dias(df,mf,af);
					for(i=pos1;i<=pos2;i++)
						efectivas+=habi_reser[0][i].res_efectivas;
					bandera_repor(&band);
					fprintf(repor,"Del ");
					imprimir_fecha_repor(di,mi,ai);
					fprintf(repor," al ");
					imprimir_fecha_repor(df,mf,af);
					if(efectivas<10)
						fprintf(repor," 0%d           Reservaciones efectivas",efectivas);
					else
						fprintf(repor," %d           Reservaciones efectivas",efectivas);
			break;
			
			case 'c': //Adultos y niños atendidos
					fscanf(ope,"%d %d %d %d %d %d ",&di,&mi,&ai,&df,&mf,&af);

					pos1=fecha_a_dias(di,mi,ai);
					pos2=fecha_a_dias(df,mf,af);
					for(i=pos1;i<=pos2;i++)
					{
						adultos+=habi_reser[0][i].cont_adul;
						ninos+=habi_reser[0][i].cont_nino;
					}
					bandera_repor(&band);
					fprintf(repor,"Del ");
					imprimir_fecha_repor(di,mi,ai);
					fprintf(repor," al ");
					imprimir_fecha_repor(df,mf,af);
					if(adultos<10)
						fprintf(repor," 0%d           Adultos atendidos\n",adultos);
					else
						fprintf(repor," %d           Adultos atendidos\n",adultos);
					fprintf(repor,"Del ");
					imprimir_fecha_repor(di,mi,ai);
					fprintf(repor," al ");
					imprimir_fecha_repor(df,mf,af);
					if(ninos<10)
						fprintf(repor," 0%d           Niños atendidos",ninos);
					else
						fprintf(repor," %d           Niños atendidos",ninos);
			break;
			
			case 'd': //Ingresos por concepto de camas adicionales
					fscanf(ope,"%d %d %d %d %d %d ",&di,&mi,&ai,&df,&mf,&af);
					pos1=fecha_a_dias(di,mi,ai);
					pos2=fecha_a_dias(df,mf,af);
					for(i=pos1;i<=pos2;i++)
						camas+=habi_reser[0][i].ingr_cama;
					bandera_repor(&band);
					fprintf(repor,"Del ");
					imprimir_fecha_repor(di,mi,ai);
					fprintf(repor," al ");
					imprimir_fecha_repor(df,mf,af);
					if(camas<10)
						fprintf(repor," 0%.2f           Ingresos por camas adicionales",camas);
					else
						fprintf(repor," %.2f           Ingresos por camas adicionales",camas);
					
			break;
			
			case 'e': //Piso con mayor uso
			break;
			
			case 'f': //Numero de transferencias totales
			break;
			
			case 'g':  //Ingresos por uso de caja fuerte
					fscanf(ope,"%d %d %d %d %d %d ",&di,&mi,&ai,&df,&mf,&af);
					pos1=fecha_a_dias(di,mi,ai);
					pos2=fecha_a_dias(df,mf,af);
					for(i=pos1;i<=pos2;i++)
						cajas+=habi_reser[0][i].ingr_caja;
					bandera_repor(&band);
					fprintf(repor,"Del ");
					imprimir_fecha_repor(di,mi,ai);
					fprintf(repor," al ");
					imprimir_fecha_repor(df,mf,af);
					if(cajas<10)
						fprintf(repor," 0%.2f           Ingresos por uso de caja fuerte",cajas);
					else
						fprintf(repor," %.2f           Ingresos por uso de caja fuerte",cajas);
			break;
			
			case 'h': //Porcentaje de ocupacion diaria

			break;
			
			case 'i': //Promedio del porcentaje de ocupacion diaria

			break;
						
			case 'j': //Habitaciones ocupadas
					//fscanf(ope,"%d %d %d ",&di,&mi,&ai);

					for(i=0;i<piso;i++)
					{
						for(j=0;j<num_habi[i];j++)
						{
							matriz[i][j].disponible=FALSE;
							ocupadas++;
						}
					}
					
					bandera_repor(&band);
					fprintf(repor,"Al  ");
					imprimir_fecha_repor(d,m,a);
					if(ocupadas<10)
						fprintf(repor,"               0%d           Habitaciones ocupadas",ocupadas);
					else
						fprintf(repor,"               %d           Habitaciones ocupadas",ocupadas);
			break;
			
			case 'k': //Habitaciones reservadas (Bloqueadas)
					//fscanf(ope,"%d %d %d ",&di,&mi,&ai);
					
					pos1=fecha_a_dias(d,m,a);
					
					for(i=0;i<cant_hab;i++)
					{
						habi_reser[i][pos1].ocupada_dia=TRUE;
						reservadas++;
					}
					
					bandera_repor(&band);
					fprintf(repor,"Al  ");
					imprimir_fecha_repor(d,m,a);
					if(reservadas<10)
						fprintf(repor,"               0%d           Habitaciones reservadas",reservadas);
					else
						fprintf(repor,"               %d           Habitaciones reservadas",reservadas);
			break;
			
			case 'l':  //Habitaciones libres
					//fscanf(ope,"%d %d %d ",&di,&mi,&ai);
					
					for(i=0;i<piso;i++)
					{
						for(j=0;j<num_habi[i];j++)
						{
							matriz[i][j].disponible=TRUE;
							disponibles++;
						}
					}
					
					bandera_repor(&band);
					fprintf(repor,"Al  ");
					imprimir_fecha_repor(d,m,a);
					if(disponibles<10)
						fprintf(repor,"               0%d           Habitaciones libres",disponibles);
					else
						fprintf(repor,"               %d           Habitaciones libres",disponibles);

			break;
		}
	}
}

void inicializar(habitacion matriz[N][N])
{
	int hab,i,j;
	fscanf(inici,"%f",&presu);
	fscanf(inici,"%d %d %d",&d,&m,&a);
	fscanf(inici,"%d %d",&cant_hab,&piso);
	for(i=0;i<piso;i++)
	{
		fscanf(inici,"%d",&hab);
		num_habi[i]=hab;
		for(j=0;j<hab;j++)
		{
			fscanf(inici,"%s",matriz[i][j].tipo);
			matriz[i][j].disponible=TRUE;
		/*	if(strcmp(matriz[i][j].tipo,"INDIV")==FALSE)
				matriz[i][j].capacidad=1;
			if(strcmp(matriz[i][j].tipo,"DOBLE")==FALSE)
				matriz[i][j].capacidad=3;
			if(strcmp(matriz[i][j].tipo,"MATRI")==FALSE)
				matriz[i][j].capacidad=2;
			if(strcmp(matriz[i][j].tipo,"CUADR")==FALSE)
				matriz[i][j].capacidad=4;
			if(strcmp(matriz[i][j].tipo,"SUITE")==FALSE)
				matriz[i][j].capacidad=10;*/
		}
	}	
	fprintf(conta,"FECHA      |DEBE           |HABER          |SALDO          |CONCEPTO       \n");
	imprimir_dia();
	hab=cuenta_digito(presu);
	for(i=0;i<12-hab;i++)
	{
		fprintf(conta," ");
	}
	fprintf(conta,"%.2f|               |",presu);
	hab=cuenta_digito(presu);
	for(i=0;i<12-hab;i++)
	{
		fprintf(conta," ");
	}
	fprintf(conta,"%.2f|Saldo inicial",presu);
	cargar_hab_res();
	for(i=0;i<N;i++)
	{
		for(j=0;j<M;j++)
		{
			habi_reser[i][j].c_titular=0;
			habi_reser[i][j].ocupada_dia=FALSE;
			habi_reser[i][j].res_canceladas=0;
			habi_reser[i][j].res_efectivas=0;
			habi_reser[i][j].cont_adul=0;
			habi_reser[i][j].cont_nino=0;
			habi_reser[i][j].uso_piso=0;
			habi_reser[i][j].ingr_cama=0;
			habi_reser[i][j].ingr_caja=0;
		}
	}
}
	
void leer_costos()
{
	int i=0;
	fscanf(pre,"%d %d %d",&p.habi,&p.adi,&p.comi);
	for(i=0;i<p.habi;i++)
	{
		fscanf(pre,"%s",p.nom[i][0].cad);
		fscanf(pre,"%f",&p.preci[i][0]);
	}
	for(i=0;i<p.adi;i++)
	{
		fscanf(pre,"%s",p.nom[i][1].cad);
		fscanf(pre,"%f",&p.preci[i][1]);
	}
	for(i=0;i<p.comi;i++)
	{
		fscanf(pre,"%s",p.nom[i][2].cad);
		fscanf(pre,"%f",&p.preci[i][2]);;
	}
}

void leer_operaciones(habitacion matriz[N][N])
{
	int n;
	while(!feof(ope))
	{
		fscanf(ope,"%d",&n);
		switch(n)
		{
			case 0: finalizar_dia(matriz); break;
			case 1: reservar(matriz); break;
			case 2: cancelar_reservacion(matriz); break;
			case 3: check_in(matriz); break;
			case 4: check_out(matriz,0,0,0); break;
			case 5: restaurant(); break;
			case 6: solicitar_servicios(matriz); break;
			case 7: reporte(matriz); break;
			case 8: break; //sobrecarga
		}
	}
}

int main()
{
	inici=fopen("inicializar.in","r");
	pre=fopen("precios.in","r");
	ope=fopen("operaciones.in","r");
	sobre=fopen("sobrecarga.in","r");
	reser=fopen("reservaciones.out","w");
	conta=fopen("contabilidad.out","w");
	repor=fopen("reportes.out","w");
	sobreout=fopen("sobrecarga.out","w");
	habitacion matriz[N][N];
	inicializar(matriz);
	leer_costos();
	leer_operaciones(matriz);
	fclose(inici);
	fclose(pre);
	fclose(ope);
	//fclose(sobre);
	fclose(reser);
	fclose(conta);
	fclose(repor);
	//fclose(sobreout);
	return 0;
}
