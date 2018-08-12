struct WF{
	vector<vector<int> > V; //Вектор файлов []вектор интов
	vector<double> tf_idf;
};

void classes(const vector<WF> &word_file, const vector<string> &file_list, int N){
	double n,m,length_i,length_j;  //Числитель и знаменатель
	int rank_class[N][N]={0};  //Отношения файлв
	
	for (int i=0; i<N-1; i++){  //проверяем отношение каждого файла с каждим один раз
		for (int j=i+1; j<N; j++){
			n=0; m=0; length_i=0; length_j=0;
			//вычисляем числитель
			for (int t=0; t<word_file.size(); t++){
				n+=word_file[t].tf_idf[i]*word_file[t].tf_idf[j];
			}
			//вычисляем знаменатель
			for (auto c: word_file){
				length_i+=c.tf_idf[i]*c.tf_idf[i];
				length_j+=c.tf_idf[j]*c.tf_idf[j];
			}
			sqrt(length_i); sqrt(length_j);
			m=length_i*length_j;
			rank_class[i][j]=n/m;
			rank_class[j][i]=n/m;
		}
	}
	cout<<endl<<endl;
	for(int i=1; i<=N; i++){
		cout<<i<<") "<<file_list[i-1]<<endl;
	}
	cout<<endl;
	cout<<"    ";
	for (int i=1; i<=N; i++){
		cout<<" ";
		cout<<setw(2)<<i;
		cout<<"   ";
	}
	cout<<endl;
	for (int i=0; i<N; i++){
		cout<<setw(2)<<i+1<<":"; cout<<" ";
		for (int j=0; j<N; j++){
			cout<<setw(5)<<rank_class[i][j]; cout<<" ";
		}
		cout<<endl;
	}
	return;
}

vector<int> classify(const vector<WF> &word_file, const vector<string> &file_list, int N){
	double n,m,length_i,length_j;  //Числитель и знаменатель
	int rank_class[N][N]={0};  //Отношения файлв
	int num;
	vector<vector<int> > classes;  //вектор по классам в котором номера файлов
	vector<int> file,filter;
	
	for (int i=0; i<N-1; i++){  //проверяем отношение каждого файла с каждим один раз
		for (int j=i+1; j<N; j++){
			n=0; m=0; length_i=0; length_j=0;
			//вычисляем числитель
			for (int t=0; t<word_file.size(); t++){
				n+=word_file[t].tf_idf[i]*word_file[t].tf_idf[j];
			}
			//вычисляем знаменатель
			for (auto c: word_file){
				length_i+=c.tf_idf[i]*c.tf_idf[i];
				length_j+=c.tf_idf[j]*c.tf_idf[j];
			}
			sqrt(length_i); sqrt(length_j);
			m=length_i*length_j;
			rank_class[i][j]=n/m;
			rank_class[j][i]=n/m;
		}
	}
	for (int i=0; i<N; i++) file.push_back(i);
	classes.resize(classes.size()+1);
	classes[0].push_back(0);
	file.erase(file.begin());
	for (int i=0,flag=0; i<classes.size(); i++){
		flag=0;
		ret:
		for (int j=0; j<file.size(); j++){
			if (rank_class[file[j]][classes[i][0]]>=200) { classes[i].push_back(file[j]); file.erase(file.begin()+j); goto ret; }
			else{ if (flag==0){
				classes.resize(classes.size()+1);
				classes[classes.size()-1].push_back(file[j]);
				file.erase(file.begin()+j);
				flag=1;
				goto ret;
			}}
		}
	}
	cout<<"Сформированные классы:"<<endl;
	for (int i=0; i<classes.size(); i++){
		cout<<"Класс "<<i+1<<":"<<endl;
		for (auto c: classes[i]) cout<<file_list[c]<<endl;
		cout<<endl;
	}
	cout<<"Номер изпользуемого фильтра: "; cin>>num;
	if (num!=0) for (auto c: classes[num-1]) filter.push_back(c);
	else for (int i=0; i<N; i++) filter.push_back(i);
	return filter;
}

vector<string> menu(const vector<string> &file_listall, int countN){  //MENU
	int ch=0,key;
	vector<string> list;
	int check[file_listall.size()]={0};
loop1:
	system("cls");
	cout<<endl<<endl;
	if (ch==0) cout<<"       [Выбрать файлы]      Использовать все файлы";
	if (ch==1) cout<<"        Выбрать файлы      [Использовать все файлы]";
	key=getch();
	switch(key){
		case 75:{ if (ch>0) ch--; goto loop1;}
		case 77:{ if (ch<1) ch++; goto loop1;}
		case 13: break;
		default: goto loop1;
	}
	if (ch==1) return file_listall;
	
	ch=0;
loop2:
	system("cls");
	cout<<endl;
	for (int i=0; i<countN; i++){
		if (ch==i) cout<<"->"; else cout<<"  ";
		if (check[i]==1) cout<<"[*] "; else cout<<"[ ] ";
		cout<<file_listall[i];
		cout<<endl;
	}
	cout<<endl;
	if (ch!=countN) cout<<"   Установить"; else cout<<"  [Установить]";
	key=getch();
	switch(key){
		case 72: { if (ch>0) ch--; goto loop2; }
		case 80: { if (ch<countN) ch++; goto loop2; }
		case 77: { ch=countN; goto loop2; }
		case 13: {
			if (ch!=countN){
				if (check[ch]==0) {check[ch]=1; goto loop2;}
				if (check[ch]==1) {check[ch]=0; goto loop2;}
			}
			else{
				for (int i=0; i<countN; i++) if(check[i]==1) list.push_back(file_listall[i]);
				if(!list.empty()) return list; else goto loop2;
			}
		}
		case 8:{ ch=0; goto loop1; }
		default: goto loop2;
	}
}

void file_info(const vector<WF> &word_file, const vector<string> &file_list, int N){
	double S[N];
	for (int i=0; i<word_file.size(); i++) for (int j=0; j<N; j++) S[j]+=word_file[i].tf_idf[j];
	cout<<endl<<"Ранг файлов:"<<endl;
	for (int i=0; i<N; i++){
		cout<<file_list[i]<<": ";
		cout<<S[i]<<endl;
		cout<<endl;
	}
}

void word_info(const vector<WF> &word_file, const vector<string> &file_list, int N, const vector<string> &word_list){
	string word;
	ret:
	cout<<endl<<"Введите слово: "; cin>>word; cout<<endl;
	if (word=="exit") return;
	for (int j=0; j<file_list.size(); j++){
		for (int i=0; i<word_list.size(); i++) if (word_list[i]==stem(word)) {cout<<file_list[j]<<": "<<word_file[i].tf_idf[j]<<endl; goto next;}
		cout<<"Слово не найдено"; goto ret;
		next:;
	}
	cout<<"Стем: "<<stem(word)<<endl;
	goto ret;
}
