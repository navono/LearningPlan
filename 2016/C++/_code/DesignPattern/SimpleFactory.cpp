#include<iostream>
#include<vector>
using namespace std;

class enum ProductType
{
	TypeA,
	TypeB,
	TypeC
};

class Product
{
public:
	virtual void show() = 0;
};

class ProductA : public Product
{
public:
	void show()
	{
		cout << "Type A" << endl;
	}
};

class ProductB : public Product
{
public:
	void show()
	{
		cout << "Type B" << endl;
	}
};


class Factory
{
	Product* CreateProduct(ProductType type)
	{
		switch (type)
		{
		case TypeA:
			return new ProductA();

		case TypeB:
			return new ProductB();

		default:
			return nullptr;
		}
	}
};

int main(int argc, char *argv[])
{
	// First, create a factory object
	Factory *ProductFactory = new Factory();
	Product *productObjA = ProductFactory->CreateProduct(TypeA);
	if (productObjA != nullptr)
		productObjA->Show();

	Product *productObjB = ProductFactory->CreateProduct(TypeB);
	if (productObjB != nullptr)
		productObjB->Show();

	delete ProductFactory;
	ProductFactory = nullptr;

	delete productObjA;
	productObjA = nullptr;

	delete productObjB;
	productObjB = nullptr;        

	delete productObjC;
	productObjC = nullptr;

	return 0;
}