#include<bits/stdc++.h>
#include <chrono>
# define N 4
# define MAX 9999
using namespace std;
using namespace std::chrono;
int dp[1000][1000];
vector<int>final_path(N+1);
int final_res = INT_MAX; 
bool isSubsetSum(vector<int>lands, int n, int sum)
{
    if (sum == 0)
        return true;
    if (n == 0)
        return false;
    if (lands[n - 1] > sum)
        return isSubsetSum(lands, n - 1, sum);
    return isSubsetSum(lands, n - 1, sum)
           || isSubsetSum(lands, n - 1, sum - lands[n - 1]);
}
int count_subsets_recursive(vector<int>lands,int n,int sum)
{
    if (sum == 0)
        return 1;
    else if (sum != 0 && n == 0)
        return 0;
    else if (lands[n - 1] > sum)
        return count_subsets_recursive(lands, n - 1, sum);
    else
        return count_subsets_recursive(lands, n - 1, sum) + count_subsets_recursive(lands, n - 1, sum - lands[n - 1]);
}
int count_subsets_DP(vector<int>lands,int n,int sum)
{
    if (sum == 0)
        return dp[n][sum] = 1;
    else if (sum != 0 and n == 0)
        return dp[n][sum] = 0;
    if (dp[n][sum] != -1)
        return dp[n][sum];
    else if (lands[n - 1] > sum)
        return dp[n][sum] = count_subsets_DP(lands,n - 1, sum);
    else
        return dp[n][sum] = count_subsets_DP(lands,n - 1, sum) + count_subsets_DP(lands, n - 1, sum - lands[n - 1]);
}
static int m=0;
void printAllLandGroups_recursion(vector<int>lands, int n, vector<int> v,int sum)
{
    if (sum == 0) 
	{
    	cout <<"Group of lands - "<<m+1<<": ";
    	m++;
        for (int x=0;x<v.size();x++)
        {
           cout<<"{"<<v[x]<<"}"<< " ";	
		}
        cout << endl;
        return;
    }
    if (n == 0)
    {
        return;
    }
    printAllLandGroups_recursion(lands, n - 1, v, sum);
    v.push_back(lands[n - 1]);
    printAllLandGroups_recursion(lands, n - 1, v, sum - lands[n - 1]);
}
void printAllSubsets(vector<int>lands, int n, int sum)
{
    vector<int> v;
    cout<<"All possible groups of lands required to avoid wastage of cotton [RECURSIVE APPROACH]:\n";
    printAllLandGroups_recursion(lands, n, v, sum);
}
class SubSet{
public:
  stack<int> solutionSet;
  bool hasSolution;
  int m=0;
  void solve(vector<int>& lands, int s, int idx, int sum, int n)
  {
      if ( s > sum )
      {
         return;
      }
    if(s==sum)
	{
        hasSolution = true;
        displaySolutionSet();
        return;
    }
    for ( int i = idx; i < n; i++ )
    {
        solutionSet.push (lands[i]);
        solve(lands,s+lands[i],i+1,sum,n);
        solutionSet.pop();
    }
  }
  void displaySolutionSet()
    {
        stack<int> temp;
        cout <<"Group of lands - "<<m+1<<": ";
    	m++;
        while (!solutionSet.empty()) 
        { 
            cout <<"{"<<solutionSet.top()<<"}"; 
            temp.push(solutionSet.top()); 
            solutionSet.pop();
        } 
        cout <<'\n';
        while (!temp.empty()) 
        { 
            solutionSet.push(temp.top()); 
            temp.pop();
        }
    }
};
void find_Min_cost_Route_GREEDY(vector<vector<int> > tsp)
{
	int sum = 0;
	int counter = 0;
	int j = 0, i = 0;
	int min = INT_MAX;
	map<int, int> visitedRouteList;
	visitedRouteList[0] = 1;
	int route[tsp.size()];
	while (i < tsp.size() && j < tsp[i].size())
	{
		if (counter >= tsp[i].size() - 1)
		{
			break;
		}
		if (j != i && (visitedRouteList[j] == 0))
		{
			if (tsp[i][j] < min)
			{
				min = tsp[i][j];
				route[counter] = j + 1;
			}
		}
		j++;
		if (j == tsp[i].size())
		{
			sum += min;
			min = INT_MAX;
			visitedRouteList[route[counter] - 1] = 1;
			j = 0;
			i = route[counter] - 1;
			counter++;
		}
	}
	i = route[counter - 1] - 1;

	for (j = 0; j < tsp.size(); j++)
	{

		if ((i != j) && tsp[i][j] < min)
		{
			min = tsp[i][j];
			route[counter] = j + 1;
		}
	}
	sum += min;
	cout << (sum);
}
class TSP_Branch_Bound
{
	public:
        bool visited[N];
     void copyToFinal(int curr_path[])
     { 
	    for (int i=0; i<N; i++)
		  final_path[i] = curr_path[i];
	      final_path[N] = curr_path[0];
     }
     int firstMin(int** cost_matrix, int i)
     {
    	int min = INT_MAX;
	    for (int k=0; k<N; k++)
     		if (cost_matrix[i][k]<min && i != k)
			min = cost_matrix[i][k];
      	return min;
     }
    int secondMin(int** cost_matrix, int i)
    {
	  int first = INT_MAX, second = INT_MAX;
	  for (int j=0; j<N; j++)
	  {
		 if (i == j)
		 	continue;

		 if (cost_matrix[i][j] <= first)
	     {
			second = first;
			first = cost_matrix[i][j];
		 }
		 else if (cost_matrix[i][j] <= second &&
				  cost_matrix[i][j] != first)
		    second = cost_matrix[i][j];
	  }
	return second;
}
void TSPRec(int** cost_matrix, int curr_bound, int curr_weight,
			int level, int curr_path[])
{
	if (level==N)
	{
		if (cost_matrix[curr_path[level-1]][curr_path[0]] != 0)
		{
			int curr_res = curr_weight +
					cost_matrix[curr_path[level-1]][curr_path[0]];
			if (curr_res < final_res)
			{
				copyToFinal(curr_path);
				final_res = curr_res;
			}
		}
		return;
	}
	for (int i=0; i<N; i++)
	{
		if (cost_matrix[curr_path[level-1]][i] != 0 &&
			visited[i] == false)
		{
			int temp = curr_bound;
			curr_weight += cost_matrix[curr_path[level-1]][i];

			if (level==1)
			curr_bound -= ((firstMin(cost_matrix, curr_path[level-1]) +
							firstMin(cost_matrix, i))/2);
			else
			curr_bound -= ((secondMin(cost_matrix, curr_path[level-1]) +
							firstMin(cost_matrix, i))/2);
	
			if (curr_bound + curr_weight < final_res)
			{
				curr_path[level] = i;
				visited[i] = true;
				TSPRec(cost_matrix, curr_bound, curr_weight, level+1,
					curr_path);
			}

			curr_weight -= cost_matrix[curr_path[level-1]][i];
			curr_bound = temp;
            for(int i=0;i<sizeof(visited);i++)
            {
                visited[i]=false;
            }
			for (int j=0; j<=level-1; j++)
            {
                visited[curr_path[j]] = true;
            }
		}
	}
}

void TSP(int** cost_matrix)
{
	int curr_path[N+1];
	int curr_bound = 0;
    for(int i=0;i<N+1;i++)
    {
        curr_path[i]=-1;
    }
    for(int i=0;i<N+1;i++)
    {
        visited[i]=0;
    }
	for (int i=0; i<N; i++)
		curr_bound += (firstMin(cost_matrix, i) +	
		secondMin(cost_matrix, i));

	curr_bound = (curr_bound&1)? curr_bound/2 + 1 :
								curr_bound/2;

	visited[0] = true;
	curr_path[0] = 0;
	TSPRec(cost_matrix, curr_bound, 0, 1, curr_path);
}	
};
int completed_visit = (1<<N) -1;
int TSP_dp[16][4];
int  TSP_DP(int** cost_m,int mark,int position)
{
  if(mark==completed_visit)
  { 
     return cost_m[position][0];
  }
  if(TSP_dp[mark][position]!=-1)
  {
     return TSP_dp[mark][position];
  }
  int res = MAX;
  for(int city=0;city<N;city++)
  {
    if((mark&(1<<city))==0)
    {
      int newAnswer = cost_m[position][city] + TSP_DP(cost_m,mark|(1<<city),city);
      res = min(res, newAnswer);
    }
  }
  TSP_dp[mark][position] = res;
  return TSP_dp[mark][position];
}
int main()
{
	int n,cotton,sum=0,customers;
	SubSet ss;
	bool bool_value;
	clock_t start, end;
	cout<<"Enter the number of lands included in Dhananjay's farm:\n";
	cin>>n;
	vector<int>lands(n);
	for(int i=0;i<n;i++)
	{
		cout<<"Enter the cotton growing capacity of land - "<<i+1<<endl;
		cin>>lands[i];
	}
	cout<<"Enter the number of customers:\n";
	cin>>customers;
	for(int j=0;j<customers;j++)
	{
		cout<<"Enter the amount of cotton demanded by customer - "<<j+1<<endl;
		cin>>cotton;
		sum=sum+cotton;
	}
    bool_value=isSubsetSum(lands, n, sum);
    if (bool_value== true)
    {
    	memset(dp, -1, sizeof(dp));
    	
        auto start = high_resolution_clock::now();
    	count_subsets_DP(lands, n, sum);
    	auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        
        auto start2 = high_resolution_clock::now();
        count_subsets_recursive(lands,n,sum);
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        
        int groups_dp = count_subsets_DP(lands, n, sum);
    	int groups_rec= count_subsets_recursive(lands,n,sum);
    	cout <<"Dhananjay can harvest the total demanded quantity of cotton within the farm.\n";
     	cout<<"All possible land groups: "<<groups_dp<<endl;
     	cout<<"Time taken by Recursive approach to count number of land groups: "<< duration.count()<<" microseconds"<<endl;;
    	//cout<<"All possible land groups [DYNAMIC PROGRAMMING ALGO. USED]: "<<groups_dp<<endl;
    	cout<<"Time taken by  Dynamic Programming approach to count number of land groups: "<< duration2.count()<<" microseconds"<<endl;
    	
    	auto start3 = high_resolution_clock::now();
        printAllSubsets(lands, n, sum);
        auto stop3 = high_resolution_clock::now();
        auto duration3 = duration_cast<microseconds>(stop3 - start3);
        
    	cout<<"All possible groups of lands required to avoid wastage of cotton [Backtracking Technique] :\n";
    	
        auto start4 = high_resolution_clock::now();
        ss.solve(lands,0,0,sum,n);
        auto stop4 = high_resolution_clock::now();
        auto duration4 = duration_cast<microseconds>(stop4 - start4);
        
        cout<<"Time taken by recursive algorithm to display land groups: "<<duration3.count()<<" microseconds"<<endl;
        cout<<"Time taken by Backtracking algorithm to display land groups: "<<duration4.count()<<" microseconds"<<endl;
        int num_edges;
    	cout<<"Enter the number of connections between cities:\n";
        cin>>num_edges;
        string city;
    	pair <string,int> m[N];
        cout<<"Enter the name of 4 cities where Dhananjay has to deliver the cotton:\n"; 
	    for(int i=0;i<N;i++)
	    {
	     	cin>>city;
	        m[i].first=city;
	        m[i].second=i;
     	}
        int** cost_matrix = new int*[N];
        for(int i=0;i<N;i++)
        {
            cost_matrix[i] = new int[N];
            for(int j=0;j<N;j++)
            {
                cost_matrix[i][j] = 0;
            }
        }
        string first_city,second_city;
        for(int i=0;i<num_edges;i++)
        {
            int first_node,second_node,weight;
            cout<<"Enter the pair of connected cities - "<<i+1<<": ";
            cin >> first_city >> second_city;
			cout<<endl<<"Enter the cost of travelling associated: ";
			cin>>weight;
			cout<<endl;
			for(int j=0;j<N;j++)
			{
				if(first_city==m[j].first)
				first_node=m[j].second;
			}
			for(int k=0;k<N;k++)
			{
				if(second_city==m[k].first)
				second_node=m[k].second;
			}
            cost_matrix[first_node][second_node] = weight;
            cost_matrix[second_node][first_node] = weight;
        }
        cout<<"Adjacency cost matrix comprising travelling costs of all the cities:\n";
        for(int i=0;i<N;i++)
        {
            for(int j=0;j<N;j++)
            {
                cout << cost_matrix[i][j] << " ";
            }
            cout <<endl;
        }
        cout << endl;
        TSP_Branch_Bound TSP_obj;
        auto start6 = high_resolution_clock::now();
        TSP_obj.TSP(cost_matrix);
        auto stop6 = high_resolution_clock::now();
        auto duration6 = duration_cast<nanoseconds>(stop6 - start6);
        
	    cout<<"Path Taken by Dhananjay to maximize his savings : ";
	    for (int i=0; i<=N; i++)
	    {
		  cout<<m[final_path[i]].first<<"-->";
	    }
        cout << endl;
        vector<vector<int> > tsp;
        for ( int i = 0;i < N;i++ )
        {
            for(int j=0;j<N;j++)
            {
               tsp[i][j] = cost_matrix[i][j];
            }
        }
	    cout<<"Minimum cost of transportation [TSP using Greedy Approach] :";
     	auto start5 = high_resolution_clock::now();
        find_Min_cost_Route_GREEDY(tsp);
        auto stop5 = high_resolution_clock::now();
        auto duration5 = duration_cast<nanoseconds>(stop5 - start5);
        for(int i=0;i<(1<<N);i++)
		{
          for(int j=0;j<N;j++)
		  {
            TSP_dp[i][j] = -1;
		  }
        }
        int** cost_m = new int*[N];
        for ( int i = 0;i < N;i++ )
        {
            for(int j=0;j<N;j++)
            {
              cost_m[i][j] = cost_matrix[i][j];
            }
        }
        auto start7 = high_resolution_clock::now();
	    int min=TSP_DP(cost_m,1,0);
	    auto stop7 = high_resolution_clock::now();
        auto duration7 = duration_cast<nanoseconds>(stop7 - start7);
        
        cout<<endl<<"Minimum cost of transportation [TSP using Dynamic Programming] :"<<min<<endl;
	    cout<<"Minimum cost of transportation [TSP Branch & Bound] :"<<final_res<<endl;
	    cout<<"Time taken by  [TSP Greedy Approach] to minimixe the cost of travelling: "<< duration5.count()<<" nanoseconds"<<endl;
	    cout<<"Time taken by  [TSP Branch and Bound Method] to minimixe the cost of travelling: "<< duration6.count()<<" nanoseconds"<<endl;
	    cout<<"Time taken by  [TSP Dynamic Programming Approach] to minimixe the cost of travelling: "<< duration7.count()<<" nanoseconds"<<endl;
	}
    else
        cout <<"It's not possible for Dhananjay to harvest the demanded quantity in his farm.\n";
    return 0;
}