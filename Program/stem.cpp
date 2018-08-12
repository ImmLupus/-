#include <string>
using namespace std;

struct oconchanie{
	string ocon;
	int m;
	bool trig;  //Необходимость а или я перед окончанием
};

string stem(string word)
{
	string newocon;
	pair<int,int> rv,r1,r2;
	oconchanie max_ocon;
	
	string glas="аеиоуыэюя";
	string perf_group1[]={"в","вши","вшись"};
	string perf_group2[]={"ив","ивши","ившись","ыв","ывши","ывшись"};
	string adjective[]={"ее","ие","ые","ое","ими","ыми","ей","ий","ый","ой","ем","им","ым","ом",
	"его","ого","ему","ому","их","ых","ую","юю","ая","яя","ою","ею"};
	string partic_group1[]={"ем","нн","вш","ющ","щ"};
	string partic_group2[]={"ивш","ывш","ующ"};
	string reflex[]={"ся","сь"};
	string verb_group1[]={"ла","на","ете","йте","ли","й","л","ем","н","ло","но","ет","ют",
	"ны","ть","ешь","нно"};
	string verb_group2[]={"ила","ыла","ена","ейте","уйте","ите","или","ыли","ей","уй","ил","ыл",
	"им","ым","ен","ило","ыло","ено","ят","ует","уют","ит","ыт","ены","ить","ыть","ишь","ую","ю"};
	string noun[]={"а","ев","ов","ие","ье","е","иями","ями","ами","еи","ии","и","ией","ей","ой",
	"ий","й","иям","ям","ием","ем","ам","ом","о","у","ах","иях","ях","ы","ь","ию","ью","ю","ия","ья","я"};
	string superlative[]={"ейш","ейше"};
	string derivational[]={"ост","ость"};
 	
	for (int i=0; i<word.size(); i++) {//Первичная обработка
		if (word[i]=='ё') word[i]='е';
		if (word[i]<'а') word[i]+='а'-'А';
	}
	
	for (int i=0; i<word.size(); i++) //нахождение rv
	for (int j=0; j<9; j++) if (word[i]==glas[j]){
		i++;
		rv.first=i;
		rv.second=word.size()-1;
		goto outrv;
	}
outrv:
	for (int i=0; i<word.size(); i++) //нахождение r1
	for (int j=0,f=0; j<9; j++) if (word[i]==glas[j]){
		for (int q=0; q<9; q++) if (word[i+1]==glas[q]) f=1;
		if (f==1) continue;
		i+=2;
		r1.first=i;
		r1.second=word.size()-1;
		goto outr1;
	}
outr1:
	for (int i=r1.first; i<=r1.second; i++) //нахождение r2
	for (int j=0,f=0; j<9; j++) if (word[i]==glas[j]){
		for (int q=0; q<9; q++) if (word[i+1]==glas[q]) f=1;
		if (f==1) continue;
		i+=2;
		r2.first=i;
		r2.second=word.size()-1;
		goto outr2;
	}
outr2:
	//cout<<endl<<rv.first<<" "<<rv.second<<endl<<r1.first<<" "<<r1.second<<endl<<r2.first<<" "<<r2.second;
	
	//if (rv.first==0 && rv.second==0) goto exit;
	//Шаг 1
	max_ocon.ocon=""; max_ocon.m=0; max_ocon.trig=0;
	for (int i=0,s=0; i<sizeof(perf_group1)/sizeof(string); i++,s=0) {//нахождение perf_group1
		if (word.size()-rv.first<perf_group1[i].size()+1) continue;
		for (int j=0; j<perf_group1[i].size(); j++){
			if (word[word.size()-1-j]==perf_group1[i][perf_group1[i].size()-1-j]) s++;
		}
		if (s==perf_group1[i].size() && (word[word.size()-perf_group1[i].size()-1]=='а' || word[word.size()-perf_group1[i].size()-1]=='я')){
			if (perf_group1[i].size()>max_ocon.m){
				max_ocon.ocon=perf_group1[i];
				max_ocon.m=perf_group1[i].size();
				max_ocon.trig=1;
			}
		}
	}
	for (int i=0,s=0; i<sizeof(perf_group2)/sizeof(string); i++,s=0) {//нахождение perf_group2
		if (word.size()-rv.first<perf_group2[i].size()) continue;
		for (int j=0; j<perf_group2[i].size(); j++){
			if (word[word.size()-1-j]==perf_group2[i][perf_group2[i].size()-1-j]) s++;
		}
		if (s==perf_group2[i].size()){
			if (perf_group2[i].size()>max_ocon.m){
				max_ocon.ocon=perf_group2[i];
				max_ocon.m=perf_group2[i].size();
				max_ocon.trig=0;
			}
		}
	}
	if (max_ocon.ocon!="" && max_ocon.m!=0){//Удаление
		word.erase(word.size()-max_ocon.ocon.size());
		goto outstep1;
	}
	
	max_ocon.ocon=""; max_ocon.m=0; max_ocon.trig=0;
	for (int i=0,s=0; i<sizeof(reflex)/sizeof(string); i++,s=0) { //Нахождение reflexive
		if (word.size()-rv.first<reflex[i].size()) continue;
		for (int j=0; j<reflex[i].size(); j++){
			if (word[word.size()-1-j]==reflex[i][reflex[i].size()-1-j]) s++;
		}
		if (s==reflex[i].size()) word.erase(word.size()-reflex[i].size());
	}
	
	max_ocon.ocon=""; max_ocon.m=0; max_ocon.trig=0;
	for (int i=0,s=0; i<sizeof(adjective)/sizeof(string); i++,s=0) { //Нахождение adjective
		if (word.size()-rv.first<adjective[i].size()) continue;
		for (int j=0; j<adjective[i].size(); j++){
			if (word[word.size()-1-j]==adjective[i][adjective[i].size()-1-j]) s++;
		}
		if (s==adjective[i].size()){
			if (adjective[i].size()>max_ocon.m){
				max_ocon.ocon=adjective[i];
				max_ocon.m=adjective[i].size();
				max_ocon.trig=0;
			}
		}
	}
	for (int i=0; i<sizeof(partic_group1)/sizeof(string); i++) { //Нахождение partic_group1 + adjective
		for (int j=0,s=0; j<sizeof(adjective)/sizeof(string); j++,s=0) {
			newocon=partic_group1[i]+adjective[j];
			if (word.size()-rv.first<newocon.size()+1) continue;
			for (int q=0; q<newocon.size(); q++){
				if (word[word.size()-1-q]==newocon[newocon.size()-1-q]) s++;
			}
			if (s==newocon.size() && (word[word.size()-newocon.size()-1]=='а' || word[word.size()-newocon.size()-1]=='я')){
				if (newocon.size()>max_ocon.m){
					max_ocon.ocon=newocon;
					max_ocon.m=newocon.size();
					max_ocon.trig=1;
				}
			}
		}
	}
	for (int i=0; i<sizeof(partic_group2)/sizeof(string); i++) { //Нахождение partic_group2 + adjective
		for (int j=0,s=0; j<sizeof(adjective)/sizeof(string); j++,s=0) {
			newocon=partic_group2[i]+adjective[j];
			if (word.size()-rv.first<newocon.size()) continue;
			for (int q=0; q<newocon.size(); q++){
				if (word[word.size()-1-q]==newocon[newocon.size()-1-q]) s++;
			}
			if (s==newocon.size()){
				if (newocon.size()>max_ocon.m){
					max_ocon.ocon=newocon;
					max_ocon.m=newocon.size();
					max_ocon.trig=1;
				}
			}
		}
	}
	if (max_ocon.ocon!="" && max_ocon.m!=0){//Удаление
		word.erase(word.size()-max_ocon.ocon.size());
		goto outstep1;
	}
	
	max_ocon.ocon=""; max_ocon.m=0; max_ocon.trig=0;
	for (int i=0,s=0; i<sizeof(verb_group1)/sizeof(string); i++,s=0) {//нахождение verb_group1
		if (word.size()-rv.first<verb_group1[i].size()+1) continue;
		for (int j=0; j<verb_group1[i].size(); j++){
			if (word[word.size()-1-j]==verb_group1[i][verb_group1[i].size()-1-j]) s++;
		}
		if (s==verb_group1[i].size() && (word[word.size()-verb_group1[i].size()-1]=='а' || word[word.size()-verb_group1[i].size()-1]=='я')){
			if (verb_group1[i].size()>max_ocon.m){
				max_ocon.ocon=verb_group1[i];
				max_ocon.m=verb_group1[i].size();
				max_ocon.trig=1;
			}
		}
	}
	for (int i=0,s=0; i<sizeof(verb_group2)/sizeof(string); i++,s=0) {//нахождение verb_group2
		if (word.size()-rv.first<verb_group2[i].size()) continue;
		for (int j=0; j<verb_group2[i].size(); j++){
			if (word[word.size()-1-j]==verb_group2[i][verb_group2[i].size()-1-j]) s++;
		}
		if (s==verb_group2[i].size()){
			if (verb_group2[i].size()>max_ocon.m){
				max_ocon.ocon=verb_group2[i];
				max_ocon.m=verb_group2[i].size();
				max_ocon.trig=0;
			}
		}
	}
	if (max_ocon.ocon!="" && max_ocon.m!=0){//Удаление
		word.erase(word.size()-max_ocon.ocon.size());
		goto outstep1;
	}
	
	for (int i=0,s=0; i<sizeof(noun)/sizeof(string); i++,s=0) { //Нахождение noun
		if (word.size()-rv.first<noun[i].size()) continue;
		for (int j=0; j<noun[i].size(); j++){
			if (word[word.size()-1-j]==noun[i][noun[i].size()-1-j]) s++;
		}
		if (s==noun[i].size()) word.erase(word.size()-noun[i].size());
	}
	
outstep1:
	//Шаг 2
	if (word[word.size()-1]=='и') word.erase(word.size()-1);
	//Шаг 3
	for (int i=0,s=0; i<sizeof(derivational)/sizeof(string); i++,s=0) { //Нахождение derivational
		if (word.size()-r2.first<derivational[i].size()) continue;
		for (int j=0; j<derivational[i].size(); j++){
			if (word[word.size()-1-j]==derivational[i][derivational[i].size()-1-j]) s++;
		}
		if (s==derivational[i].size()) word.erase(word.size()-derivational[i].size());
	}
	//Шаг 4
	if (word[word.size()-1]=='н' && word[word.size()-2]=='н') {word.erase(word.size()-1); goto outstep4;}
	
	for (int i=0,s=0; i<sizeof(superlative)/sizeof(string); i++,s=0) { //Нахождение superlative
		if (word.size()-rv.first<superlative[i].size()) continue;
		for (int j=0; j<superlative[i].size(); j++){
			if (word[word.size()-1-j]==superlative[i][superlative[i].size()-1-j]) s++;
		}
		if (s==superlative[i].size()) word.erase(word.size()-superlative[i].size());
	}
	if (word[word.size()-1]=='н' && word[word.size()-2]=='н') {word.erase(word.size()-1); goto outstep4;}
	
	if (word[word.size()-1]=='ь') word.erase(word.size()-1);
	
	outstep4:
	return word;
}
