#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;
#define N_DIMENSIONS 3
#define N_CENTROIDS 10
#define SPACE_SIZE 500000


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
      void clearCentroidHistory()
      {
         count = 0;
         int i = 0;
         for(i = 0; i< N_CENTROIDS;i++)
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
	int i = 0;
	int j = 0;
	double average = 0;
	for(i=0;i<N_CENTROIDS;i++)
	{
	  	for(j=0;j<N_DIMENSIONS;j++)
	  	{
	  	     average = history[i].getSumCoords()[j] /history[i].getCount();
	  	     centroids[i].set_point(average,j);
	  	}
	}
	
}
void sequential_Kmeans_computation(Point *observations,Point *centroids,CentroidHistory *history)
{
		int i = 0;
		int j = 0;
		double min_distance = 0;
		double distance = 0;
		int index_min = 0;
		for(i=0;i<SPACE_SIZE;i++)
		{
				min_distance = 0;
				for(j=0;j<N_CENTROIDS;j++)
				{
				  index_min = index_min_centroid(observations[i],centroids);
				  history[index_min].set_SumCoords(observations[i]);
				}
		}
		
}
int main()
{
	Point p1;
	Point observations[SPACE_SIZE];
	Point centroids[N_CENTROIDS];
	CentroidHistory history_centroid[N_CENTROIDS];
	return 0;
}
