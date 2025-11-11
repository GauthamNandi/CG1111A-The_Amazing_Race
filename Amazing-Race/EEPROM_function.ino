void setBlackWhite(){
  setBalance();
  int eeAddress = 0;
  for(int i=0;i<3;++i){
    EEPROM.put(eeAddress,whiteArray[i]);
    eeAddress+=sizeof(float);
  }
  for(int i=0;i<3;++i){
    EEPROM.put(eeAddress,blackArray[i]);
    eeAddress+=sizeof(float);
  }
}
void readBlackWhite(){
  int eeAddress = 0;
  for(int i=0;i<3;++i){
    EEPROM.get(eeAddress,whiteArray[i]);
    eeAddress+=sizeof(float);
  } 
  for(int i=0;i<3;++i){
    EEPROM.get(eeAddress,blackArray[i]);
  }
  eeAddress+=sizeof(float);
}
