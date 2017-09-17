#include <iostream>
#include <vector>
#include <stdint.h>

#include "gtest/gtest.h"

namespace
{
  const uint8_t Start1Value = 0x55;
  const uint8_t Start2Value = 0xDA;
}

struct SDataFrame
{
  uint8_t Data[255];
  ssize_t DataSize;
};

ssize_t StreamToDataPackage(uint8_t* data, ssize_t len, SDataFrame& frame)
{
  bool StartFound = false;
  uint8_t FrameSize = 0;
  ssize_t FrameLocation = 0;

  // Find the header of the frame
  // 0x55 0xDA <size>
  for(ssize_t i = 0; i<len; i++)
  {
    if((len - i) >= 3)
    {
      if(data[i] == Start1Value && data[i+1] == Start2Value)
      {
        StartFound = true;
        FrameSize = data[i+2];
        FrameLocation = i+3;
        break;
      }
    }
  }

  // Found the header now look of the required data is already 
  // in the buffer
  if(StartFound)
  {
    if(len-FrameLocation >= FrameSize)
    {
      uint8_t* framePtr = data + FrameLocation;
      frame.DataSize = FrameSize;
      memcpy(frame.Data, framePtr, FrameSize);

      return FrameLocation + FrameSize;
    }
  }

  return 0;
}


int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  int ret = RUN_ALL_TESTS();
  return ret;
}

class CStreamToDataPackage : public ::testing::Test
{
protected:

};

TEST_F(CStreamToDataPackage, SingleFrame)
{
  std::vector<uint8_t> data{0x55, 0xDA, 0x02, 0x01, 0x02};
  SDataFrame retFrame;
  
  ssize_t ret = StreamToDataPackage(data.data(), data.size(), retFrame);

  EXPECT_EQ(5, ret);
  EXPECT_EQ(2, retFrame.DataSize);
  EXPECT_EQ(0x01, retFrame.Data[0]);
  EXPECT_EQ(0x02, retFrame.Data[1]);
}

TEST_F(CStreamToDataPackage, MultiFrame)
{
  std::vector<uint8_t> data{0x55, 0xDA, 0x02, 0x01, 0x02, 0x55, 0xDA, 0x01, 0x10};
  SDataFrame retFrame1;
  SDataFrame retFrame2;
  
  ssize_t ret = StreamToDataPackage(data.data(), data.size(), retFrame1);
  ASSERT_EQ(ret, 5);
  
  data.erase(data.begin(), data.begin()+ret);

  ret = StreamToDataPackage(data.data(), data.size(), retFrame2);
  EXPECT_EQ(ret, 4);

  EXPECT_EQ(2, retFrame1.DataSize);
  EXPECT_EQ(0x01, retFrame1.Data[0]);
  EXPECT_EQ(0x02, retFrame1.Data[1]);

  EXPECT_EQ(1, retFrame2.DataSize);
  EXPECT_EQ(0x10, retFrame2.Data[0]);
}

TEST_F(CStreamToDataPackage, IncompleteFrame)
{
  std::vector<uint8_t> data{0x55, 0xDA, 0x02, 0x01};
  SDataFrame retFrame;
  
  ssize_t ret = StreamToDataPackage(data.data(), data.size(), retFrame);
  EXPECT_EQ(0, ret);

  data.push_back(0x02);

  ret = StreamToDataPackage(data.data(), data.size(), retFrame);

  EXPECT_EQ(5, ret);
  EXPECT_EQ(2, retFrame.DataSize);
  EXPECT_EQ(0x01, retFrame.Data[0]);
  EXPECT_EQ(0x02, retFrame.Data[1]);
}

TEST_F(CStreamToDataPackage, InvalidPreData)
{
  std::vector<uint8_t> data{0xAA, 0x55, 0xDA, 0x02, 0x01, 0x02};
  SDataFrame retFrame;
  
  ssize_t ret = StreamToDataPackage(data.data(), data.size(), retFrame);

  EXPECT_EQ(6, ret);
  EXPECT_EQ(2, retFrame.DataSize);
  EXPECT_EQ(0x01, retFrame.Data[0]);
  EXPECT_EQ(0x02, retFrame.Data[1]);
}