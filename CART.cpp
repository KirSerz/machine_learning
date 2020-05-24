#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <algorithm> 
#include <vector>
#include <sstream> 

using namespace std;
const unsigned int N = 7;

struct Node
{
	string x;//атрибут разбиения
	int id_stolb;//id столбца 
	bool flag_node;//флаг показывающей наличие пустых потомков
	Node *l, *r; //указатели на новые звенья.
};

void print_Tree(Node *&Tree, int level)
{
	if (Tree)
	{
		print_Tree(Tree->l, level + 1);
		for (int i = 0; i< level; i++) cout << "	";
		cout << Tree->x <<"["<<Tree->id_stolb <<"]"<<endl;
		print_Tree(Tree->r, level + 1);
	}
}
/*Добавили очистку памяти*/
void del(Node *&Tree){
	if (Tree != NULL) 
	{
		del(Tree->l); 
		del(Tree->r); 
		delete Tree; 
		Tree = NULL; 
	}
}

/*определяем числовой или категориальный критерий. 0-число 1-нет*/
bool num(string a){
	bool num = 0;
	stringstream s;
	s << a;
	long result;
	s >> result;
	if (!s) 
		num = 1;
	return num;
}

int max(vector<double> arr, int ind){
	double max;
	int max_id = 0;
	max = 0;
	for (int j = 0; j < ind; j++)
		if (arr[j] > max&&arr[j] >= 0){
			max = arr[j];
			max_id = j;
		}
	return max_id;
}

/*функция считывания данных из файла*/
int size_file(string file_name){
	int size_mass = 0;
	ifstream data_file;
	data_file.open(file_name);
	string line;
	while (getline(data_file, line)) {
		size_mass++;
	}
	data_file.close();
	return size_mass;
}

vector<vector<string>>data(int size_mass, string file_name){
	vector<vector<string>>st_1;
	ifstream data_file;
	data_file.open(file_name);
	int i = 0;
	while (!data_file.eof())
	{
		st_1.push_back(vector<string>(N));
		for (int j = 0; j < N-1;j++)
		{
			getline(data_file, st_1[i][j], '\t');
		}
		getline(data_file, st_1[i][N-1], '\n');
		i++;
		if (data_file.eof() || i == size_mass)
			break;
	}
	return st_1;
}

vector<vector<string>> id_and_summ(vector<vector<string>> st_1, int j, int size_mass){
	vector<vector<string>>id_and_summ;
	int buff = 0;
	//cout << "//id_and_summ\\" << endl;
	id_and_summ.push_back(vector<string>(2));
		for (int i = 0; i <size_mass; i++)		
			for (int n = 0; n < id_and_summ.size(); n++)	{
					if (st_1[i][j] == id_and_summ[n][0])
						break;
					if (n == id_and_summ.size() - 1)
						{						
						id_and_summ[id_and_summ.size() - 1][0] = st_1[i][j];
						id_and_summ.push_back(vector<string>(2)); 
						break;
						}				
					}		
			if (num(st_1[0][j]) == 1)
			{
				for (int n = 0; n < id_and_summ.size(); n++){
					for (int i = 0; i < size_mass; i++)
						if (id_and_summ[n][0] == st_1[i][j])
							buff++;				
					id_and_summ[n][1] = to_string(buff);
					buff = 0;
				}
			}
			else
			{
				double max = 0;
				int id_max = 0;
				for (int n = 0; n < id_and_summ.size(); n++){
					//cout << id_and_summ[n][0]<< endl;
					for (int i = 0; i < size_mass; i++)
					if (atof(id_and_summ[n][0].c_str()) >= atof(st_1[i][j].c_str()))
						buff++;

					id_and_summ[n][1] = to_string(buff);
					if (atof(id_and_summ[n][0].c_str()) >= max)
					{
						max = atof(id_and_summ[n][0].c_str());
						id_max = n;
					}
					buff = 0;
				}
				if (id_and_summ.size() > 2)
				{
					if (id_max == 0)
						id_and_summ.erase(id_and_summ.begin() + id_max);
					else
						id_and_summ.erase(id_and_summ.begin() + (id_max - 1));
				}


			}
		return id_and_summ;
}

int Kol_vo(vector<vector<string>> st_1, int j, string a, string c, int size_mass){			//j это номер проверяемого столбца
	int Kol_vo = 0;
	for (int i = 0; i < size_mass; i++)
		if (st_1[i][j] == c)
			if (st_1[i][N-1] == a)
				Kol_vo++;
	//cout <<c<<"  "<< "Kol_vo="<<Kol_vo<<"  "<<a<<endl;
	return Kol_vo;
}

vector<string> classes_kriter(int j, string mass[6][50]){//возвращает виды атрибутов в j столбце.
	vector <string>buff_mass;
	buff_mass.push_back(mass[j][0]);
	for (int i = 0; i < 50; i++)
		for (int c = 0; c < buff_mass.size(); c++)	
			if (mass[j][i] == buff_mass[c]){
				buff_mass.push_back(mass[j][i]);
				break;
			}
	return buff_mass;
}

double new_Q(vector<vector<string>> st_1, int j, int size_mass, int v, vector<vector<string>>temp){
	string a = "Da", b = "net";
	double Pl, Pr, p1, p2, p1_1=0, p2_1=0, Q = 0, W = 0, buff=0;

	Pl = atof(temp[v][1].c_str()) / size_mass;	
	Pr = (size_mass - atof(temp[v][1].c_str())) / size_mass;
	
	p1 = Kol_vo(st_1, j, a, temp[v][0], size_mass) / atof(temp[v][1].c_str());
	p2 = Kol_vo(st_1, j, b, temp[v][0], size_mass) / atof(temp[v][1].c_str());
	
	for (int i = 0; i < temp.size(); i++)
		if (i!=v)
		{
			p1_1 += Kol_vo(st_1, j, a, temp[i][0], size_mass);
			p2_1 += Kol_vo(st_1, j, b, temp[i][0], size_mass);
		}
	
	p1_1 = p1_1 / (size_mass - atof(temp[v][1].c_str()));
	p2_1 = p2_1 / (size_mass - atof(temp[v][1].c_str()));
	
	W = abs(p1 - p1_1) + abs(p1 - p1_1);
	Q = 2 * Pl*Pr*W;
	return Q;
}

string Cart_raschet(vector<vector<string>> st_1, int size_mass){
	//cout << "cart_raschet" << endl;
	int max_id = 0, ind = 0,size=0;
	vector<double>answer;
	vector<string>kriteri;
	vector<vector<string>>temp;
	string  enter;
	for (int j = 0; j < N-1; j++)
	{
		temp = id_and_summ(st_1, j, size_mass); 
		size = temp.size();
		
		for (int i = 0; i < size; i++)
		{			
			answer.push_back(new_Q(st_1, j, size_mass, i,temp));
			kriteri.push_back(temp[i][0]);
			ind++;
		}
	}
	max_id = max(answer, ind);
	enter = kriteri[max_id];
	return enter;
}

int stolb(vector<vector<string>> st_1, string enter, int size_mass){
	int stolb;
	for (int i = 0; i < size_mass; i++)
		for (int j = 0; j < N - 1; j++)
				if (enter==st_1[i][j])
					stolb=j;
	return stolb;
}
/*функция возвращает 0/1 если в массиве есть различные значения*/
bool yes_or_no(vector<vector<string>>v){
	bool flag = 0;
	if (v.size()!=0)
		for (int i = 0; i < v.size(); i++)		
			if (v[0][N - 1] != v[i][N - 1])
			{
				flag = 1;
				break;
			}
	return flag;
}

void add_node(string answer, Node *&MyTree, vector<vector<string>>st_1, int size_mass){
	cout << "------------------------------------------------------" << endl;
	bool flag_r = 0, flag_l = 0;
	vector<vector<string>>left_v;
	vector<vector<string>>right_v;
	answer = Cart_raschet(st_1, size_mass);
	
	if (NULL == MyTree) //создаем дерево 
	{
		cout << "new" << endl;
		MyTree = new Node;
		MyTree->x = answer;
		MyTree->id_stolb = stolb(st_1, MyTree->x, size_mass);
		MyTree->flag_node = 1;
		MyTree->l = MyTree->r = NULL;
	}

	/*делим массив данных согласно значению в узле*/
	for (int i = 0; i < size_mass; i++){		
		if (num(MyTree->x)==0)//функция num возвращает 0, то в стринговой переменной "х" числовой критерий если 1 то в 
								//переменной х находится котегориальный критерий
		{
			if (atof(st_1[i][MyTree->id_stolb].c_str()) > atof(MyTree->x.c_str())){
				right_v.push_back(vector<string>(N));
				copy(st_1[i].begin(), st_1[i].end(),right_v[right_v.size() - 1].begin());
			}
			else{
				left_v.push_back(vector<string>(N));
				copy(st_1[i].begin(), st_1[i].end(), left_v[left_v.size() - 1].begin());
			}	
		}
		else	{			
			if (st_1[i][MyTree->id_stolb] == MyTree->x)	{
				right_v.push_back(vector<string>(N));
				copy(st_1[i].begin(), st_1[i].end(), right_v[right_v.size() - 1].begin());
			}
			else{
				left_v.push_back(vector<string>(N));
				copy(st_1[i].begin(), st_1[i].end(), left_v[left_v.size() - 1].begin());
			}
		}
	}

	vector<vector<vector<string>>>buff_vector;
	buff_vector.push_back(left_v);
	buff_vector.push_back(right_v);
	/*   проверка на узел/лист  */

	flag_l = yes_or_no(left_v);
	flag_r = yes_or_no(right_v);

	if (flag_l == 0){                               //Лист. инициализируем его значением выходного атрибута
		MyTree->l = new Node;  
		MyTree->l->l = MyTree->l->r = NULL; 
		MyTree->l->x = left_v[0][N - 1];
		MyTree->l->id_stolb = N-1;
		MyTree->l->flag_node = 0;
	}
	else{			 //узел.
		MyTree->l = new Node; 
		MyTree->l->l = MyTree->l->r = NULL; 
		MyTree->l->x = Cart_raschet(left_v, left_v.size());	
		MyTree->l->id_stolb = stolb(left_v, MyTree->l->x, left_v.size());
		MyTree->l->flag_node = 1;
	}

	if (flag_r == 0){//Лист. инициализируем его значением выходного атрибута
		MyTree->r = new Node; 
		MyTree->r->l = MyTree->r->r = NULL;
		MyTree->r->x = right_v[0][N - 1];
		MyTree->r->id_stolb = N - 1;
		MyTree->r->flag_node = 0;
	}
	else{			//узел
		MyTree->r = new Node; 
		MyTree->r->l = MyTree->r->r = NULL; 
		MyTree->r->x = Cart_raschet(right_v, right_v.size());	
		MyTree->r->id_stolb = stolb(right_v, MyTree->r->x, right_v.size());
		MyTree->r->flag_node = 1;
	}

	cout << "						x =" << MyTree->x << " [" << MyTree->id_stolb<<"]" << endl;			
	cout << "						r->x =" << MyTree->r->x << " [" << MyTree->r->id_stolb << "]" << endl;
	cout << "						l->x =" << MyTree->l->x << " [" << MyTree->r->id_stolb << "]" << endl;
	cout << "right - " << right_v.size() << " Yzel(?)= " << flag_r << endl;									
	for (int i = 0; i < right_v.size(); i++){															
		for (int j = 0; j < N; j++)																		
			cout << " " << right_v[i][j] << " ";															
		cout << endl;	}																					
	cout << endl;																							
	cout << "left - " << left_v.size() << " Yzel(?)= " << flag_l << endl;									
	for (int i = 0; i < left_v.size(); i++){																
		for (int j = 0; j < N; j++)																	
			cout << " " << left_v[i][j] << " ";																
		cout << endl;	}																					
	cout << endl;																							
	
	
	/*if (MyTree->r->flag_node == 0 && MyTree->l->flag_node == 0)	
		MyTree->flag_node = 0;//если значение флагов 
							//потомков равно 0 присваиваем флагу узла 0
	else{*/
		if (flag_l == 1 && MyTree->l->flag_node != 0)
			add_node(answer, MyTree->l, left_v, left_v.size());//вход в рекурсию		
		
		if (flag_r == 1 && MyTree->r->flag_node != 0)
			add_node(answer, MyTree->r, right_v, right_v.size());//вход в рекурсию
	//}
}


int main()
{
	
	string file_name="Data_2.txt";
	string vvod[N - 1], answer;
	int size_mass = size_file(file_name);
	vector<vector<string>>st_1;
	st_1=data(size_mass, file_name);
	Node *Tree = NULL;
	int level=0;
	cout << "------------------------------------------------------" << endl;
	add_node(answer, Tree, st_1, size_mass);
	cout << "-------------------------------" << endl;
	print_Tree(Tree, level);
	cout << '\n';
	del(Tree);
	system("pause");
	return 0;
}
