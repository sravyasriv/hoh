#include "labs/coroutine.h"

void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){
  if(shellstate.functioncode==18){
    if(shellstate.states==2){
      shellstate.states=1;
      coroutine_reset(f_coro);
    }
    else if(shellstate.states==0)
      shellstate.states=1;
      shellstate.f_done =false;
      f_locals.N = shellstate.arg2_1;
  }
  if(shellstate.states==1){
    h_begin(f_coro);
    for(f_locals.j=0;f_locals.j<f_locals.N;f_locals.j++){
        f_locals.sum=1;
        for(f_locals.i=2;f_locals.i*f_locals.i<=f_locals.j;f_locals.i++){
            if(f_locals.j%f_locals.i==0){
              if(f_locals.i*f_locals.i!=f_locals.j)
                f_locals.sum=f_locals.sum+f_locals.i+f_locals.j/f_locals.i;
              else
                f_locals.sum=f_locals.sum+f_locals.i;
            }
            if(f_locals.j%100==0){
              shellstate.f_done=false;
              h_yield(f_coro);
            }
        }
        if(f_locals.sum==f_locals.j&&f_locals.j!=1){
          f_locals.f_ret=f_locals.j;
          hoh_debug("Perfect Number");
          hoh_debug(shellstate.f_ret);
        }  
    }
    shellstate.f_done=true;
    shellstate.f_ret=f_locals.f_ret;
    if(shellstate.f_done==true&&shellstate.states==1){
      shellstate.states=2;
      hoh_debug(shellstate.f_ret);
      hoh_debug("Coroutine Completed");
    }
    h_end(f_coro);
  }
}
