
 
void FindPeaks(){
  
  //Signal = analogRead(SensorPin);
  Signal = 0.8 * S + 0.2 * analogRead(SensorPin);
  
  sampleCounter += 10;
  int N = sampleCounter - lastBeatTime;

  //Thresholdの選定
  if(Signal < thresh && Signal < T && N > (IBI/5)*3){
    T = Signal;
  }
  
  //Peaksの選定
  if(Signal > thresh && Signal > P && N > (IBI/5)*3){
    P = Signal;
  }

  //前回のIBIを参照してそれの3/5より大きい場合
  if ((N > 300) && (N > (IBI/5)*3)){
    if ( (Signal > thresh) && (Pulse == false) && (P > Signal) ){
      Pulse = true;
      IBI = sampleCounter - lastBeatTime;
      lastBeatTime = sampleCounter;  
      next += 1;
     }
  }

  //peakを検知して閾値を下回ったら、Thresholdを更新
  if (Signal < thresh && Pulse == true){
    Pulse = false;

    Serial.print(P);
    Serial.print(" ");
    Serial.print(T);
    Serial.print(" ");
    
    amp = P - T;
    thresh = amp/2 + T;
    Serial.print(thresh);
    Serial.print(" ");
    Serial.println(IBI);
    P = thresh;
    T = thresh;
  }
 
  if (N > 2500){
    thresh = 1600;
    P = 1024;
    T = 2048;
    lastBeatTime = sampleCounter;
  }

  S = Signal;
}
