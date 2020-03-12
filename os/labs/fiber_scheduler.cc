#include "labs/fiber_scheduler.h"

//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//
void g_s(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int* arg){
    addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
    addr_t& f_stack    = *pf_stack;
    int& ret           = *pret;
    bool& done         = *pdone;
    int &n             = *arg;
    int i,j,sum,f_ret;

    for(j=2;j<=n;j++){
        sum=1;
        for(i=2;i<=j/2;i++){
            if(j%i==0){
              if(j%i==0){
                sum=0;
              }  
            }
        }
        if(sum==1){
           f_ret=j;
           // hoh_debug("Prime Number");
           // hoh_debug(f_ret);
        }  
        if(j%100==0){
            done=false;
            stack_saverestore(f_stack,main_stack);
        }
    }
    for(;;){
        done=true;
        ret=f_ret; 
        stack_saverestore(f_stack,main_stack);
    }    
}
void f_s(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int* arg){
    addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
    addr_t& f_stack    = *pf_stack;
    int& ret           = *pret;
    bool& done         = *pdone;
    int &n             = *arg;
	int i,j,sum,f_ret;
	hoh_debug("going to perfect");
    for(j=0;j<n;j++){
        sum=1;
        for(i=2;i*i<=j;i++){
            if(j%i==0) {
            	if(i*i!=j)
                	sum=sum+i+j/i;
              	else
                	sum=sum+i;
            }
        }
        if(sum==j&&j!=1){
           f_ret=j;
           hoh_debug("Perfect Number");
           hoh_debug(f_ret);
        }  
        if(j%100==0){
            done=false;
            //hoh_debug("::");
            stack_saverestore(f_stack,main_stack);
        }
    }
    for(;;){
        done=true;
        ret=f_ret; 
        hoh_debug("Printing done");
       	stack_saverestore(f_stack,main_stack);
    } 
}
void shell_step_fiber_scheduler(shellstate_t& shellstate, addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size){
	
	if(shellstate.mode==1){
        shellstate.flag_first=(0);
      shellstate.flag_second=(0);
      shellstate.index = (0);
      for (int i = 0; i < 5; ++i){
        if(shellstate.f_done_s[i]==false){
          if(shellstate.schedule[i]==25){
            shellstate.flag_first++;
          }
          else if(shellstate.schedule[i]==26) shellstate.flag_second++;
          shellstate.index++;
        }
      }
		for(shellstate.temp_index=0;shellstate.temp_index<5;shellstate.temp_index++){
			if(shellstate.f_done_s[shellstate.temp_index]==false){
				// hoh_debug("Fiber inp : " <<shellstate.temp_index<<"for "<< shellstate.arg2_1_s[shellstate.temp_index]);
                // hoh_debug("Fiber inp : " <<shellstate.temp_index<<"for "<< shellstate.arg_1_s[shellstate.temp_index]);
				if(shellstate.statef_s[shellstate.temp_index]==1){
                    stack_saverestore(stackptrs[2*shellstate.temp_index],stackptrs[2*shellstate.temp_index+1]);
                }   
                if(shellstate.schedule[shellstate.temp_index]==25){
					        // hoh_debug("in fs"<<shellstate.index);
					shellstate.index++;
        			shellstate.flag_first++;
		      		if(shellstate.statef_s[shellstate.temp_index]==0){
		      			shellstate.f_done_s[shellstate.temp_index]=false;
						stack_init5(stackptrs[2*shellstate.temp_index+1],arrays[shellstate.temp_index*stackptrs_size/arrays_size],arrays_size/10, &f_s, &stackptrs[2*shellstate.temp_index], &stackptrs[2*shellstate.temp_index+1], &shellstate.f_ret_s[shellstate.temp_index],&shellstate.f_done_s[shellstate.temp_index],&shellstate.arg2_1_s[shellstate.temp_index]);
		      			shellstate.statef_s[shellstate.temp_index]=1;
		      		}		        
	    		}
	    		else if(shellstate.schedule[shellstate.temp_index]==26){
	    			shellstate.index++;
        			shellstate.flag_second++;
		      		if(shellstate.statef_s[shellstate.temp_index]==0){
		      			shellstate.f_done_s[shellstate.temp_index]=false;
						stack_init5(stackptrs[2*shellstate.temp_index+1],arrays[shellstate.temp_index*stackptrs_size/arrays_size],arrays_size/10, &g_s, &stackptrs[2*shellstate.temp_index], &stackptrs[2*shellstate.temp_index+1], &shellstate.f_ret_s[shellstate.temp_index],&shellstate.f_done_s[shellstate.temp_index],&shellstate.arg2_1_s[shellstate.temp_index]);
		      			shellstate.statef_s[shellstate.temp_index]=1;
		      		}
	    		}
			}
			if(shellstate.f_done_s[shellstate.temp_index]==true&&shellstate.statef_s[shellstate.temp_index]==1){
			        	shellstate.statef_s[shellstate.temp_index]=2;
			        	shellstate.displayed[shellstate.temp_index]=false;
			        	hoh_debug(shellstate.f_ret_s[shellstate.temp_index]);
			        	hoh_debug("Fiber Completed");
			}
	    }
	}	
}

			//else
			//{
			  //shellstate.temp_index++;
			  //if(shellstate.temp_index>=shellstate.index)
			  //{
			  //	shellstate.temp_index=0;
			  //}
			// if(shellstate.index>0 &&shellstate.displayed[shellstate.tmp_index])
			// {
			// 	if(shellstate.schedule[shellstate.temp_index]=25)
			// 	  	shellstate.flag_first--;
			// 	  else if(shellstate.schedule[shellstate.temp_index]=26)
			// 	  	shellstate.flag_second--;
			// 	  for(int k=shellstate.temp_index;k<shellstate.index-1;k++)
			// 	  {
			// 	  	shellstate.f_done_s[k]=shellstate.f_done_s[k+1];
			// 	  	shellstate.f_ret_s[k]=shellstate.f_ret_s[k+1];
			// 	  	shellstate.statef_s[k]=shellstate.statef_s[k+1];
			// 	  	shellstate.schedule[k]=shellstate.schedule[k+1];
			// 	  }
			// 	  shellstate.f_done_s[shellstate.index-1]=true;
			// 	  shellstate.f_ret_s[shellstate.index-1]=0;
			// 	  shellstate.statef_s[shellstate.index-1]=0;
			// 	  shellstate.schedule[shellstate.index-1]=0;
			// 	  shellstate.index--;
			// 	  shellstate.flag_index--;
			// }	
