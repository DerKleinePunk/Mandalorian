/*
 * MemoryStream
 * Copyright (C) 2016-2022 Sofian Audry -- info(@)sofianaudry(.)com
 * 
 * This library is mainly used for debugging purposes. It writes and reads
 * from a single/common buffer, allowing to test your functions and algorithms.
 * 
 * Example use:
 * <code>
 * MemoryStream stream;
 * stream.write(42);
 * int val = stream.read(); // <-- variable "val" will contain value 42
 * </code>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#ifndef MEMORY_STREAM_H_
#define MEMORY_STREAM_H_

#include <Arduino.h>

#ifndef MEMORY_STREAM_BUFFER_SIZE
#define MEMORY_STREAM_BUFFER_SIZE 128
#endif

class MemoryStream : public Stream
{
public:
  MemoryStream();

  // Stream methods
  virtual int available() { return length; }
  
  virtual int read();
  
  virtual int peek();
  
  virtual void flush() { }
  
  virtual size_t write(uint8_t c);
  
  virtual size_t write(const uint8_t *buffer, size_t size);
  void clear();
     
private:
  // Circular buffer containing the information.
  uint8_t buffer[MEMORY_STREAM_BUFFER_SIZE];
  
  // Current write position in the circular buffer.
  int writePos;
  
  // Current read position in the circular buffer.
  int readPos;
  
  // Currentl length of the data available in buffer.
  int length;
};

#endif

