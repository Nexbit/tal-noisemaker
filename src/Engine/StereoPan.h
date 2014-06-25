/*
	==============================================================================
	This file is part of Tal-NoiseMaker by Patrick Kunz.

	Copyright(c) 2005-2010 Patrick Kunz, TAL
	Togu Audio Line, Inc.
	http://kunz.corrupt.ch

	This file may be licensed under the terms of of the
	GNU General Public License Version 2 (the ``GPL'').

	Software distributed under the License is distributed
	on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
	express or implied. See the GPL for the specific language
	governing rights and limitations.

	You should have received a copy of the GPL along with this
	program. If not, go to http://www.gnu.org/licenses/gpl.html
	or write to the Free Software Foundation, Inc.,  
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
	==============================================================================
 */

#if !defined(__StereoPan_h)
#define __StereoPan_h

#include "AudioUtils.h"

class StereoPan 
{
public:
    float modulationAmount;

    AudioUtils audioUtils;

    StereoPan() 
    {
        this->modulationAmount = 1.0f;
    }

    inline void setModulationAmount(float value)
    {
        this->modulationAmount = value;
    }

    inline void process(float *sampleL, float *sampleR, float lfoValue) 
    {
        if (this->modulationAmount != 0.0f)
        {
            this->modulationAmount = fabs(this->modulationAmount);
            // There are better ways of making a panning (-3dB)
            lfoValue = 0.5f * (lfoValue + 1.0f);
            *sampleL *= 1.0f - lfoValue * this->modulationAmount;
            *sampleR *= 1.0f - (1.0f - lfoValue) * this->modulationAmount;
        }
    }
};

#endif
