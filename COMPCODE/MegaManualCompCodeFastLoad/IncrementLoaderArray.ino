void incrementLoaderArray(){
  int length = strlen(currentBlocks);
  for (int i=0;i<length-1;i++){
    currentBlocks[i] = currentBlocks[i+1];
  }
  currentBlocks[length-1] = 'E';
}