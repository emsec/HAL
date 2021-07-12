//  MIT License
//
//  Copyright (c) 2019 Ruhr University Bochum, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2021 Max Planck Institute for Security and Privacy. All Rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "hal_core/netlist/boolean_function.h"
#include "hal_core/netlist/gate_library/enums/async_set_reset_behavior.h"
#include "hal_core/netlist/gate_library/gate_type_component/gate_type_component.h"

namespace hal
{
    class GateType;

    class FFComponent : public GateTypeComponent
    {
    public:
        FFComponent(std::unique_ptr<GateTypeComponent> component, const BooleanFunction& next_state_bf, const BooleanFunction& clock_bf);

        ComponentType get_type() const override;
        static bool is_class_of(const GateTypeComponent* component);

        std::set<GateTypeComponent*> get_components(const std::function<bool(const GateTypeComponent*)>& filter = nullptr) const override;

        BooleanFunction get_next_state_function() const;
        void set_next_state_function(const BooleanFunction& next_state_bf);

        BooleanFunction get_clock_function() const;
        void set_clock_function(const BooleanFunction& clock_bf);

        BooleanFunction get_async_reset_function() const;
        void set_async_reset_function(const BooleanFunction& async_reset_bf);

        BooleanFunction get_async_set_function() const;
        void set_async_set_function(const BooleanFunction& async_set_bf);

        const std::pair<AsyncSetResetBehavior, AsyncSetResetBehavior>& get_async_set_reset_behavior() const;
        void set_async_set_reset_behavior(const AsyncSetResetBehavior behav_state, const AsyncSetResetBehavior behav_neg_state);

    private:
        static constexpr ComponentType m_type = ComponentType::ff;
        std::unique_ptr<GateTypeComponent> m_component;

        BooleanFunction m_next_state_bf;
        BooleanFunction m_clock_bf;
        BooleanFunction m_async_reset_bf;
        BooleanFunction m_async_set_bf;
        std::pair<AsyncSetResetBehavior, AsyncSetResetBehavior> m_async_set_reset_behavior = {AsyncSetResetBehavior::undef, AsyncSetResetBehavior::undef};
    };
}    // namespace hal