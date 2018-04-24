#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>

class PageReplacement
{
public:
	PageReplacement();
	~PageReplacement();
	void run();
	void LRU();

private:
	void addInfo() const;

private:
	int pages;					// 虚拟内存的尺寸P
	int firstPageFramePos;		// 工作面的起始位置p
	int pageFrames;				// 工作面中包含的页数e
	int rateM;					// 工作面移动率m
	std::vector<int> seqVec;	// 序列号
	std::vector<int> mem;		// 内存块
};


PageReplacement::PageReplacement()
	: mem(3, -1)
{
	this->run();
}

PageReplacement::~PageReplacement()
{
}

void PageReplacement::run()
{
	std::cout << "请输入虚拟内存尺寸P：";
	std::cin >> pages;
	std::cout << "请输入工作面的起始位置p：";
	std::cin >> firstPageFramePos;
	std::cout << "请输入工作面中包含的页数e：";
	std::cin >> pageFrames;
	std::cout << "请输入工作面移动率m：";
	std::cin >> rateM;
	//std::cout << "请输入在0和1之间的值t：";
	//std::cin >> t;

	for (int i = 0; i < rateM; ++i)
	{
		int randomNum = (rand() % pageFrames) + firstPageFramePos;
		seqVec.push_back(randomNum);
	}

	std::cout << "序列号：";
	for (int i = 0; i < seqVec.size(); ++i)
	{
		std::cout << seqVec.at(i) << " ";
	}

	std::cout << std::endl;
}

void PageReplacement::LRU()
{
	int nLack = 0;							// 缺页数
	std::list<int> seqList(seqVec.begin(), seqVec.end());
	int nTotal = seqList.size();

	std::vector<int> timer(mem.size(), 0);		// 每个内存块对应的时钟

	while (!seqList.empty())
	{
		int head = *seqList.begin();			// 去队头的页面
		seqList.pop_front();
		bool equal = false;						// 标识内存中是否有与队头相等的页面
		int vacant = -1;						// 标识内存中是否有空闲页面

		for (int i = 0; i < mem.size(); ++i)
		{
			if (mem.at(i) == head)				// 如果找到相等的页面
			{
				equal = true;
				this->addInfo();				// 显示内存块

				timer.at(i) = -1;				// 相等的时钟清零，其他的时钟加1
				for (int j = 0; j < timer.size(); ++j)
				{
					++timer[j];
				}

				break;
			}

			else if (mem.at(i) == -1 && vacant == -1)		// 如果找到空闲位
			{
				vacant = i;
			}
		}

		if (equal)					// 如果找到相等的页面，则进行下一个查找
		{
			continue;
		}

		++nLack;

		if (vacant !=  -1)
		{
			mem[vacant] = head;			// 把队头的放入到空闲位中
			this->addInfo();

			timer[vacant] = -1;			// 空闲位时钟清零，其他时钟加1
			for (int j = 0; j < timer.size(); ++j)
			{
				++timer[j];
			}

			continue;
		}

		else
		{
			int max = timer[0];
			int subIndex = 0;
			for (int p = 0; p < timer.size(); ++p)
			{
				if (timer.at(p) > max)
				{
					max = timer.at(p);		// 找到最近最久没有被使用的内存块
					subIndex = p;
				}
			}

			mem[subIndex] = head;			// 把队头的放入到最近最久未被使用的内存块中
			this->addInfo();

			timer[subIndex] = -1;
			for (int j = 0; j < timer.size(); ++j)
			{
				++timer[j];
			}

		}

	}

	std::cout << "缺页率： " << (double)nLack/nTotal * 100 << "%" << std::endl;
}

void PageReplacement::addInfo() const
{ 
	std::cout << mem.at(0) << "    " <<  mem.at(1) << "    "  << mem.at(2) << std::endl;
}


int main()
{
	PageReplacement pageReplacement;

	pageReplacement.LRU();

	return 0;
}
