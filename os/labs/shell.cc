#include "labs/shell.h"
#include "labs/vgatext.h"
//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  state.Output=0;
  state.NumberOfKeysPressed=0;
  state.NumberOfKeysPressedSoFar=0;
  state.complete=0;
  state.evaluate=0;
  state.flag=0;
  state.arg=0;
  state.functioncode=0;
  state.l=0;
  state.arg2_1=0;
  state.arg_1 =0;
  state.f_ret=0;
  state.f_done=true;
  state.states=0;
  state.statef=0;
  state.flag_s=0;
  state.mode=0;
  state.main=0;
  for (int i = 0; i < 5; i++)
  {
    state.statef_s[i]=0;
    state.f_ret_s[i]=0;
    state.f_done_s[i]=1;
    state.displayed[i]=true;
    state.arg_1_s[i]=0;
    state.arg2_1_s[i]=0;
    state.schedule[i]=0;
  }
  state.temp_index=0;
  state.index=0;
  state.stop=0;
}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//

// functioncode = 0 -> login
// functioncode = 1 -> homepage
// functioncode = 2 -> factorial
// functioncode = 5 -> factorial input
// functioncode = 6 -> factorial output
// functioncode = 3 -> fibonacii 
// functioncode = 7 -> fibonacci input
// functioncode = 8 -> fibonnaci output
// functioncode = 4 -> echo
// functioncode = 9 -> echo input
// functioncode = 10 -> echo output
// functioncode = 11 -> error
// 16 ->nonprimentive
// 
void shell_update(uint8_t scankey, shellstate_t& stateinout){

  hoh_debug("Got: "<<unsigned(scankey));
  if(scankey!=00)
    stateinout.NumberOfKeysPressed++;
    for (int i = 0; i < 5; i++){
      if(stateinout.statef_s[i]==2){
        stateinout.displayed[i]=true;
        stateinout.statef_s[i]=0;
        stateinout.arg2_1_s[i]=0;
        stateinout.arg_1_s[i]=0;
      }
    }
    if(stateinout.mode==1||stateinout.functioncode==16){
        // if((stateinout.functioncode==28||stateinout.functioncode==29)&&scankey==(uint32_t)1)
        // {
            // stateinout.mode=0;
            // stateinout.functioncode=1;
        // }
        // else
        if(stateinout.index>5){
          stateinout.functioncode=28;
        }
        else if(stateinout.flag_first>3||stateinout.flag_second>3){
        stateinout.functioncode=29;
        }
      //perfect output
      else if(scankey==(uint32_t)28&&stateinout.functioncode==23) {
      stateinout.functioncode=25;
      stateinout.mode=1;
      bool u1=false;
      int i=0;
      stateinout.flag_first=0;
      stateinout.flag_second=0;
      stateinout.index = 0;
      for (int i = 0; i < 5; ++i)
      {
        if(stateinout.f_done_s[i]==false){
          if(stateinout.schedule[i]==25){
            stateinout.flag_first++;
          }
          else if(stateinout.schedule[i]==26) stateinout.flag_second++;
          stateinout.index++;
        }
      }
      for (i = 0; i < 5; i++){
        if(stateinout.f_done_s[i]){
          u1 = true;
          break;
        }
      }
      hoh_debug("shell_update f_done "<<i);
      if(u1){
        stateinout.schedule[i]=25;
        stateinout.f_done_s[i]=false;
        stateinout.flag_s=0;
      }
      else{
        stateinout.index=6;
      }
      
    }
    //prime output
    else if(scankey==(uint32_t)28&&stateinout.functioncode==24) {
      stateinout.functioncode=26;
      stateinout.mode=1;
      bool u1=false;
      int i=0;
      stateinout.flag_first=0;
      stateinout.flag_second=0;
      stateinout.index = (-1);
      for (int i = 0; i < 5; ++i)
      {
        if(stateinout.f_done_s[i]==false){
          if(stateinout.schedule[i]==25){
            stateinout.flag_first++;
          }
          else if(stateinout.schedule[i]==26) stateinout.flag_second++;
          stateinout.index++;
        }
      }
      for (i = 0; i < 5; i++){
        if(stateinout.f_done_s[i]){
          u1 = true;
          break;
        }
      }
      hoh_debug("shell_update f_done "<<i);
      if(u1){
        stateinout.schedule[i]=26;
        stateinout.f_done_s[i]=false;
        stateinout.flag_s=0;
      }
      else{
        stateinout.index =6;
      }
    }
    //perfect input
    else if((scankey==(uint32_t)2||scankey==(uint32_t)3||scankey==(uint32_t)4||scankey==(uint32_t)5||scankey==(uint32_t)6||scankey==(uint32_t)7||scankey==(uint32_t)8||scankey==(uint32_t)9||scankey==(uint32_t)10||scankey==(uint32_t)11)&&(stateinout.functioncode==23||stateinout.functioncode==21))
    {
      int i=0;
      stateinout.main=0;
      if(scankey==(uint32_t)2)
        i=1;
      else if(scankey==(uint32_t)3)
        i=2;
      else if(scankey==(uint32_t)4)
        i=3;
      else if(scankey==(uint32_t)5)
        i=4;
      else if(scankey==(uint32_t)6)
        i=5;
      else if(scankey==(uint32_t)7)
        i=6;
      else if(scankey==(uint32_t)8)
        i=7;
      else if(scankey==(uint32_t)9)
        i=8;
      else if(scankey==(uint32_t)10)
        i=9;
      else if(scankey==(uint32_t)11)
        i=0;
      stateinout.arg_1_s[stateinout.index]=stateinout.arg_1_s[stateinout.index]*10+i;
      stateinout.arg2_1_s[stateinout.index]=stateinout.arg_1_s[stateinout.index];
      hoh_debug("index"<<stateinout.index);
      hoh_debug("ans"<<stateinout.arg2_1_s[stateinout.index]);
      stateinout.functioncode=23;
    }
    //prime input
    else if((scankey==(uint32_t)2||scankey==(uint32_t)3||scankey==(uint32_t)4||scankey==(uint32_t)5||scankey==(uint32_t)6||scankey==(uint32_t)7||scankey==(uint32_t)8||scankey==(uint32_t)9||scankey==(uint32_t)10||scankey==(uint32_t)11)&&(stateinout.functioncode==22||stateinout.functioncode==24))
    {
      int i=0;
      stateinout.main=0;
      if(scankey==(uint32_t)2)
        i=1;
      else if(scankey==(uint32_t)3)
        i=2;
      else if(scankey==(uint32_t)4)
        i=3;
      else if(scankey==(uint32_t)5)
        i=4;
      else if(scankey==(uint32_t)6)
        i=5;
      else if(scankey==(uint32_t)7)
        i=6;
      else if(scankey==(uint32_t)8)
        i=7;
      else if(scankey==(uint32_t)9)
        i=8;
      else if(scankey==(uint32_t)10)
        i=9;
      else if(scankey==(uint32_t)11)
        i=0;
      stateinout.arg_1_s[stateinout.index]=stateinout.arg_1_s[stateinout.index]*10+i;
      stateinout.arg2_1_s[stateinout.index]=stateinout.arg_1_s[stateinout.index];
      hoh_debug("ans"<<stateinout.arg2_1_s[stateinout.index]);
      stateinout.functioncode=24;
    }
    //perfect select
    else if(scankey==(uint32_t)28&&stateinout.flag_s==1)
    {
      stateinout.functioncode=21;
    }
    //prime select
    else if(scankey==(uint32_t)28&&stateinout.flag_s==2)
    {
      stateinout.functioncode=22;
    //   stateinout.flag_s=0;
    //   stateinout.flag_second++;
    }
    // arrow keys up 
    else if(scankey==(uint32_t)72)
    {
      stateinout.flag_s--;
      if(stateinout.flag_s<1)
        stateinout.flag_s=2;
      stateinout.flag2_s=stateinout.flag_s;
    }
    // arrow key down
    else if(scankey==(uint32_t)80)
    {
      stateinout.flag_s++;
      if(stateinout.flag_s>2)
        stateinout.flag_s=1;
      stateinout.flag2_s=stateinout.flag_s;
    }
    //esc to homepage
    else if(scankey==(uint32_t)1&&stateinout.functioncode==16)
    {
        stateinout.functioncode=1;
        // stateinout.index=0;
        // stateinout.mode=0;
    }
    //esc to temp_homepage 
    else if(scankey==(uint32_t)1)
    {
      stateinout.functioncode=16;
      stateinout.main=1;
    }
  }
  else if((scankey==(uint32_t)2||scankey==(uint32_t)3||scankey==(uint32_t)4||scankey==(uint32_t)5||scankey==(uint32_t)6||scankey==(uint32_t)7||scankey==(uint32_t)8||scankey==(uint32_t)9||scankey==(uint32_t)10||scankey==(uint32_t)11)&&(stateinout.functioncode==2||stateinout.functioncode==5))
  {
    int i=0;
    if(scankey==(uint32_t)2)
      i=1;
    else if(scankey==(uint32_t)3)
      i=2;
    else if(scankey==(uint32_t)4)
      i=3;
    else if(scankey==(uint32_t)5)
      i=4;
    else if(scankey==(uint32_t)6)
      i=5;
    else if(scankey==(uint32_t)7)
      i=6;
    else if(scankey==(uint32_t)8)
      i=7;
    else if(scankey==(uint32_t)9)
      i=8;
    else if(scankey==(uint32_t)10)
      i=9;
    else if(scankey==(uint32_t)11)
      i=0;
    stateinout.arg=stateinout.arg*10+i;
    stateinout.arg2=stateinout.arg;
    stateinout.functioncode=5;
  }
  // fibonacii input
  else if((scankey==(uint32_t)2||scankey==(uint32_t)3||scankey==(uint32_t)4||scankey==(uint32_t)5||scankey==(uint32_t)6||scankey==(uint32_t)7||scankey==(uint32_t)8||scankey==(uint32_t)9||scankey==(uint32_t)10||scankey==(uint32_t)11)&&(stateinout.functioncode==3||stateinout.functioncode==7))
  {
    int i=0;
    if(scankey==(uint32_t)2)
      i=1;
    else if(scankey==(uint32_t)3)
      i=2;
    else if(scankey==(uint32_t)4)
      i=3;
    else if(scankey==(uint32_t)5)
      i=4;
    else if(scankey==(uint32_t)6)
      i=5;
    else if(scankey==(uint32_t)7)
      i=6;
    else if(scankey==(uint32_t)8)
      i=7;
    else if(scankey==(uint32_t)9)
      i=8;
    else if(scankey==(uint32_t)10)
      i=9;
    else if(scankey==(uint32_t)11)
      i=0;
    stateinout.arg=stateinout.arg*10+i;
    stateinout.arg2=stateinout.arg;
    stateinout.functioncode=7;
  }
  // echo input
  else if((stateinout.functioncode==4||stateinout.functioncode==9)&&(scankey==(uint32_t)30||scankey==(uint32_t)31||scankey==(uint32_t)32||scankey==(uint32_t)33||(scankey==(uint32_t)57)||scankey==(uint32_t)34||scankey==(uint32_t)35||scankey==(uint32_t)36||scankey==(uint32_t)37||scankey==(uint32_t)38||scankey==(uint32_t)16||scankey==(uint32_t)17||scankey==(uint32_t)18||scankey==(uint32_t)19||scankey==(uint32_t)20||scankey==(uint32_t)21||scankey==(uint32_t)22||scankey==(uint32_t)23||scankey==(uint32_t)24||scankey==(uint32_t)25||scankey==(uint32_t)44||scankey==(uint32_t)45||scankey==(uint32_t)46||scankey==(uint32_t)47||scankey==(uint32_t)48||scankey==(uint32_t)49||scankey==(uint32_t)50))
  {
    char ch;
    if(scankey==(uint32_t)30)
      ch='a';
    else if(scankey==(uint32_t)31)
      ch='s';
    else if(scankey==(uint32_t)32)
      ch='d';
    else if(scankey==(uint32_t)33)
      ch='f';
    else if(scankey==(uint32_t)34)
      ch='g';
    else if(scankey==(uint32_t)35)
      ch='h';
    else if(scankey==(uint32_t)36)
      ch='j';
    else if(scankey==(uint32_t)37)
      ch='k';
    else if(scankey==(uint32_t)38)
      ch='l';
    else if(scankey==(uint32_t)16)
      ch='q';
    else if(scankey==(uint32_t)17)
      ch='w';
    else if(scankey==(uint32_t)18)
      ch='e';
    else if(scankey==(uint32_t)19)
      ch='r';
    else if(scankey==(uint32_t)20)
      ch='t';
    else if(scankey==(uint32_t)21)
      ch='y';
    else if(scankey==(uint32_t)22)
      ch='u';
    else if(scankey==(uint32_t)23)
      ch='i';
    else if(scankey==(uint32_t)24)
      ch='o';
    else if(scankey==(uint32_t)25)
      ch='p';
    else if(scankey==(uint32_t)44)
      ch='z';
    else if(scankey==(uint32_t)45)
      ch='x';
    else if(scankey==(uint32_t)46)
      ch='c';
    else if(scankey==(uint32_t)47)
      ch='v';
    else if(scankey==(uint32_t)48)
      ch='b';
    else if(scankey==(uint32_t)49)
      ch='n';
    else if(scankey==(uint32_t)50)
      ch='m';
    else if(scankey==(uint32_t)57)
      ch=' ';
    // else{
    //   stateinout.functioncode =11;
    // }
    stateinout.args[stateinout.l]=ch;
    stateinout.l++;
    stateinout.l3=stateinout.l;
    stateinout.functioncode=9;
  } 
  // coroutine input
  else if((scankey==(uint32_t)2||scankey==(uint32_t)3||scankey==(uint32_t)4||scankey==(uint32_t)5||scankey==(uint32_t)6||scankey==(uint32_t)7||scankey==(uint32_t)8||scankey==(uint32_t)9||scankey==(uint32_t)10||scankey==(uint32_t)11)&&(stateinout.functioncode==12||stateinout.functioncode==17))
  {
    int i=0;
    if(scankey==(uint32_t)2)
      i=1;
    else if(scankey==(uint32_t)3)
      i=2;
    else if(scankey==(uint32_t)4)
      i=3;
    else if(scankey==(uint32_t)5)
      i=4;
    else if(scankey==(uint32_t)6)
      i=5;
    else if(scankey==(uint32_t)7)
      i=6;
    else if(scankey==(uint32_t)8)
      i=7;
    else if(scankey==(uint32_t)9)
      i=8;
    else if(scankey==(uint32_t)10)
      i=9;
    else if(scankey==(uint32_t)11)
      i=0;
    stateinout.arg_1=stateinout.arg_1*10+i;
    stateinout.arg2_1=stateinout.arg_1;
    stateinout.functioncode=17;
  }
   // fiber input
  else if((scankey==(uint32_t)2||scankey==(uint32_t)3||scankey==(uint32_t)4||scankey==(uint32_t)5||scankey==(uint32_t)6||scankey==(uint32_t)7||scankey==(uint32_t)8||scankey==(uint32_t)9||scankey==(uint32_t)10||scankey==(uint32_t)11)&&(stateinout.functioncode==14||stateinout.functioncode==19))
  {
    int i=0;
    if(scankey==(uint32_t)2)
      i=1;
    else if(scankey==(uint32_t)3)
      i=2;
    else if(scankey==(uint32_t)4)
      i=3;
    else if(scankey==(uint32_t)5)
      i=4;
    else if(scankey==(uint32_t)6)
      i=5;
    else if(scankey==(uint32_t)7)
      i=6;
    else if(scankey==(uint32_t)8)
      i=7;
    else if(scankey==(uint32_t)9)
      i=8;
    else if(scankey==(uint32_t)10)
      i=9;
    else if(scankey==(uint32_t)11)
      i=0;
    stateinout.arg_1=stateinout.arg_1*10+i;
    stateinout.arg2_1=stateinout.arg_1;
    stateinout.functioncode=19;
  }
  // factorial output
  else if(scankey==(uint32_t)28&&stateinout.functioncode==5)
  {
    stateinout.functioncode=6;
  }
  // fibonacci output
  else if(scankey==(uint32_t)28&&stateinout.functioncode==7)
  {
    stateinout.functioncode=8;
  }
  // echo output
  else if(scankey==(uint32_t)28&&stateinout.functioncode==9)
  {
    stateinout.functioncode=10;
  }
  //coroutine output
  else if(scankey==(uint32_t)28&&stateinout.functioncode==17)
  {
    stateinout.functioncode=18;
  }
  //fiber output
  else if(scankey==(uint32_t)28&&stateinout.functioncode==19)
  {
    stateinout.functioncode=20;
  }
  // select non-preemptive option
  else if(scankey==(uint32_t)28&&stateinout.flag==6)
  {
    stateinout.functioncode=16;
    stateinout.flag=0;
  }
  // select fiber option
  else if(scankey==(uint32_t)28&&stateinout.flag==5)
  {
    stateinout.functioncode=14;
    stateinout.flag=0;
    // if(stateinout.states=2){
    stateinout.arg2_1=0;
    stateinout.arg_1=0;
    // }
  }
   // select coroutine option
  else if(scankey==(uint32_t)28&&stateinout.flag==4)
  {
    stateinout.functioncode=12;
    stateinout.flag=0;
// if(stateinout.states=2){
    stateinout.arg2_1=0;
    stateinout.arg_1=0;
    // }
  }
  // selectf echo option
  else if(scankey==(uint32_t)28&&stateinout.flag==3)
  {
    stateinout.functioncode=4;
    stateinout.flag=0;
  }
  // select fibonacci option
  else if(scankey==(uint32_t)28&&stateinout.flag==2)
  {
    stateinout.functioncode=3;
    stateinout.flag=0;
  }
  // select factorial option
  else if(scankey==(uint32_t)28&&stateinout.flag==1)
  {
    stateinout.functioncode=2;
    stateinout.flag=0;
  }
  // arrow keys up 
  else if(scankey==(uint32_t)72)
  {
    stateinout.flag--;
    if(stateinout.flag<1)
      stateinout.flag=6;
    stateinout.flag2=stateinout.flag;
  }
  // arrow key down
  else if(scankey==(uint32_t)80)
  {
    stateinout.flag++;
    if(stateinout.flag>6)
      stateinout.flag=1;
    stateinout.flag2=stateinout.flag;
  }
  // login to homepage
  else if(scankey==(uint32_t)28)
    stateinout.functioncode=1;
  //esc to homepage 
  else if(scankey==(uint32_t)1)
    stateinout.functioncode=1;
  // error state
  else 
    stateinout.functioncode=11;
  // hoh_debug("ki "<<stateinout.index);
  // hoh_debug(" kio " << stateinout.arg2_1_s[stateinout.index]);
}

//
// do computation
//
void shell_step(shellstate_t& stateinout){
  // if(stateinout.mode==1&&stateinout.index>0)
  // {
  //   stateinout.temp_index++;
  //     if(stateinout.temp_index>=stateinout.index)
  //     {
  //       stateinout.temp_index=0;
  //     }
  // }
  // calculating answer factorial
  // else 
    if(stateinout.functioncode==6) {
      int result=1;
      for(int i=1;i<=stateinout.arg;i++)
        result*=i;
      stateinout.Output=result;
      stateinout.arg=0;
    }
    else if(stateinout.functioncode==8){
      // calculating fibonacci and result1 and 2 is for 0 and 1 terms
      int result1=0,result2=1,result3=0,result=0;
      if(stateinout.arg==1) result=result1;
      else if(stateinout.arg==2) result=result2;
      else{
          for(int i=3;i<=stateinout.arg;i++){
            result3=result1+result2;
            result1=result2;
            result2=result3;
          }
          result=result3;
      }
      stateinout.Output=result;
      stateinout.arg=0;
    }
    else if(stateinout.functioncode==10){
      // copying string to echo
      stateinout.l2=stateinout.l;
      stateinout.l=0;
    }
    else if(stateinout.functioncode==1){
    //reset everything 
      stateinout.l3=0;
      stateinout.l=0;
      stateinout.arg2=0;
      stateinout.arg=0;
      // stateinout.f_ret=0;
      // stateinout.f_done=false;
      stateinout.NumberOfKeysPressedSoFar=0;
      // if(stateinout.f_done==true&&stateinout.states==2)
      // {
      //   stateinout.arg_1=0;
      //   stateinout.arg2_1=0;
      // }
      // if(stateinout.f_done==true&&stateinout.statef==2)
      // {
      //   stateinout.arg_1=0;
      //   stateinout.arg2_1=0;
      // }
      // stateinout.index=0;
      // stateinout.temp_index=0;
      stateinout.flag_index=0;
      // stateinout.flag_first=0;
      // stateinout.flag_second=0;
      // stateinout.mode=0;
      // for(int i=0;i<5;i++)
      // {
      //   stateinout.f_done_s[i]=false;
      //   stateinout.f_ret_s[i]=0;
      //   stateinout.displayed[i]=false;
      //   stateinout.schedule[i]=0;
      //   stateinout.statef_s[i]=0;
      // }
      //hoh_debug("Reinitialized");
    }
    else if(stateinout.functioncode==18) {
      if(stateinout.f_done==true&&stateinout.states==2){
        stateinout.functioncode=13;
        stateinout.arg_1=0;
      }
    }
    else if(stateinout.functioncode==20) {
      if(stateinout.f_done==true&&stateinout.statef==2){
        stateinout.functioncode=15;
        stateinout.arg_1=0;
      }
    }
    else if(stateinout.functioncode==16){
      stateinout.mode=1;
      stateinout.f_done=false;
    }
    /*else if(stateinout.functioncode==25||stateinout.functioncode==26)
    {
      stateinout.arg_1_s[stateinout.temp_index]=0;
      if(stateinout.f_done_s[stateinout.temp_index]==true)
      {
        stateinout.arg2_1_s[stateinout.temp_index]=0;
        stateinout.functioncode=27;
      }
      //stateinout.arg2_1_s[stateinout.temp_index]=0;
    }*/
    // else if(stateinout.functioncode==16) {
      //stateinout.arg_1_s[stateinout.temp_index]=0;
      // for(int i=0;i<5;i++){
      //   if(stateinout.f_done_s[i]==true){
          // stateinout.arg2_1_s[sta.index]=0;
          // stateinout.arg_1_s[i] =0;
      //   }
      // }
    // }
    // else if(stateinout.functioncode==28||stateinout.functioncode==29){
      // stateinout.index=0;
      // stateinout.temp_index=0;
      // for(int i=0;i<5;i++)
      //   stateinout.schedule[i]=0;
    // }
    /*
  else if(stateinout.functioncode!=0&&stateinout.functioncode!=1)
  {
    if(stateinout.f_done==true&&stateinout.states==2)
      {
        stateinout.functioncode=13;
      }
  }*/
}
  
//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
      // hoh_debug(shell.temp_index);
  render.functioncode=shell.functioncode;
  render.NumberOfKeysPressed=shell.NumberOfKeysPressed;
  render.state=shell.state;
  render.displayResult=shell.evaluate;
  render.FunctionResult=shell.Output;
  render.complete=shell.complete;
  render.evaluate=shell.evaluate;
  render.flag=shell.flag2;
  render.l2=shell.l3;
  render.arg2=shell.arg2;
  render.arg2_1=shell.arg2_1;
  render.arg=shell.arg;
    for(int i=0;i<shell.l3;i++){
      render.input[i]=shell.args[i];
    }
    for(int i=0;i<shell.l2;i++){
      render.input[i]=shell.args[i];
      render.args[i]=shell.args[i];
    }
    render.l=shell.l2;
    render.f_ret=shell.f_ret;
    render.f_done=shell.f_done;
    render.f_i=shell.f_i;
    render.f_j=shell.f_j;
    render.f_sum=shell.f_sum;
    render.states=shell.states;
    render.statef=shell.statef;
    render.flag2_s=shell.flag2_s;
    render.flag_s=shell.flag_s;
    render.mode=shell.mode;
    render.main=shell.main;
    render.index=shell.index;
    render.temp_index=shell.temp_index;
    render.flag_index=shell.flag_index;
    render.flag_first=shell.flag_first;
    render.flag_second=shell.flag_second;
    for (int i = 0; i < 5; i++){
      render.statef_s[i]=shell.statef_s[i];
      render.f_ret_s[i]=shell.f_ret_s[i];
      render.f_done_s[i]=shell.f_done_s[i];
      // if(render.f_done_s[i])hoh_debug("f_done" <<render.f_done_s[i]);
      // if(shell.f_done_s[i])hoh_debug(" "<<shell.f_done_s[i]);
      render.arg2_1_s[i]=shell.arg2_1_s[i];
      render.schedule[i]=shell.schedule[i];
      render.displayed[i]=shell.displayed[i];
    }
    // for (int i = 0; i < 5; ++i)
    // {
    //   hoh_debug(i<<"is "<<render.f_done_s[i]);
    // }
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){ 
  bool u =true;
  bool w =true; 
  for (int i = 0; i < 5; ++i)
  {
    u = u && (a.f_done_s[i]==b.f_done_s[i]);  
    w = w && (a.statef_s[i]==b.statef_s[i]);
  }
  // hoh_debug(u);
  // if(!w)hoh_debug("sravya");
  // if(!u)hoh_debug("datta");
  if(a.NumberOfKeysPressed==b.NumberOfKeysPressed&&a.f_done==b.f_done&&a.arg2_1==b.arg2_1&&a.arg_1==b.arg_1&&a.functioncode==b.functioncode&&u&&w)
        return true;
  return false;
}


static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
//static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){
    hoh_debug("Rendr");
    // for (int i = 0; i < 5; ++i)
    // {
    //   hoh_debug(i<<"is "<<state.f_done_s[i]);
    // }
    drawrect(0,0,80,25,9,0,w,h,vgatext_base);
    drawtext(23,2,"Number of keys pressed:",23,0,11,w,h,vgatext_base);
    drawnumberinhex(47,2, (uint32_t)state.NumberOfKeysPressed,w,0,11, w, h, vgatext_base);
    if(state.functioncode==0){
      drawtext(33,11,"WELCOME",7,0,11,w,h,vgatext_base);
      drawtext(23,13,"Press enter to see Main Menu",28,0,11,w,h,vgatext_base);
    }
    //Displaying rectangle on selecting 
    else if(state.functioncode==1){ 
      fillrect(1,3,78,24,0,1,w,h,vgatext_base);
      drawtext(22,5,"MAIN MENU",9,0,11,w,h,vgatext_base);
      drawtext(10,7,"1.Factorial",11,0,11,w,h,vgatext_base);
      drawtext(10,9,"2.Fibonacci",11,0,11,w,h,vgatext_base);
      drawtext(10,11,"3.Echo",6,0,11,w,h,vgatext_base);
      drawtext(34,7,"4.Coroutine",12,0,11,w,h,vgatext_base);
      drawtext(34,9,"5.Fiber",7,0,11,w,h,vgatext_base);
      drawtext(34,11,"6.Non-Preemptive",16,0,11,w,h,vgatext_base);
      drawtext(1,23,"Enter your choice using arrows",31,0,11,w,h,vgatext_base);
      if(state.flag==1){
          drawrect(9,6,26,9,0,01,w,h,vgatext_base);
          drawtext(10,7,"1.Factorial",11,0,11,w,h,vgatext_base);
      }
      else if(state.flag==2){
          drawrect(9,8,26,11,0,01,w,h,vgatext_base);
          drawtext(10,9,"2.Fibonacci",11,0,11,w,h,vgatext_base);
      }
      else if(state.flag==3){
          drawrect(9,10,27,13,0,01,w,h,vgatext_base);
          drawtext(10,11,"3.Echo",6,0,11,w,h,vgatext_base);
      }
      else if(state.flag==4){
        drawrect(32,6,51,9,0,01,w,h,vgatext_base);
        drawtext(34,7,"4.Coroutine",12,0,11,w,h,vgatext_base);
      }
      else if(state.flag==5){
        drawrect(33,8,51,11,0,01,w,h,vgatext_base);
        drawtext(34,9,"5.Fiber",7,0,11,w,h,vgatext_base);
      }
      else if(state.flag==6){
        drawrect(33,10,51,13,0,01,w,h,vgatext_base);
        drawtext(34,11,"6.Non-Preemptive",16,0,11,w,h,vgatext_base);
      }
      if(state.f_done==true&&state.states==2){
        drawtext(7,20,"answer for coroutine is",23,0,11, w, h, vgatext_base);
        drawnumberinhex(31,20,(uint32_t)state.f_ret,w,0,11, w, h, vgatext_base);
      }
      else if(state.f_done==true&&state.statef==2)
      {
        drawtext(7,21,"answer for fiber is",19,0,11, w, h, vgatext_base);
        drawnumberinhex(35,21,(uint32_t)state.f_ret,w,0,11, w, h, vgatext_base);
      }
    }
    else
    {
      fillrect(1,3,78,24,0,1,w,h,vgatext_base);
      drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
      
      // factorial
      if(state.functioncode==2||state.functioncode==5||state.functioncode==6){
          drawtext(7,8,"Enter a number whose factorial is to be calculated",56,0,11, w, h, vgatext_base);
          drawnumberinhex(58,8,(uint32_t)state.arg2,w,0,11, w, h, vgatext_base);
          if(state.functioncode==6){
            drawtext(7,13,"Factorial is",12,0,11, w, h, vgatext_base);
            drawnumberinhex(20,13,(uint32_t)state.FunctionResult,w,0,11, w, h, vgatext_base);
          }
      }
      // fibonacci
      else if(state.functioncode==3||state.functioncode==7||state.functioncode==8){
          drawtext(7,8,"Enter the term of fibonacci series to be calculated",60, 0,11, w, h, vgatext_base);
          drawnumberinhex(59,8,(uint32_t)state.arg2,w,0,11, w, h, vgatext_base);
          if(state.functioncode==8){
            drawtext(7,13,"Term is",7, 0,11, w, h, vgatext_base);
            drawnumberinhex(15,13,(uint32_t)state.FunctionResult,w,0,11, w, h, vgatext_base);
          }
      }
      // echo
      else if(state.functioncode==4||state.functioncode==9||state.functioncode==10){
          drawtext(12,8,"Enter the string you want to echo :",35,0,11, w, h, vgatext_base);
          drawtext(48,8,(const char*)&state.input,state.l2,0,11, w, h, vgatext_base);
          if(state.functioncode==10){
            drawtext(12,11,"String Entered is :",20,0,11, w, h, vgatext_base);
            drawtext(33,11,(const char*)&state.args,state.l, 0,11, w, h, vgatext_base);
          }
      }
      //coroutine input
      else if(state.functioncode==12||state.functioncode==17)
      {
        drawtext(3,8,"Enter the upper limit to calculate largest perfect number",57, 0,11, w, h, vgatext_base);
        drawnumberinhex(61,8,(uint32_t)state.arg2_1,w,0,11, w, h, vgatext_base);
      }
      //fiber input
      else if(state.functioncode==14||state.functioncode==19)
      {
        drawtext(3,8,"Enter the upper limit to calculate largest perfect number",57, 0,11, w, h, vgatext_base);
        drawnumberinhex(61,8,(uint32_t)state.arg2_1,w,0,11, w, h, vgatext_base);
      }
      //preemptive option
      else if(state.functioncode==16||state.mode==1){
          fillrect(1,3,78,24,0,1,w,h,vgatext_base);
          drawtext(5,5,"Perfect Number",14,0,11,w,h,vgatext_base);
          drawtext(5,7,"Prime Number",12,0,11,w,h,vgatext_base);
          // if(state.statef_s[state.temp_index]==2){
            // hoh_debug("Displayin in functioncode=16 or mode=1");
            // for(int i=0;i<5;i++){
            //   hoh_debug("i is "<<i);
            //   hoh_debug("f_done "<<state.f_done_s[i]);
            //   hoh_debug("statef_s "<<state.statef_s[i]);
            //   hoh_debug("displayed or not "<<state.displayed[i]);
            //   hoh_debug("schedule "<<state.schedule[i]);
            //   hoh_debug("done");
            // }
          hoh_debug("index "<<state.index);
          hoh_debug("main "<<state.main);
          hoh_debug("present functioncode "<<state.functioncode);
          // hoh_debug("mode "<<state.mode);
          // hoh_debug("bye");
          if(state.flag_s==1) {
            drawrect(4,4,20,7,0,01,w,h,vgatext_base);
            drawtext(5,5,"Perfect Number",14,0,11,w,h,vgatext_base);
          }
          else if(state.flag_s==2){
            drawrect(4,6,20,9,0,01,w,h,vgatext_base);
            drawtext(5,7,"Prime Number",12,0,11,w,h,vgatext_base);
          }
          //perfect input
          if(state.functioncode==21||state.functioncode==23){
            fillrect(1,3,78,24,0,1,w,h,vgatext_base);
            // for(int i=0;i<5;i++){
              // if(state.schedule[i]==25){
                drawtext(7,8,"largest perfect number below",28, 0,11, w, h, vgatext_base);
                drawnumberinhex(36,8,(uint32_t)state.arg2_1_s[state.index],w,0,11, w, h, vgatext_base);
              // }
            // }
            drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
          }
          //prime input
          else if(state.functioncode==22||state.functioncode==24)
          {
            fillrect(1,3,78,24,0,1,w,h,vgatext_base);
            // for(int i=0;i<5;i++){
              // if(state.schedule[i]==26){
                drawtext(7,8,"largest prime number below",26, 0,11, w, h, vgatext_base);
                drawnumberinhex(35,8,(uint32_t)state.arg2_1_s[state.index],w,0,11, w, h, vgatext_base);
              // }
            // }
            drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
          }
          if(state.functioncode==28){
            fillrect(1,3,78,24,0,1,w,h,vgatext_base);
            drawtext(23,8,"Cannot enter more than 5 processes",34,0,11, w, h, vgatext_base);
            drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
          }
          else if(state.functioncode==29){
            fillrect(1,3,78,24,0,1,w,h,vgatext_base);
            drawtext(23,8,"Cannot enter more than 3 processes of this kind",49,0,11, w, h, vgatext_base);
            drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
          }
          hoh_debug("fors");
          for(int i=0;i<5;i++){
            hoh_debug("fors in "<<i<<" "<<state.f_done_s[i]<<" "<<state.statef_s[i]<<" "<<state.f_ret_s[i]);
            if(state.f_done_s[i]==true&&state.statef_s[i]==2&&state.displayed[i]==false){
              hoh_debug("Pt");
              if(state.schedule[i]==25){
                hoh_debug("Entered this printing "<<state.f_ret_s[i]);  
                fillrect(1,3,78,24,0,1,w,h,vgatext_base);
                drawtext(7,23-i,"largest perfect number:",23,0,11, w, h, vgatext_base);
                drawnumberinhex(31,23-i,(uint32_t)state.f_ret_s[i],w,0,11, w, h, vgatext_base);
              }
              else if(state.schedule[i]==26){
                hoh_debug("pt2");
                fillrect(1,3,78,24,0,1,w,h,vgatext_base);
                drawtext(7,23-i,"largest prime number:",21,0,11, w, h, vgatext_base);
                drawnumberinhex(29,23-i,(uint32_t)state.f_ret_s[i],w,0,11, w, h, vgatext_base);
              }
            }
          }
      }
      else if(state.functioncode==11){
        // invalid
        drawtext(23,17,"Entering invalid keys. Try Again!",33,0,11,w,h,vgatext_base);
      }
      if(state.f_done==true&&state.states==2&&state.mode==0){
        fillrect(1,3,78,24,0,1,w,h,vgatext_base);
        drawtext(14,8,"largest perfect number below",28, 0,11, w, h, vgatext_base);
        drawnumberinhex(42,8,(uint32_t)state.arg2_1,w,0,11, w, h, vgatext_base);
        drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
        drawtext(21,12,"answer is :",11,0,11, w, h, vgatext_base);
        drawnumberinhex(34,12,(uint32_t)state.f_ret,w,0,11, w, h, vgatext_base);
      }
      if(state.f_done==true&&state.statef==2&&state.mode==0) {
        hoh_debug("state.f_ret");
        fillrect(1,3,78,24,0,1,w,h,vgatext_base);
        drawtext(14,8,"largest prime number below",26, 0,11, w, h, vgatext_base);
        drawnumberinhex(41,8,(uint32_t)state.arg2_1,w,0,11, w, h, vgatext_base);
        drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
        drawtext(21,12,"answer is :",11,0,11, w, h, vgatext_base);
        drawnumberinhex(33,12,(uint32_t)state.f_ret,w,0,11, w, h, vgatext_base);
      }
  }
  hoh_debug(" ");
}
 //coroutine
 /*if(state.f_done==true&&state.states==2&&state.functioncode==1)
      {
        // hoh_debug(state.f_ret);
        // hoh_debug(state.f_done);
        fillrect(1,3,78,24,0,1,w,h,vgatext_base);
          drawtext(23,22,"Press ESC to go back to main screen",35,0,11,w,h,vgatext_base);
        drawtext(5,8,"Displaying the largest perfect number under 1,00,00,0000",62,0,11, w, h, vgatext_base);
        drawnumberinhex(62,8,(uint32_t)state.f_ret,w,0,11, w, h, vgatext_base);
      //drawnumberinhex(62,10,(uint32_t)state.NumberOfKeysPressedSoFar,w,0,11, w, h, vgatext_base);
      //drawnumberinhex(62,12,(uint32_t)state.NumberOfKeysPressed,w,0,11, w, h, vgatext_base);
      //hoh_debug(state.f_ret);
      }*/
//
//
// helper functions
//
//

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  vgatext::writechar(y*w+x,c,bg,fg,vgatext_base);
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int y=y0;y<y1;y++){
    for(int x=x0;x<x1;x++){
      writecharxy(x,y,0,bg,fg,w,h,vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){

  writecharxy(x0,  y0,  0xc9, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y0,  0xbb, bg,fg, w,h,vgatext_base);
  writecharxy(x0,  y1-1,0xc8, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y1-1,0xbc, bg,fg, w,h,vgatext_base);

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y0, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y1-1, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x0,y, 0xba, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x1-1,y, 0xba, bg,fg, w,h,vgatext_base);
  }
}

static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
    if(!str[i]){
      break;
    }
  }
}

static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%16);
    number=number/16;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

