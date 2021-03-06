#include <stdio.h>
#include <Boards.h>
#include <doxygen.h>
#include <NexButton.h>
#include <NexCheckbox.h>
#include <NexConfig.h>
#include <NexCrop.h>
#include <NexDualStateButton.h>
#include <NexGauge.h>
#include <NexGpio.h>
#include <NexHardware.h>
#include <NexHotspot.h>
#include <NexNumber.h>
#include <NexObject.h>
#include <NexPage.h>
#include <NexPicture.h>
#include <NexProgressBar.h>
#include <NexRadio.h>
#include <NexRtc.h>
#include <NexScrolltext.h>
#include <NexSlider.h>
#include <NexText.h>
#include <NexTimer.h>
#include <Nextion.h>
#include <NexTouch.h>
#include <NexUpload.h>
#include <NexVariable.h>
#include <NexWaveform.h>
/* 넥션 디스플레이의 라이브러리 함수 추가 */

int relayPin = 8; // relayPin-sensor uses 8pin
int soundPin = 7; // sound-sensor uses 7pin
int soundOn = 0; // SoundOn = 0 으로 초기화
int i;
uint32_t UserMenuMuteImg = 0; // 0 : ON Image , 9 : OFF Image
uint32_t RecipeMenuMuteImg = 2; // 2 : ON Image , 8 : OFF Image
uint32_t recipeCheckImg = 11; 
const char recipeStep[5][8] = {0};
String message[5];
NexButton UserMenuRecipe0 = NexButton(0,2,"b0"); // 변수초기화, UserMenu 의 레시피0 버튼 할당(page:0,id:2,name:"b0")
NexButton UserMenuRecipe1 = NexButton(0,3,"b1"); 
NexButton UserMenuRecipe2 = NexButton(0,4,"b2");
NexButton UserMenuRecipe3 = NexButton(0,5,"b3");
NexButton UserMenuSetting = NexButton(0,7,"b6");
NexPicture UserMenuMute = NexPicture(0, 8, "p0"); 
// []p0 --> mute
// 변수초기화, UserMenu 의 음소거 버튼 할당(page:0,id:2,name:"b0")
NexText RecipeMenuName = NexText(1,1,"t0");       // 변수초기화, RecipeMenu의 레시피명 텍스트 할당(page:1,id,1,name:"t0")
NexText RecipeMenuStatus = NexText(1,2,"t2");     // 변수초기화, RecipeMenu의 상태 텍스트 할당(page:1,id:2,name:"t2")
NexText RecipeMenuStep0 = NexText(1,3,"t3");      // 변수초기화, RecipeMenu의 레시피스텝0 할당(page:1,id:3,name:"t2") 
NexText RecipeMenuStep1 = NexText(1,4,"t4");
NexText RecipeMenuStep2 = NexText(1,5,"t5");
NexText RecipeMenuStep3 = NexText(1,6,"t6");
NexText RecipeMenuStep4 = NexText(1,7,"t7");
NexButton RecipeMenuCancel = NexButton(1,14,"b2");    // 변수초기화, RecipeMenu의 취소하기 버튼 할당(page:1,id:14,name:"b2")
NexButton RecipeMenuStart = NexButton(1,12,"b0");     // 변수초기화, RecipeMenu의 시작버튼 할당(page:1,id:12,name:"b0")
NexButton RecipeMenuTempStop = NexButton(1,13,"b1");  // 변수초기화, RecipeMenu의 일시정지버튼 할당(page:1,id:12,name:"b0")
NexPicture RecipeMenuMute = NexPicture(1,15,"p0");    // 변수초기화, RecipeMenu의 음소거 이미지 할당(page:1,id:15,name:"p0")
NexPicture recipeCheck[5] = {NexPicture(1, 16, "p1"),NexPicture(1, 17, "p2"),NexPicture(1, 18, "p3"),NexPicture(1, 19, "p4"),NexPicture(1, 20, "p5")};
NexPage page0 = NexPage(0,0,"page0");                 // 변수초기화, page0 할당
NexPage page1 = NexPage(1,0,"page1");                 // 변수초기화, page1 할당

NexTouch *nex_listen_list[]= {
  &page0,
  &page1,
  &UserMenuRecipe0,
  &UserMenuRecipe1,
  &UserMenuRecipe2,
  &UserMenuRecipe3,
  &UserMenuMute,
  &UserMenuSetting,
  &RecipeMenuStart,
  &RecipeMenuTempStop,
  &RecipeMenuMute,
  &recipeCheck[0],
  &recipeCheck[1],
  &recipeCheck[2],
  &recipeCheck[3],
  &recipeCheck[4],
  &RecipeMenuCancel,
  NULL
};

/* Files */
char currentRecipe[100][50] = {
  {"fc 10","w 5","p 5","w 5","p 5"}
};
char str1[100];
char str2[100];
char str3[100];

/* Load Recipe */ //[]Script_loadRecipe
String loadRecipe(int rid){
  if(rid == 0){
    return recipe[0];
  }
  else if(rid ==1){
    return recipe[1];
  }
}

/* Load RecipeName */
char * getRName(int recipeNum){
  int num = recipeNum;
  char *recipeName = str1;
  if(num == 0){
    strcpy(recipeName, "overpour");
    return recipeName;
  }
  if(num == 1){
    strcpy(recipeName, "dotdrip");
    return recipeName;
  }
}

/* Lexer */
void lexer(String messagee){
   
    String msg = messagee;
    int first = msg.indexOf(","); // 첫번째 콤마위치
    int second = msg.indexOf(",",first+1); // 두번째 콤마 위치
    int third = msg.indexOf(",",second+1); // 두번째 콤마 위치
    int forth = msg.indexOf(",",third+1); // 세번째 콤마 위치
    int strlength = msg.length(); // 문자열 길이
    message[0] = msg.substring(0, first); // 첫번째 토큰
    message[1] = msg.substring(first+1, second); // 두번째 토큰
    message[2] = msg.substring(second+1,third); // 세번째 토큰
    message[3] = msg.substring(third+1,forth); // 네번째 토큰
    message[4] = msg.substring(forth+1,strlength);
    /*sting to char for setText (Char save to recipeStep[])*/
    message[0].toCharArray(recipeStep[0], message[0].length()+1);
    message[1].toCharArray(recipeStep[1], message[1].length()+1);
    message[2].toCharArray(recipeStep[2], message[2].length()+1);
    message[3].toCharArray(recipeStep[3], message[3].length()+1);
    message[4].toCharArray(recipeStep[4], message[4].length()+1);
  
}

/* 실제 구동(현재는 stub 코드) */
void Continue(){
    for( i = 0; i< 6 ;i ++){
    int first = message[i].indexOf(" ");
    int strlength = message[i].length();
    String command = message[i].substring(0,first); //fc
    String sec = message[i].substring(first+1,strlength); // 10
    if(command.equals("fc")){ //string 비교함수 string.equals() 같으면 1 다르면 0 반환
       recipeCheck[i].setPic(12);
       digitalWrite(relayPin,LOW); 
       delay(sec.toInt()*300); //string -> int 
       Serial.println(F("/*---filterClean 단계입니다.---*/"));
       Serial.println(sec+F("초 경과"));
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);//너무 빨라 바로 아래의 코드를 무시하므로, Serail.write(0xff)로 시간 벌어줘야됨.
       recipeCheck[i].setPic(recipeCheckImg);
    }
    else if(command.equals("w")){
       recipeCheck[i].setPic(12);
       digitalWrite(relayPin,HIGH); 
       delay(sec.toInt()*300);
       Serial.println();
       Serial.println(F("/*--- wait 단계입니다. ---*/"));
       Serial.println(sec+F("초 경과"));
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);
       recipeCheck[i].setPic(recipeCheckImg);
    }
    else if(command.equals("p")){
       recipeCheck[i].setPic(12);
       digitalWrite(relayPin,LOW); 
       delay(sec.toInt()*300);
//       Serial.println();
       Serial.println(F("/*--- pour 단계입니다. ---*/"));
       Serial.println(sec+F("초 경과"));
       Serial.write(0xff);
       Serial.write(0xff);
       Serial.write(0xff);
       recipeCheck[i].setPic(recipeCheckImg);
    }
    
//    delay(1000);
//    Serial.println(i);
  }
  digitalWrite(relayPin,HIGH); 
  RecipeMenuStatus.setText("Finish");
//  int second = message[1].indexOf(" ");
//  strlength = message[1].length();
//  command = message[1].substring(0,second); //p
//  sec = message[1].substring(second+1,strlength);
//  if(command.equals("w")){
//     delay(sec.toInt()*1000);
//     recipeCheck[1].setPic(recipeCheckImg);
//  }
  
}


/* User Menu Event Handler */
void UserMenuRecipe0PopCallback(void *ptr){ //UserMenu의 레시피0버튼 눌렸다 떼어질 때의 이벤트 핸들러.
  page1.show(); //drip menu
  RecipeMenuStatus.setText("Proceed"); //[] dripping
  int rid = 0;
  const char * recipeName = gRlist(rid); //getRecipeName
  RecipeMenuName.setText(recipeName);
  String msg = loadRecipe(rid);  
  lexer(msg); //recipeStep[0] ~ [4] 까지 채워짐.


  RecipeMenuStep0.setText(recipeStep[0]);
  RecipeMenuStep1.setText(recipeStep[1]);
  RecipeMenuStep2.setText(recipeStep[2]);
  RecipeMenuStep3.setText(recipeStep[3]);
  RecipeMenuStep4.setText(recipeStep[4]);
  
  
  Continue();
  

  RecipeMenuMute.setPic(RecipeMenuMuteImg);
  setSound();
  
}

void UserMenuRecipe1PopCallback(void *ptr){ //UserMenu의 레시피1버튼 눌렸다 떼어질 때의 이벤트 핸들러.
  page1.show();
  int rid = 1;
  const char * recipeName = gRlist(rid);
  RecipeMenuName.setText(recipeName);
  String msg = loadRecipe(rid);  //  {"test 10,wait2 10,Pour 10s,wait 10s"},
  lexer(msg);
  RecipeMenuStep0.setText(recipeStep[0]);
  RecipeMenuStep1.setText(recipeStep[1]);
  RecipeMenuStep2.setText(recipeStep[2]);
  RecipeMenuStep3.setText(recipeStep[3]);
  RecipeMenuStep4.setText(recipeStep[4]);
  Continue();
  RecipeMenuMute.setPic(RecipeMenuMuteImg);
  setSound();
}
//[]
//gotoDripMenu(rid, 


/* Mute */
void UserMenuMuteCallback(void *ptr){
   UserMenuMute.getPic(&UserMenuMuteImg); // 음소거ON 이미지ID : 0,2 음소거OFF 이미지ID : 8,9
   if (UserMenuMuteImg == 0){
        UserMenuMuteImg = 9;          
        RecipeMenuMuteImg = 8;  
        soundOn = 1;
    }
    else{
        UserMenuMuteImg = 0;
        RecipeMenuMuteImg = 2;
        soundOn = 0;
    }
    UserMenuMute.setPic(UserMenuMuteImg); //UserMenu의 음소거 버튼에 이미지 세팅.
}

/* Recipe Menu Event Handler */
/* Start  */
void RecipeMenuStartPopcallback(void *ptr){ //RecipeMenu의 시작버튼이 눌렸을 때의 이벤트 핸들러.
  digitalWrite(relayPin,LOW);               //relayPin : LOW
  RecipeMenuStatus.setText("Proceed");      //RecipeMenu의 상태창 "Proceed" 로 세팅
  setSound();
}

/* TempStop  */
void RecipeMenuTempStopPopcallback(void *ptr){//RecipeMenu의 일시정지버튼이 눌렸을 때의 이벤트 핸들러.
  digitalWrite(relayPin,HIGH);               //relayPin : HIGH
  RecipeMenuStatus.setText("Stop");          //RecipeMenu의 상태창 "Stop" 로 세팅
  setSound();
}

/* Mute */
void RecipeMenuMuteCallback(void *ptr){ //RecipeMenu의 음소거 버튼이 눌려졌다 떼어질 때의 이벤트 핸들러.
  RecipeMenuMute.getPic(&RecipeMenuMuteImg);
   if (RecipeMenuMuteImg == 2){ 
        UserMenuMuteImg = 9; 
        RecipeMenuMuteImg = 8;      
        soundOn = 1;
    }
    else{
        UserMenuMuteImg = 0;
        RecipeMenuMuteImg = 2;
        soundOn = 0;
    }
    RecipeMenuMute.setPic(RecipeMenuMuteImg);
}

/* Move to page0 */
void RecipeMenuCancelCallback(void *ptr){ 
  page0.show();                           //page0으로 이동.
  UserMenuMute.setPic(UserMenuMuteImg);   //UserMenu의 음소거버튼 이미지 세팅.
  setSound();
}

/* System Related  */

void setup() {
  Serial.begin(9600);
  delay(500);
//  for(int i = 0; i < 2; i++){
//    Serial.print(recipe[i]);
//  }

  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
//  Serial.end();
//  Serial.begin(115200);
  
  /*출력 핀 설정*/
  pinMode(relayPin,OUTPUT);
  pinMode(soundPin,OUTPUT);
  
  /*버튼이 눌렸다 떼어지는 이벤트가 발생했을 때 인자의 콜백함수 실행*/

  UserMenuRecipe0.attachPop(UserMenuRecipe0PopCallback); 
  UserMenuRecipe1.attachPop(UserMenuRecipe1PopCallback);
  RecipeMenuStart.attachPop(RecipeMenuStartPopcallback);
  RecipeMenuTempStop.attachPop(RecipeMenuTempStopPopcallback);
  UserMenuMute.attachPop(UserMenuMuteCallback);
  RecipeMenuMute.attachPop(RecipeMenuMuteCallback);
  RecipeMenuCancel.attachPop(RecipeMenuCancelCallback);
}

void loop() {
  delay(30);
  nexLoop(nex_listen_list);
}

void setSound(){ // sound setting.
  if(soundOn == 1){
      tone(soundPin,400,100);
  }
}
