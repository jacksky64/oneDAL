/* file: TrainingResultId.java */
/*******************************************************************************
* Copyright 2014-2020 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
 * @ingroup neural_networks_training
 * @{
 */
package com.intel.daal.algorithms.neural_networks.training;

import java.lang.annotation.Native;

import com.intel.daal.utils.*;
/**
 * <a name="DAAL-CLASS-ALGORITHMS__NEURAL_NETWORKS__TRAINING__TRAININGRESULTID"></a>
 * \brief Available identifiers of result of the neural network model based training
 */
public final class TrainingResultId {
    /** @private */
    static {
        LibUtils.loadLibrary();
    }

    private int _value;

    /**
     * Constructs the training result object identifier using the provided value
     * @param value     Value corresponding to the training result object identifier
     */
    public TrainingResultId(int value) {
        _value = value;
    }

    /**
     * Returns the value corresponding to the training result object identifier
     * @return Value corresponding to the training result object identifier
     */
    public int getValue() {
        return _value;
    }

    @Native private static final int modelId = 0;

    public static final TrainingResultId model = new TrainingResultId(modelId); /*!< Neural network Model */
}
/** @} */
