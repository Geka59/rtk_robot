// функции печати в сериал порт //

void debug_print(){
  Serial.print(xleft);
  Serial.print('|');
  Serial.print(yleft);
  Serial.print('|');
  Serial.print(xright);
  Serial.print('|');
  Serial.print(yright);
  Serial.print('|');
  Serial.print(a_button);
  Serial.print('|');
  Serial.print(b_button);
  Serial.print('|');
  Serial.print(x_button);
  Serial.print('|');
  Serial.print(y_button);
  Serial.print('|');
  Serial.print(dpad_right);
  Serial.print('|');
  Serial.print(dpad_up);
  Serial.print('|');
  Serial.print(dpad_down);
  Serial.print('|');
  Serial.print(right_shoulder);
  Serial.print('|');
  Serial.print(left_shoulder);
  Serial.print('|');
  Serial.print(r_trigger_index_pos);
  Serial.print('|');
  Serial.print(l_trigger_index_pos);
  Serial.println();
  
  
  
  
  
  
  
  
  
  
  
  }
