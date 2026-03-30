void incrementLoaderArray(){
  int length = strlen(currentBlocks);
  for (int i=length-1;i>0;i--){
    currentBlocks[i] = currentBlocks[i-1];
  }
  currentBlocks[0] = 'E';
}