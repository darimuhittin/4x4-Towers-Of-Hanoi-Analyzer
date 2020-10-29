#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
using namespace std;

class Node
{
public:
	Node(string d);
	~Node();
	void ebeveynEkle(Node *ebeveyn);
	void cocukEkle(Node *cocuk);
	vector<Node *> ebeveynler;
	vector<Node *> cocuklar;
	string durum;
	vector<stack<int>> cubuklar;
	ofstream ofs;

	void cocuklariAyarla(Node *node, vector<string> &ziyaretEdilmis);
	void cocukDokumuAl(Node *x, vector<string> &ziyaretEdilenler);
};
Node::Node(string d)
{
	durum = d;
	for (int i = 0; i < 4; i++)
	{
		cubuklar.push_back(stack<int>());
	}

	const char *ind = durum.c_str();
	for (int i = 0; i < 4; i++)
	{
		int cubukNo = *ind - '1';
		cubuklar.at(cubukNo).push(4 - i);
		ind++;
	}
}

Node::~Node()
{
}

void Node::ebeveynEkle(Node *ebeveyn)
{
	ebeveynler.push_back(ebeveyn);
}

void Node::cocukEkle(Node *cocuk)
{
	cocuklar.push_back(cocuk);
}

void Node::cocuklariAyarla(Node *node, vector<string> &ziyaretEdilmis)
{
	cout << node->durum << " icin cocukAyarla." << endl;
	ziyaretEdilmis.push_back(node->durum);

	for (int iKaynak = 0; iKaynak < 4; iKaynak++)
	{
		if (node->cubuklar.at(iKaynak).empty())
		{
			continue;
		}

		for (int iHedef = 0; iHedef < 4; iHedef++)
		{
			bool hedefBos = false;

			if (iKaynak == iHedef)
			{
				continue;
			}

			if (!node->cubuklar.at(iHedef).empty())
			{
				if (node->cubuklar.at(iHedef).size() >= 4)
				{
					return;
				}
			}
			else
			{
				hedefBos = true;
			}

			int kaynak = node->cubuklar.at(iKaynak).top();
			int hedef = hedefBos ? 5 : node->cubuklar.at(iHedef).top();
			; //bos ise

			if (kaynak < hedef)
			{
				string yeniDurum;
				yeniDurum = node->durum;
				yeniDurum.at(4 - kaynak) = iHedef + '1';
				Node *yeniNode = new Node(yeniDurum);
				yeniNode->ebeveynEkle(node);
				node->cocukEkle(yeniNode);
			}
		}
	}

	for (Node *cocuk : node->cocuklar)
	{
		if (find(ziyaretEdilmis.begin(), ziyaretEdilmis.end(), cocuk->durum) == ziyaretEdilmis.end())
		{
			cocuklariAyarla(cocuk, ziyaretEdilmis);
		}
	}
}

void Node::cocukDokumuAl(Node *x, vector<string> &ziyaretEdilenler)
{
	ziyaretEdilenler.push_back(x->durum);
	for (Node *d : x->cocuklar)
	{
		cout << x->durum << "'in cocugu : " << d->durum << std::endl;
		ofs << x->durum << "," << d->durum << endl;
		//Bulunamamis ise
		if (std::find(ziyaretEdilenler.begin(), ziyaretEdilenler.end(), d->durum) == ziyaretEdilenler.end())
		{
			cocukDokumuAl(d, ziyaretEdilenler);
		}
	}
}

int main(int argc, char const *argv[])
{
	Node *a = new Node("1111");
	vector<string> z1;

	a->cocuklariAyarla(a, z1);

	cout << endl;

	vector<string> z2;

	a->ofs.open("data.csv");
	a->cocukDokumuAl(a, z2);
	a->ofs.close();

	for (string var : z2)
	{
		cout << var << "-";
	}

	cout << endl
		 << endl;
	cout << "Toplam node sayisi :" << z2.size();
	cout << endl
		 << endl;

	delete a;
	return 0;
}
