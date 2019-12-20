// ----------------------------------------------------------------------------
//  OgmaNeo
//  Copyright(c) 2016-2019 Ogma Intelligent Systems Corp. All rights reserved.
//
//  This copy of OgmaNeo is licensed to you under the terms described
//  in the OGMANEO_LICENSE.md file included in this distribution.
// ----------------------------------------------------------------------------

#include "Helpers.h"

using namespace ogmaneo;

DoubleBuffer ogmaneo::createDoubleBuffer(
    ComputeSystem &cs,
    cl_int size
) {
    DoubleBuffer db;

    db[_front] = cl::Buffer(cs.getContext(), CL_MEM_READ_WRITE, size);
    db[_back] = cl::Buffer(cs.getContext(), CL_MEM_READ_WRITE, size);

    return db;
}

void ogmaneo::writeBufferToStream(ComputeSystem &cs, std::ostream &os, cl::Buffer &buf, int size) {
    std::vector<unsigned char> data(size);
    cs.getQueue().enqueueReadBuffer(buf, CL_TRUE, 0, size, data.data());
    os.write(reinterpret_cast<const char*>(data.data()), size);
}

void ogmaneo::readBufferFromStream(ComputeSystem &cs, std::istream &is, cl::Buffer &buf, int size) {
    std::vector<unsigned char> data(size);
    buf = cl::Buffer(cs.getContext(), CL_MEM_READ_WRITE, size);
    is.read(reinterpret_cast<char*>(data.data()), size);
    cs.getQueue().enqueueWriteBuffer(buf, CL_TRUE, 0, size, data.data());
}

void ogmaneo::writeDoubleBufferToStream(ComputeSystem &cs, std::ostream &os, DoubleBuffer &buf, int size) {
    std::vector<unsigned char> data(size);
    cs.getQueue().enqueueReadBuffer(buf[_front], CL_TRUE, 0, size, data.data());
    os.write(reinterpret_cast<const char*>(data.data()), size);
    cs.getQueue().enqueueReadBuffer(buf[_back], CL_TRUE, 0, size, data.data());
    os.write(reinterpret_cast<const char*>(data.data()), size);
}

void ogmaneo::readDoubleBufferFromStream(ComputeSystem &cs, std::istream &is, DoubleBuffer &buf, int size) {
    std::vector<unsigned char> data(size);
    buf = createDoubleBuffer(cs, size);
    is.read(reinterpret_cast<char*>(data.data()), size);
    cs.getQueue().enqueueWriteBuffer(buf[_front], CL_TRUE, 0, size, data.data());
    is.read(reinterpret_cast<char*>(data.data()), size);
    cs.getQueue().enqueueWriteBuffer(buf[_back], CL_TRUE, 0, size, data.data());
}