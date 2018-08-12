#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <string>
#include <vector>
#include <locale.h>
#include <map>
#include <windows.h>
#include <conio.h>
#include "stem.cpp"
#include "extended.cpp"
using namespace std;

typedef FILE* pFILE;
typedef pFILE* ppFILE;

/*struct WF{
	vector<vector<int> > V; //������ ������ []������ �����
	vector<double> tf_idf;
};*/
struct SW{
	int file,pos,check,neighbor;
	double rank;
};
SW get_SW(int file, int pos, int check, int neighbor){SW temp; temp.file=file; temp.pos=pos; temp.rank=0.0; temp.check=check; temp.neighbor=0; return temp;}
main(){
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_CTYPE, "rus");
	ppFILE F;
	FILE *out,*in;
	vector<map<int,int> > number_list;  //number_list - ������ ������; number_list[i] - ������ ���: ������� ����� � ������ � � ������� word_list
	vector<string> file_list,word_list,seek_list,file_listall;
	vector<WF> word_file;
	vector<SW> seek_word;
	vector<int> filter;
	string file,temp_str,str;
	char word[31],temp_cstr[31],ch[100];//����������� �� ������ �����
	int N,flag,count,temp,id,countN,filter_on=0,max_rezult=10,num;
	
	in=fopen("input.txt","r");
	fscanf(in,"%d",&countN);
	for (int i=0; i<countN; i++){ //������ �������� ���� ������
		fscanf(in,"%s",&temp_cstr);
		file=temp_cstr;
		if (file[file.size()-1]!='t' && file[file.size()-2]!='x' && file[file.size()-3]!='t' && file[file.size()-4]!='.') file+=".txt";
		file_listall.push_back(file);
	}
	file_list=menu(file_listall,countN);
	N=file_list.size();
	system("cls");
	cout<<"�������� �������***"<<endl;
	
	F = new pFILE[N];
	int file_count[N];
	number_list.resize(N);
	for (int i=0; i<N; i++) F[i]=fopen(file_list[i].c_str(),"r");
	filter.resize(N);
	
	for (int q=0; q<N; q++){//���� ������������ ���� � �������� �������
		count=0;
		flag=fscanf(F[q],"%s",&word);
		count++;
		while(flag!=-1){
			if ((word[0]<'�' || word[0]>'�') && (word[0]<'�' || word[0]>'�')) {flag=fscanf(F[q],"%s",&word); count++; continue;}
			if (!((word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�') || (word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�')))
			word[strlen(word)-1]='\0';
			if (!((word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�') || (word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�')))
			word[strlen(word)-1]='\0';
			temp_str=word;
			temp_str=stem(temp_str);
			strcpy(word,temp_str.c_str());//������������
			
			for (int i=0; i<word_list.size(); i++) if (word_list[i]==word){
				word_file[i].V[q].push_back(count);
				number_list[q][count]=i;
				flag=fscanf(F[q],"%s",&word);
				count++;
				goto loop2;
			}
			temp_str=word;
			word_list.push_back(temp_str);
			word_file.resize(word_file.size()+1);
			word_file[word_file.size()-1].V.resize(N);
			word_file[word_file.size()-1].V[q].push_back(count);
			number_list[q][count]=word_list.size()-1;
			flag=fscanf(F[q],"%s",&word);
			count++;
			loop2:;
		}
		file_count[q]=count;
	}
	
	int word_presence; double tf=0.0,idf=0.0;  //���������� tf-idf
	for (int i=0; i<word_list.size(); i++) word_file[i].tf_idf.resize(N);
	for (int i=0; i<word_list.size(); i++){
		for (int q=0; q<N; q++){
			tf=0.0; idf=0.0; word_presence=0;
			tf=(double)(word_file[i].V[q].size())/(double)file_count[q];
			for (auto c: word_file[i].V) if (!(c.empty())) word_presence++;
			idf=log2((double)N/(double)(word_presence));
			word_file[i].tf_idf[q]=tf*idf;
		}
	}
	
	out=fopen("output.txt","w");
	for (int i=0; i<word_list.size(); i++){//����� �������
		fprintf(out,"%s\n",word_list[i].c_str());
		for (int j=0; j<N; j++){
			fprintf(out,"%s %d %.10lf\n",file_list[j].c_str(),word_file[i].V[j].size(),word_file[i].tf_idf[j]);
			for (auto c: word_file[i].V[j]) fprintf(out,"%d ",c);
			fprintf(out,"\n");
		}
		fprintf(out,"\n\n");
	}
	
//====================================================================================================================================================================
	cout<<"***�������� ������� ���������";
rep:
	
	cout<<endl<<"������� ��������� ������: "<<endl;
	getline(cin,str);//������ ������ ��� ������
	
	if (str=="exit") return 0;   //�������������� �������
	if (str=="classes") { classes(word_file,file_list,N); goto rep;}
	if (str=="classify"){ filter_on=1; filter=classify(word_file,file_list,N); getline(cin,str); goto rep;}
	if (str=="max_change") { cout<<endl<<"������� ������������ ���������� �����������: "; cin>>max_rezult; getline(cin,str); goto rep; }
	if (str=="file_info") { file_info(word_file,file_list,N); goto rep; }
	if (str=="word_info") { word_info(word_file,file_list,N,word_list); getline(cin,str); goto rep; }
	if (str=="stem") {
		cout<<endl<<"������� ����� ����������: ";
		cin>>num; cout<<endl;
		if(num!=0){
			num--;
			if (num<=seek_word.size()) for (int i=-10; i<=10; i++) cout<<word_list[number_list[seek_word[num].file][seek_word[num].pos+i]]<<" ";
		}
		else for (auto c: seek_list) cout<<stem(c)<<" ";
		getline(cin,str); goto rep;
	}
	if (str=="help") {
		cout<<endl<<"����������� ������� ���������:"<<endl<<"exit - ����� �� ���������"<<endl;
		cout<<"classes - ������� ����������� ������ �� �������"<<endl;
		cout<<"classify - ������������� ������ �� ������� � ������������� ���������� ������� (0 - ��� ������)"<<endl;
		cout<<"max_change - ��������� ������������� ���������� ��������� �����������"<<endl;
		cout<<"file_info - ���������� � ������"<<endl;
		cout<<"word_info - ���������� � ��������� ��������� ����� (��� ������ exit)"<<endl;
		cout<<"stem - �������� �������������� ��������� ��� ������(0)"<<endl;
		goto rep;
	}
	
	seek_list.clear();  seek_word.clear();
	
	cout<<endl;
	id=str.find(" ");
	while(id!=-1){//��������� �� �����
		seek_list.push_back(stem(str.substr(0,id)));
		str.erase(0,id+1);
		id=str.find(" ");
	}
	seek_list.push_back(stem(str.substr(0,id)));
	
	int check,neighbor; //������� ����������
	for (int t=0; t<seek_list.size(); t++){  //������ �� ������� ������
		for (int j=0; j<word_list.size(); j++) if(word_list[j]==seek_list[t]){//����� �����
			for(int q=0; q<N; q++){
				for (auto c: word_file[j].V[q]){  //���� �� ���� ���������������� ������� �����
					check=0; neighbor=0;
					if (!seek_word.empty()) for (auto d: seek_word) if (abs(d.pos-c)<=10) goto next_c; //���� ��������� �������� ����� ������ � ������ �����������
					for (auto d: seek_list){
						for (int i=-10; i<=10; i++){
						if (word_list[number_list[q][c+i]]==d) { //���-�� ����������
							check++;
							if (abs(i)==1) neighbor=1;
							goto next_d;
						}  
					}
					next_d:;
				}
					if(filter_on==1){ for (auto d: filter) if (d==q) seek_word.push_back(get_SW(q,c,check,neighbor)); }
					else seek_word.push_back(get_SW(q,c,check,neighbor));
					next_c:;
				}
			}
		}
	}
	//cout<<seek_word.size();
	//return 0;
	if (seek_word.empty()) {cout<<"���������� �� �������"<<endl; goto rep;}
	
	for (int j=0; j<seek_word.size(); j++){
		for (int i=-10; i<=10; i++){ //��������� ����� ��� �������� ���������
			seek_word[j].rank=seek_word[j].rank+word_file[number_list[seek_word[j].file][seek_word[j].pos+i]].tf_idf[seek_word[j].file];
			if (seek_word[j].neighbor==1) seek_word[j].rank*=10; 
		}
	}
	
	SW temp_SW;
	for (int i=0; i<seek_word.size()-1; i++){//���������� �������� ��������� �� ���������� ��������� ����
	for (int j=i+1; j<seek_word.size(); j++){
		if (seek_word[j].check>seek_word[i].check){
			temp_SW=seek_word[i];
			seek_word[i]=seek_word[j];
			seek_word[j]=temp_SW;
		}
	}
}
	for (int i=0; i<seek_word.size()-1; i++){//���������� �������� ��������� �� �����
	for (int j=i+1; j<seek_word.size(); j++){
		if (seek_word[j].check==seek_word[i].check)
		if (seek_word[j].rank>seek_word[i].rank){
			temp_SW=seek_word[i];
			seek_word[i]=seek_word[j];
			seek_word[j]=temp_SW;
		}
	}
}
	
	//������������� �����
	int qu,cnt=1;
	for (auto c: seek_word){   //���� �� ���� �������� ���������
		if (cnt>max_rezult) break;
		cout<<cnt<<") ";
		cnt++;
	qu=c.file;
	for (int i=0; i<N; i++) fclose(F[i]);   //��������
	for (int i=0; i<N; i++) F[i]=fopen(file_list[i].c_str(),"r");   //� �������� ������
	
		count=0;
		flag=fscanf(F[qu],"%s",&word);
		count++;
		while(flag!=-1){
			if ((word[0]<'�' || word[0]>'�') && (word[0]<'�' || word[0]>'�')) {flag=fscanf(F[qu],"%s",&word); count++; continue;}
			if (!((word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�') || (word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�')))
			word[strlen(word)-1]='\0';
			if (!((word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�') || (word[strlen(word)-1]>='�' && word[strlen(word)-1]<='�')))
			word[strlen(word)-1]='\0';   //������������
			if (count>=c.pos-10 && count<=c.pos+10) cout<<word<<" ";  //������� ������ ������ �����
			if (count>c.pos+10) break;
			flag=fscanf(F[qu],"%s",&word);
			count++;
		}
		cout<<endl;   //��� ����
		cout<<"   ����: "<<file_list[c.file]<<"  ������� ������� �����: "<<c.pos<<endl<<"  ����: ";
		printf("%.10lf",c.rank);
		cout<<endl<<endl;
	}
	goto rep;
}
