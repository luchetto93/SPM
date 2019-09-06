#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<fstream>
#include<chrono>
#include<thread>
#include<mutex>
using namespace std;
#define N_DIMENSIONS 2
#define N_CENTROIDS 10
#define SPACE_SIZE 1000
#define N_THREADS 230

// GLOBAL VARIABLE INITIALIZATION
	//Point observations[SPACE_SIZE];	
	//Point centroids[N_CENTROIDS];
		
mutex mutex_global_centroid;		
class Point
{
	private:
	  double dimensions[N_DIMENSIONS];
	  int index_in_dimensions;
	public: 
	  Point()
	  {
	  	index_in_dimensions = 0;
	  }
	  double* get_dimensions()
	  {
	  	return dimensions;
	  }
	  int get_indexInDimensions()
	  {
	     return index_in_dimensions;
	  }
	  void set_dimensions(double v)
	  {
	   if(index_in_dimensions<N_DIMENSIONS)
	   {
	    	dimensions[index_in_dimensions] = v;
	    	index_in_dimensions++;
	   }
	   else
	   {
	     cout << "ERROR: ";
	   }
	  }
	  void set_point(double v,int index)
	  {
	     dimensions[index] = v;
	  }
	  double euclidean_distance(Point p)
	  {
	    int i = 0;
	    double euclidean_distance = 0;
	    double sum = 0;
	    double appo = 0;
	    for(i=0;i<N_DIMENSIONS;i++)
	    { 
	    		sum = sum + (dimensions[i] - p.dimensions[i])*(dimensions[i] - p.dimensions[i]);
	    }
	    return sum;
	  }
};

class CentroidHistory
{
    private:
    	double sumCoords[N_DIMENSIONS];
    	int count;
    
    public: 
      CentroidHistory()
      {
      	count = 0;
      }
      void clear_history()
      {
        int i = 0;
        count = 0;
        for (i=0;i<N_DIMENSIONS;++i)
        {
           sumCoords[i] = 0;
        }
      }
      
    	void set_SumCoords(Point p)
    	{
    			int i  = 0;
    			count++;
    			for(i=0;i<N_DIMENSIONS;i++)
    			{
    			   sumCoords[i] = sumCoords[i] + p.get_dimensions()[i];
    			   
    			}
    	}
    	double getCount()
    	{
    	   return count;
    	}
    	double * getSumCoords()
    	{
    	  return sumCoords;
    	}
    	
};
void print_array(double *a,int dim)
{
   int i = 0;
   for(i=0;i<dim;i++)
   {
      cout << a[i];
   }
}
int index_min_centroid(Point p, Point *centroids)
{
    int i = 0;
    double sum = 0;
    int closest_index = 0;
    double min_distance = 1e32;
    for(i=0;i<N_CENTROIDS;i++)
    {
    	 sum = centroids[i].euclidean_distance(p);
    	 if(sum < min_distance)
    	 {
    	    min_distance = sum;
    	    closest_index = i;
    	 }
    }
    return closest_index;

}
void update_new_centroid(Point *centroids, CentroidHistory *history)
{
	cout << "COMPUTO";
	int i = 0;
	int j = 0;
	double average = 0;
	for(i=0;i<N_CENTROIDS;i++)
	{
		average = 0;
	  	for(j=0;j<N_DIMENSIONS;j++)
	  	{
	  	     if(history[i].getCount()>0)
	  	     {
	  	         average = history[i].getSumCoords()[j] /history[i].getCount();
	  	     }
	  	     else
	  	     {
	  	    	 average = 0;
	  	     }
	  	     centroids[i].set_point(average,j);
	  	    
	  	}
	}
	for (i=0;i<N_CENTROIDS;++i)
	{
		history[i].clear_history();
	}
	
}
void sequential_Kmeans_computation(Point *observations,Point *centroids,CentroidHistory *history,int K_ite)
{
		int i = 0;
		int j = 0;
		int count_ite = 0;
		double min_distance = 0;
		double distance = 0;
		int index_min = 0;
		int k = 0;
	while(count_ite < 10)
	{	
		for(i=0;i<SPACE_SIZE;i++)
		{
				min_distance = 0;
				index_min = index_min_centroid(observations[i],centroids);
				history[index_min].set_SumCoords(observations[i]);		
		}

		update_new_centroid(centroids,history);
		count_ite++;
	}	
}
Point * generate_random_centroid2(Point observations[SPACE_SIZE],Point centroids[N_CENTROIDS])
{
	int generated = 0;
	int *selected_idx = (int*)malloc(sizeof(int)*N_CENTROIDS);
	int i = 0;
	int stop = 0;
	while(generated < N_CENTROIDS)
	{
		int j = rand() % SPACE_SIZE;
		while(i<N_CENTROIDS)
		{
		  if(selected_idx[i]==j)
		  {
		  	stop = 1;
		  }
		}
		if(!stop)
		{generated++;}
	}
	i = 0;
	while(i < N_CENTROIDS)
	{
		centroids[i] = observations[selected_idx[i]];
	}
}
void generate_random_centroid(Point observations[SPACE_SIZE],Point centroids[N_CENTROIDS])
{
	int v[SPACE_SIZE];
	int i = 0;
	for (i=0;i<SPACE_SIZE;++i)
	{
		v[i] = i;
	}
	for(i=SPACE_SIZE-1;i>0;--i)
	{
	  	int j = rand()%i;
	  	int temp = v[i];
	  	v[i] = v[j];
	  	v[j] = temp;
	}
	for (i = 0;i<N_CENTROIDS;++i)
	{
		centroids[i] = observations[v[i]];
	}
}
void print_centroid(Point *centroids)
{
	 int k = 0;
	 int i = 0;
	 for (i=0;i<N_CENTROIDS;i++)
  	 {
   	 	double *v = centroids[i].get_dimensions();
   	 	for(k=0;k<N_DIMENSIONS;k++)
   	 	{
   	 	   cout << v[k];
   	 	   cout << "|";
   	 	}
  	 }
}
Point cent [N_CENTROIDS];
int global_counter = 0;
void call_from_thread(int tid,int st_p,int end_p,Point *observations,int count_ite) {
	int min_distance = 0;
	int index_min = 0;
	int i = 0;
	CentroidHistory history [N_CENTROIDS];
	Point partial_cent[N_CENTROIDS];
    	while(count_ite < 10)
	{	
		for(i=0;i<SPACE_SIZE;i++)
		{
				min_distance = 0;
				index_min = index_min_centroid(observations[i],cent);
				history[index_min].set_SumCoords(observations[i]);		
		}

		update_new_centroid(partial_cent,history);
		count_ite++;
		/// putting a barrier
		mutex_global_centroid.lock()
		for(i=0;i<N_CENTROIDS;++i)
		{
			cent[i] = partial_
		}
	}
     }

int main()
{
	Point p1;
	Point observations[SPACE_SIZE];
	Point centroids[N_CENTROIDS];
	CentroidHistory history_centroid[N_CENTROIDS]; // each thread will have a CentroidHistory, where it stores the local computation. centroids stored [0,1,2,.....,N]
	string line;
	float v_1;
      	float v_2;
  	ifstream myfile ("plotexample.data");
  	int i = 0;
  	if (myfile.is_open())
  	{
  		
    		while ( myfile >> v_1 >> v_2)
    		{
    			Point new_p;
    			new_p.set_dimensions(SPACE_SIZE);
    			new_p.set_point(v_1,0);
    			new_p.set_point(v_2,1);
    			observations[i] = new_p;
    			i = i + 1;
    		}
    		myfile.close();
  	}

  else cout << "Unable to open file";
  double *v; 
  v = observations[990].get_dimensions();
  //cout << v[0] << v[1];
  generate_random_centroid(observations,centroids);
  double *v1 = centroids[1].get_dimensions();
  cout << v1[0] << "-" << v1[1];
  //sequential_Kmeans_computation(observations,centroids,history_centroid,10);
  int d = SPACE_SIZE / N_THREADS;
  int rest = SPACE_SIZE % N_THREADS;
  int st_p = 0;
  int end_p = 0;
  for (int i = 0; i < N_THREADS; ++i) {
            
            if(i<rest){
              if(i==0){
              st_p=0;
              }
              else
              {
               st_p = end_p+1;
               }
               end_p = st_p + d; 
               cout << "st_p: " << st_p << "end_p: " << end_p;
               cout << "\n";
             }
             else{
             	st_p = end_p+1;
             	end_p = st_p + d-1; 
             	cout << "st_p: " << st_p << "end_p: " << end_p;
                cout << "\n";
             }
              
           
            // t[i] = std::thread(call_from_thread, st_p,end_p);
             //std:: cout << value;
         }
}

