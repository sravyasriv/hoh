#include "labs/fiber.h"
void g(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int* arg){
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
           hoh_debug("Prime Number");
           hoh_debug(f_ret);
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
void f(addr_t* pmain_stack, addr_t* pf_stack, int* pret, bool* pdone,int* arg){
    addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
    addr_t& f_stack    = *pf_stack;
    int& ret           = *pret;
    bool& done         = *pdone;
    int &n             = *arg;
  int i,j,sum,f_ret;
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
          stack_saverestore(f_stack,main_stack);
      }
  }
  for(;;){
      done=true;
      ret=f_ret; 
      stack_saverestore(f_stack,main_stack);
  }  
}

void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){

  //insert your code here
  if(shellstate.functioncode==20){
      if(shellstate.statef==2){
          shellstate.statef=1;
          shellstate.f_done =false;
          stack_init5(f_stack, f_array, f_arraysize, &f, &main_stack, &f_stack, &shellstate.f_ret,&shellstate.f_done,&shellstate.arg2_1);
      }
      else if(shellstate.statef==0){
          stack_init5(f_stack, f_array, f_arraysize, &f, &main_stack, &f_stack, &shellstate.f_ret, &shellstate.f_done,&shellstate.arg2_1);
          shellstate.statef=1;
      } 
  }
  if(shellstate.statef==1){
      stack_saverestore(main_stack,f_stack);
  }
  if(shellstate.f_done==true&&shellstate.statef==1) {
      shellstate.statef=2;
      hoh_debug(shellstate.f_ret);
      hoh_debug("Fiber Completed");
  }
}

