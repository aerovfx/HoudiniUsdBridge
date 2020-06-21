/*
 * Copyright 2019 Side Effects Software Inc.
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
 *
 * Produced by:
 *      Side Effects Software Inc.
 *      123 Front Street West, Suite 1401
 *      Toronto, Ontario
 *      Canada   M5J 2M2
 *      416-504-9876
 *
 */

#ifndef __aovBuffer__
#define __aovBuffer__

#include <pxr/pxr.h>
#include <pxr/imaging/hd/renderBuffer.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/base/gf/vec4f.h>
#include <BRAY/BRAY_Interface.h>
#include <HUSD/XUSD_HydraRenderBuffer.h>
#include <SYS/SYS_AtomicInt.h>
#include <UT/UT_UniquePtr.h>

PXR_NAMESPACE_OPEN_SCOPE

class BRAY_HdAOVBuffer : public XUSD_HydraRenderBuffer
{
public:
    BRAY_HdAOVBuffer(const SdfPath &id);
    ~BRAY_HdAOVBuffer() override;

    bool                Allocate(const GfVec3i &dimensions,
				HdFormat format,
				bool multiSampled) override final;

    HdFormat            GetFormat() const override final;
    uint                GetDepth() const override final { return 1; }
    uint                GetWidth() const override final;
    uint                GetHeight() const override final;
    bool                IsMultiSampled() const override final
				{ return myMultiSampled; }

    void                *Map() override final;
    void                Unmap() override final;
    bool                IsMapped() const override final;

    bool                IsConverged() const override final;
    void		setConverged();
    void		clearConverged();

    void                Resolve() override final;

    int                 NumExtra() const override final;
    HdFormat            GetFormatExtra(int idx) const override final;
    const UT_StringHolder &GetPlaneName(int idx) const override final;
    void*               MapExtra(int idx) override final;
    void                UnmapExtra(int idx) override final;
    const UT_Options    &GetMetadata() const override final;

    bool		isValid() const { return myAOVBuffer.isValid(); }
    const BRAY::AOVBufferPtr	&aovBuffer() const { return myAOVBuffer; }
    void		setAOVBuffer(const BRAY::AOVBufferPtr &aov)
    {
	myAOVBuffer = aov;
    }

private:
    void                _Deallocate() override final;

    BRAY::AOVBufferPtr		myAOVBuffer;
    UT_UniquePtr<uint8_t[]>	myTempbuf;
    SYS_AtomicInt32		myConverged;
    int				myWidth, myHeight;
    HdFormat			myFormat;
    bool			myMultiSampled;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
