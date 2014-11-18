#include<queue>
#include<iostream>
#include <iomanip>
using namespace std;

class compareColorRank {
  public:
    bool operator()(std::pair<int, int> &p1, std::pair<int, int>  &p2)
    {
      return p1.second < p2.second;
    }
};
typedef std::priority_queue <std::pair<int,int>, vector<std::pair<int,int> >, compareColorRank> pq_type;

int main()
{
  std::pair<int,int> p1(1, 20);
  std::pair<int,int> p2(2, 10);
  std::pair<int,int> p3(3, 40);

  pq_type priorityColors ; 

  priorityColors.push(p1); ;
  priorityColors.push(p2);
  priorityColors.push(p3);

  while (! priorityColors.empty()) {  
    std::pair<int,int> p =  priorityColors.top();
    std::cout << setw(3) << (p).first << " " << setw(3) << (p).second << " " << endl;
    priorityColors.pop();
  }

  return 0;
}


