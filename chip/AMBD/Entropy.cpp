/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2018 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *          Provides implementations for the CHIP entropy sourcing functions
 *          on the AmebaD platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <support/crypto/CHIPRNG.h>

using namespace ::chip;

namespace chip {
namespace DeviceLayer {
namespace Internal {

namespace {

extern int rtw_get_random_bytes(void* dst, u32 size);

int GetEntropy(uint8_t * buf, size_t bufSize)
{
    rtw_get_random_bytes(buf, bufSize);
    return 0;
}

} // unnamed namespace

CHIP_ERROR InitEntropy()
{
    CHIP_ERROR err;
    unsigned int seed;

    // Initialize the source used by Chip to get secure random data.
    err = ::chip::Platform::Security::InitSecureRandomDataSource(GetEntropy, 64, NULL, 0);
    SuccessOrExit(err);

    // Seed the standard rand() pseudo-random generator with data from the secure random source.
    err = ::chip::Platform::Security::GetSecureRandomData((uint8_t *) &seed, sizeof(seed));
    SuccessOrExit(err);
    srand(seed);
    printf("srand seed set: %u", seed);

exit:
    if (err != CHIP_NO_ERROR)
    {
        printf(TAG, "InitEntropy() failed: err=%d", err);
    }
    return err;
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
