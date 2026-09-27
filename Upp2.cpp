#include<iostream>
#include<fstream>		//引入文件输入输出流
#include<string> 
#include<cstdlib>
using namespace std;

class Student
{public:
	Student(string nam="",float s=0):studentId(""),name(nam),gender(""),age(0),scoreA(s),scoreB(0){    }
	void add(){cin>>studentId>>name>>gender>>age>>scoreA>>scoreB;}
	void display() const
	{
		cout<<"学号："<<studentId<<endl;
		cout<<"姓名："<<name<<endl;
		cout<<"性别："<<gender<<endl;
		cout<<"年龄："<<age<<endl;
		cout<<"A课成绩："<<scoreA<<endl;
		cout<<"B课成绩："<<scoreB<<endl;
	}
	friend ostream & operator <<(ostream &,const Student &);
	friend istream & operator >>(istream &, Student &);
	friend class application;
private:
	string studentId;
	string name;
	string gender;
	int age;
	float scoreA;
	float scoreB;
};

class application
{
public:
	bool askYesNo(const string &prompt,bool &answer) const
	{
		string choice;
		while(true)
		{
			cout<<prompt<<flush;
			if(!(cin>>choice))
			{
				cerr<<"读取选择失败。"<<endl;
				return false;
			}
			if(choice=="Y"||choice=="y"||choice=="是"||choice=="\xE6\x98\xAF")
			{
				answer=true;
				return true;
			}
			if(choice=="N"||choice=="n"||choice=="否"||choice=="\xE5\x90\xA6")
			{
				answer=false;
				return true;
			}
			cout<<"请输入“是”或“否”（也可输入 Y 或 N）。"<<endl;
		}
	}

	bool findStudent(const Student students[],int count) const
	{
		if(count==0)
		{
			cout<<"当前没有学生信息。"<<endl;
			return true;
		}

		string studentId;
		cout<<"请输入要查找学生的学号："<<flush;
		if(!(cin>>studentId))
		{
			cerr<<"学号输入无效。"<<endl;
			return false;
		}

		for(int studentIndex=0;studentIndex<count;studentIndex++)
		{
			if(students[studentIndex].studentId==studentId)
			{
				cout<<"查找到的学生信息："<<endl;
				students[studentIndex].display();
				return true;
			}
		}

		cout<<"未找到学号为"<<studentId<<"的学生。"<<endl;
		return true;
	}

	bool addStudents(Student students[],int &count,int capacity) const
	{
		int additionalCount;
		cout<<"请输入要增加的学生人数（最多还能增加"<<capacity-count<<"人）："<<flush;
		if(!(cin>>additionalCount))
		{
			cerr<<"人数输入无效，学生信息未保存。"<<endl;
			return false;
		}
		if(additionalCount<1||additionalCount>capacity-count)
		{
			cerr<<"增加人数必须在 1 到 "<<capacity-count<<" 之间，学生信息未保存。"<<endl;
			return false;
		}

		for(int i=0;i<additionalCount;i++)
		{
			cout<<"请输入新增第"<<i+1<<"名学生的信息（学号 姓名 性别 年龄 A课成绩 B课成绩）："<<flush;
			students[count].add();
			if(!cin)
			{
				cerr<<"学生信息输入无效，数据文件未修改。"<<endl;
				return false;
			}
			count++;
		}
		return true;
	}

	bool deleteStudent(Student students[],int &count,bool &deleted) const
	{
		deleted=false;
		if(count==0)
		{
			cout<<"当前没有学生信息可删除。"<<endl;
			return true;
		}

		string studentId;
		cout<<"请输入要删除学生的学号："<<flush;
		if(!(cin>>studentId))
		{
			cerr<<"学号输入无效。"<<endl;
			return false;
		}

		int deleteIndex=0;
		while(deleteIndex<count&&students[deleteIndex].studentId!=studentId)
		{
			deleteIndex++;
		}
		if(deleteIndex==count)
		{
			cout<<"未找到学号为"<<studentId<<"的学生，未删除信息。"<<endl;
			return true;
		}

		for(int studentIndex=deleteIndex;studentIndex<count-1;studentIndex++)
		{
			students[studentIndex]=students[studentIndex+1];
		}
		count--;
		deleted=true;
		cout<<"操作成功！"<<endl;
		return true;
	}

	float calculateAverageScore(const Student &student) const
	{
		return (student.scoreA+student.scoreB)/2.0f;
	}

	void showRankings(const Student students[],int count) const
	{
		Student rankedStudents[20];
		for(int studentIndex=0;studentIndex<count;studentIndex++)
		{
			rankedStudents[studentIndex]=students[studentIndex];
		}

		for(int pass=count-1;pass>0;pass--)
		{
			for(int studentIndex=0;studentIndex<pass;studentIndex++)
			{
				if(calculateAverageScore(rankedStudents[studentIndex])<calculateAverageScore(rankedStudents[studentIndex+1]))
				{
					Student temp=rankedStudents[studentIndex];
					rankedStudents[studentIndex]=rankedStudents[studentIndex+1];
					rankedStudents[studentIndex+1]=temp;
				}
			}
		}

		for(int studentIndex=0;studentIndex<count;studentIndex++)
		{
			switch(studentIndex)
			{
			case 0: cout<<"第1名："; break;
			case 1: cout<<"第2名："; break;
			case 2: cout<<"第3名："; break;
			default: cout<<"第"<<studentIndex+1<<"名：";
			}
			cout<<rankedStudents[studentIndex].name
				<<"，成绩"<<calculateAverageScore(rankedStudents[studentIndex])<<endl;
		}
	}

	void runMenu(const Student students[],int count) const
	{
		char choice;
		while(true)
		{
			cout<<"功能菜单："<<endl;
			cout<<"A. 计算每位学生两课平均成绩"<<endl;
			cout<<"B. 查找学生"<<endl;
			cout<<"C. 按两课平均成绩从高到低排序"<<endl;
			cout<<"Q. 退出"<<endl;
			cout<<"请选择功能："<<flush;
			if(!(cin>>choice))
			{
				cerr<<"读取菜单选项失败。"<<endl;
				return;
			}

			switch(choice)
			{
			case 'A':
			case 'a':
				for(int studentIndex=0;studentIndex<count;studentIndex++)
				{
					cout<<"学号："<<students[studentIndex].studentId
						<<"，姓名："<<students[studentIndex].name
						<<"，两课平均成绩："<<calculateAverageScore(students[studentIndex])<<endl;
				}
				break;
			case 'B':
			case 'b':
				{
					bool shouldFind;
					if(!askYesNo("是否查找学生？（输入是/否）：",shouldFind))
					{
						return;
					}
					if(shouldFind&&!findStudent(students,count))
					{
						return;
					}
				}
				break;
			case 'C':
			case 'c':
				showRankings(students,count);
				break;
			case 'Q':
			case 'q':
				cout<<"已退出功能菜单。"<<endl;
				return;
			default:
				cout<<"无效选项，请选择 A、B、C 或 Q。"<<endl;
			}
		}
	}
};

ostream & operator <<(ostream &out, const Student &s)
{out<<s.studentId<<" "<<s.name<<" "<<s.gender<<" "<<s.age<<" "<<s.scoreA<<" "<<s.scoreB<<endl;       return out;}

istream & operator >>(istream &input, Student &s)
{input >> s.studentId >> s.name >> s.gender >> s.age >> s.scoreA >> s.scoreB;    return input;}

int read(Student *p,int capacity,const string &fileName)
{
	ifstream infile(fileName.c_str(),ios::in);
	if(!infile)
	{
		cerr<<"无法打开学生数据文件 Stu.txt。"<<endl;
		exit(1); 
	}
	int count=0;
	while(count<capacity)
	{
		infile>>ws;
		if(infile.eof())
		{
			break;
		}
		if(!(infile>>p[count]))
		{
			cerr<<"第"<<count+1<<"条学生记录格式错误，应包含：学号 姓名 性别 年龄 A课成绩 B课成绩。"<<endl;
			exit(1);
		}
		count++;
	}
	if(count==capacity)
	{
		infile>>ws;
		if(!infile.eof())
		{
			cerr<<"学生人数超过系统容量（"<<capacity<<"人）。"<<endl;
			exit(1);
		}
	}
	infile.close();
	return count;
}

void write(Student *p,int b,const string &fileName)
{
	ofstream outfile(fileName.c_str(),ios::out);
	if(!outfile)
	{
		cerr<<"无法打开学生数据文件进行写入。"<<endl;
		exit(1); 
	}
	for(int i=0;i<b;i++)
	{
		outfile<<p[i];
	}	
	outfile.close();
}

int main(int, char *argv[])
{
	Student stud[20];
	application app;
	const int capacity=sizeof(stud)/sizeof(stud[0]);
	int n;
	string executablePath(argv[0]);
	size_t pathSeparator=executablePath.find_last_of("\\/");
	string fileName="Stu.txt";
	if(pathSeparator!=string::npos)
	{
		fileName=executablePath.substr(0,pathSeparator+1)+"Stu.txt";
	}
	
	n=read(stud,capacity,fileName);
	cout<<"从文件读取的学生信息："<<endl<<endl;
	for(int i=0;i<n;i++)
	{
		stud[i].display();
	}

	bool shouldChange;
	if(!app.askYesNo("是否更改现有学生信息？（Y=是，N=否）：",shouldChange))
	{
		return 1;
	}

	bool dataChanged=false;
	if(shouldChange)
	{
		for(int i=0;i<n;i++)
		{
			cout<<"请输入第"<<i+1<<"名学生的信息（学号 姓名 性别 年龄 A课成绩 B课成绩）："<<flush;
			stud[i].add();
			if(!cin)
			{
				cerr<<"输入错误：每名学生需要输入六项，数据文件未修改。"<<endl;
				return 1;
			}
		}
		dataChanged=true;
	}

	bool shouldAdd;
	if(!app.askYesNo("是否录入新的学生信息？（Y=是，N=否）：",shouldAdd))
	{
		return 1;
	}
	if(shouldAdd)
	{
		if(!app.addStudents(stud,n,capacity))
		{
			return 1;
		}
		dataChanged=true;
	}

	bool shouldDelete;
	if(!app.askYesNo("是否删除学生信息？（Y=是，N=否）：",shouldDelete))
	{
		return 1;
	}
	if(shouldDelete)
	{
		bool deleted;
		if(!app.deleteStudent(stud,n,deleted))
		{
			return 1;
		}
		dataChanged=dataChanged||deleted;
	}

	if(dataChanged)
	{
		cout<<"更新后的学生信息："<<endl<<endl;
		for(int i=0;i<n;i++)
		{
			stud[i].display();
		}
		write(stud,n,fileName);
	}

	app.runMenu(stud,n);
	
	return 0;
} 
