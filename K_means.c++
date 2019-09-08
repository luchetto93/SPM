#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<fstream>
#include<chrono>
#include<unistd.h>
#include<thread>
#include<mutex>
#include<time.h>
#include<vector>
#include<condition_variable>
#define N_DIMENSIONS 2
#define N_CENTROIDS 10
#define SPACE_SIZE 300000
#define N_THREADS 10
#define X 2.0
using namespace std;
// GLOBAL VARIABLE INITIALIZATION
	//Point observations[SPACE_SIZE];	
	//Point centroids[N_CENTROIDS];
		
mutex mutex_global_centroid;
mutex mutex_global_centroid_2;	
condition_variable cv;
condition_variable cv_2;
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
	 
	  void sum_coordinates(double *v, int dim)
	  {
	  	int i = 0;
	  	if(N_DIMENSIONS == dim)
	  	{
	  	  for(i=0;i<dim;++i)
	  	  {
	  	  	dimensions[i] = dimensions[i] + v[i];
	  	  }
	  	
	  	}
	  }
	  double euclidean_distance(Point p)
	  {
	    int i = 0;

	    double sum = 0;

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
    	int involved = 0;
    public: 
      CentroidHistory()
      {
      	count = 0;
      	involved = 0;
      }
      int get_involved()
      {
       return involved;
      }
      void clear_history()
      {
        int i = 0;
        count = 0;
        involved = 0;
        for (i=0;i<N_DIMENSIONS;++i)
        {
           sumCoords[i] = 0;
        }
      }
      
    	void set_SumCoords(Point p)
    	{
    			int i  = 0;
    			count++;
    			involved = 1;
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
    	void increment_centroid_history(CentroidHistory c_p)
    	{
    		int i = 0;
    		for(i=0;i<N_DIMENSIONS;i++)
    		{
    			sumCoords[i] = sumCoords[i] + c_p.getSumCoords()[i];
    		}
    		count = count + c_p.getCount();
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
    //cout << "Cvalue by =::" << closest_index;
    return closest_index;

}
void update_new_centroid(Point *centroids, CentroidHistory *history)
{
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
		 		//cout << "min:" << index_min;
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
Point cent [N_CENTROIDS];
Point centroid_global_2[N_CENTROIDS];
CentroidHistory global_history[N_CENTROIDS];
int global_counter = 0;
int global_counter_2 = 0;
void generate_random_centroid(Point observations[SPACE_SIZE],int *indexes,Point *cen)
{
	//int v[SPACE_SIZE];
	//int i = 0;
	//srand(time(NULL));
	//for (i=0;i<SPACE_SIZE;++i)
	//{
	//	v[i] = i;
	//}
	//for(i=SPACE_SIZE-1;i>0;--i)
	//{
	  //	int j = rand()%i;
	  //	int temp = v[i];
	  //	v[i] = v[j];
	  //	v[j] = temp;
	//}
	int i = 0;
	for (i = 0;i<N_CENTROIDS;++i)
	{
		
		//cen[i] = observations[indexes[i]];
		centroid_global_2[i] = observations[indexes[i]];
		cent[i] = observations[indexes[i]];
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


void clear_history(CentroidHistory *hist)
{
	int i = 0;
  	for (i=0;i<N_CENTROIDS;++i)
 	{ 
 	 	hist[i].clear_history();
 	}
}
CentroidHistory global_history_contiguos[N_CENTROIDS*N_THREADS];

void call_from_thread_global_history(int tid,int st_p,int end_p,Point * observations,int K_Ite)
{
	int i = 0;
	int j = 0;
	unique_lock<mutex> lck (mutex_global_centroid_2,defer_lock);
	int partial_count = 0;
	double partial_sum [N_DIMENSIONS];
	int incr = 0;
	int count_ite = 0;
	int k = 0;
	int min_distance = 0;
	int index_min = 0;

	while(count_ite < 10)
	{	
	   	 
	   	 
		for(i=st_p;i<=end_p;++i)
		{
		  min_distance = 0;
		  index_min = index_min_centroid(observations[i],centroid_global_2);
		 
		  global_history_contiguos[(tid*N_CENTROIDS)+index_min].set_SumCoords(observations[i]);		
		}
		lck.lock();
		
		global_counter_2++;
		if(global_counter_2 < N_THREADS)
		{  		  		 
		  	cv_2.wait(lck);
		  	lck.unlock();
		} 
		else
		{ 
	 	    
		     for(j=0;j<N_CENTROIDS;j++){
		  	for(i=0;i<N_THREADS;++i)
		   	{
		   	    partial_count = 0;
		   	    for(k=0;k<N_DIMENSIONS;++k)
		   	    {
			    	partial_sum[k] =  partial_sum[k] + global_history_contiguos[(i*N_CENTROIDS)+j].getSumCoords()[k];
			    	}	
			     
			    }
			    partial_count = partial_count + global_history_contiguos[(i*N_CENTROIDS)+j].getCount();
			    
			}
		      for(k=0;k<N_DIMENSIONS;++k)
			{
			    if(partial_count >0){
				partial_sum[k] = partial_sum[k] / partial_count;
				centroid_global_2[j].set_point(partial_sum[k],k);
				partial_sum[k] = 0;
				}
			}
		  	
		  

		  global_counter_2 = 0;
		  //cout << "wake-up!!!!" << "\n";
		  for (i=0;i<N_THREADS*N_CENTROIDS;++i)
 		  { 
 	 		global_history_contiguos[i].clear_history();
 		  }
 		  cv_2.notify_all();
		  lck.unlock();
		}
		
	count_ite++;	
	}
	return;
}

void call_from_thread(int tid,int st_p,int end_p,Point *observations,int K_Ite) {
	int min_distance = 0;
	int count_ite = 0;
	int index_min = 0;
	int i = 0;
	CentroidHistory history [N_CENTROIDS];
	Point partial_cent[N_CENTROIDS];
	unique_lock<mutex> lck (mutex_global_centroid,defer_lock);
	//clear_history(history);  LASCIALA FORSE UN GIORNO LA CAPIRÒ !!!!!
    	while(count_ite < 10)
	{	
		for(i=st_p;i<=end_p;i++)
		{
				min_distance = 0;
				index_min = index_min_centroid(observations[i],cent);
				history[index_min].set_SumCoords(observations[i]);		
		}
		lck.lock();
		global_counter++;
		for(i=0;i<N_CENTROIDS;i++)
		{
				global_history[i].increment_centroid_history(history[i]);
				history[i].clear_history();
		}
		if(global_counter == N_THREADS)
		{
		  
		  update_new_centroid(cent,global_history);
		  global_counter = 0;
		  cv.notify_all();
		  lck.unlock();
		}
		else
		{	
			cv.wait(lck);
			lck.unlock();
		}
	count_ite++;
	}

	return;
     }
void create_copy_centroid(Point *centroid)
{
   int i = 0;
   int k = 0;
   for(i=0;i<N_CENTROIDS;++i)
   {
   	for(k=0;k<N_DIMENSIONS;++k)
   	{
   		centroid[i].set_point(cent[i].get_dimensions()[k],k);
   	}
   }
}
void read_from_file(Point *observations)
{
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
}
static const int num_threads = 4;
int value = 4;
void generate_random_observations(Point *observations)
{
	double x = 0;
	double y = 0;
	int i = 0;
     	for(i=0;i<SPACE_SIZE;i++)
     	{
     	  x = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/X));
     	  y = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/X));
     	  Point new_p;
     	  new_p.set_dimensions(N_DIMENSIONS);
     	  new_p.set_point(x,0);
     	  new_p.set_point(y,1);
     	  observations[i] = new_p;
     	}
     	
}
void copy_in_centroid_global_2()
{
     int i = 0;
   int k = 0;
   for(i=0;i<N_CENTROIDS;++i)
   {
   	for(k=0;k<N_DIMENSIONS;++k)
   	{
   		centroid_global_2[i].set_point(cent[i].get_dimensions()[k],k);
   	}
   }
}
int main()
{	int i = 0;
	int centroid_idx[N_CENTROIDS] = {100,120,5000,8000,12000,11235,19234,17654,23456,25000};
	Point p1;
	CentroidHistory history_centroid[N_CENTROIDS];
	int k = 0;
	Point observations[SPACE_SIZE];
	Point centroid_seq[N_CENTROIDS];
	generate_random_observations(observations);
  	generate_random_centroid(observations,centroid_idx,centroid_seq);
	create_copy_centroid(centroid_seq);
  	//copy_in_centroid_global_2();

  	auto start = chrono::high_resolution_clock::now();
  	sequential_Kmeans_computation(observations,centroid_seq,history_centroid,10);
  	
 
  	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	cout << "Elapsed time: " << elapsed.count() << " s\n";

 
  int d = SPACE_SIZE / N_THREADS;
  int rest = SPACE_SIZE % N_THREADS;
  int st_p = 0;
  int end_p = 0;
  vector<thread> threads_worker(N_THREADS);
  clear_history(global_history);
  auto start1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < N_THREADS; ++i) {
            
            if(rest==0){
            	if(i==0){
            		st_p = 0;
            	}
            	else{
            		st_p = end_p+1;
            		
            	}
            	end_p = st_p +d-1; 
            }else{
            
            if(i<rest){
              if(i==0){
              st_p=0;
              }
              else
              {
               st_p = end_p+1;
               }
               end_p = st_p + d; 

             }
             else{
             	st_p = end_p+1;
             	end_p = st_p + d-1; 

             }
              }
      
  
         threads_worker[i] = thread(call_from_thread_global_history,i,st_p,end_p,observations,10);
         }
          for (auto& th : threads_worker) {
       th.join();
       }
 	
     auto finish1 = chrono::high_resolution_clock::now();
     chrono::duration<double> elapsed2 = finish1 - start1;
     cout << "Elapsed time second: " << elapsed2.count() << " s\n";
     cout << "parallel computation GLOBAL solution 2:";
     
       
       
     
  	         	        cout << "sequential computation:";
  for(i=0;i<N_CENTROIDS;++i)
    {
       for(k=0;k<N_DIMENSIONS;++k)
       {
       		cout << centroid_seq[i].get_dimensions()[k] << "|";
       }
       cout << "\n";
    }
  
         	        cout << "parallel_comput computation:";
  for(i=0;i<N_CENTROIDS;++i)
    {
       for(k=0;k<N_DIMENSIONS;++k)
       {
       		cout << centroid_global_2[i].get_dimensions()[k] << "|";
       }
       cout << "\n";
    }
     for (int i = 0; i < N_THREADS; ++i) {
            
            if(rest==0){
            	if(i==0){
            		st_p = 0;
            	}
            	else{
            		st_p = end_p+1;
            		
            	}
            	end_p = st_p +d-1; 
            }else{
            
            if(i<rest){
              if(i==0){
              st_p=0;
              }
              else
              {
               st_p = end_p+1;
               }
               end_p = st_p + d; 

             }
             else{
             	st_p = end_p+1;
             	end_p = st_p + d-1; 

             }
              }
      
  
       //  threads_worker[i] = thread(call_from_thread,i,st_p,end_p,observations,10);
         }

  	
  	
       
         // for (auto& th : threads_worker) {
        //th.join();
        //}

        //auto finish1 = chrono::high_resolution_clock::now();
	//chrono::duration<double> elapsed2 = finish1 - start1;
	//cout << "Elapsed time: " << elapsed2.count() << " s\n";
    //    cout << "parallel computation GLOBAL solution 1:";
  //for(i=0;i<N_CENTROIDS;++i)
    //{
      // for(k=0;k<N_DIMENSIONS;++k)
       //{
       	//	cout << cent[i].get_dimensions()[k] << "|";
       //}
       //cout << "\n";
    //}
  
 
  
  
}

