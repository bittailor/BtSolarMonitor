#ifndef INC_I_Publisher_H
#define INC_I_Publisher_H

#include <cstddef>

#include "MeasurementRecord.h"

#define MAX_TOPIC_LENGTH 100

class I_Publisher {
public:
    virtual ~I_Publisher(){}
    
    virtual void publish(MeasurementRecord pRecords[], size_t pLength) = 0;       
};

#endif 