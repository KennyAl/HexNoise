#pragma once

#include <assert.h>

namespace HexNoise


{
	/** Base class for every other noise module.
	* Used to connect different noise modules easily
	*/
	class HEXNOISE_API FModule
	{

	public:

		//
		// Begin Interface
		// 


		/** Every module needs to implement this function, whether it produces 1D or 3D does not matter.
		* This is because users are allowed to use a module to combine (for example add) the outputs of a 3D
		* and a 2D generator module. To allow this compatibility all modules have to have the same function implemented.
		*/
		virtual double GetNoise(double X, double Y = 0.0, double Z = 0.0) const = 0;

		/** Returns the number of sub modules this module supports */
		virtual int32 GetNumSubModules() const = 0;

		/** Returns the min value that this module can output */
		double GetMinValue();

		/** Returns the max value that this module can output */
		double GetMaxValue();

		//
		// End Interface
		//

		/** Constructor */
		FModule(int32 NumSubModules);

		/** Destructor */
		~FModule();

		/** Connects a sub module to this module
		* @param NewSubModule	Pointer to the new submodule
		* @param Index			The
		*/
		void SetSubModule(FModule& NewSubModule, int32 Index);

	protected:

		/** Checks all submodules for their min/max values and updates the ones of the caller accordingly.
		* Does not have to match the actual min/max values of the module, since it can modify the output 
		* of its sub modules!
		* Override this function, to implement logic module dependend logic and call FModule::UpdateMinMaxValues()
		* first if the your module has one or more submodules.
		* This gets called in GetMinValue() and GetMaxValue().
		*/
		virtual void UpdateMinMaxValues();

		/** Pointer to an array of pointers to submodules */
		FModule** SubModules;

		/** The min value that this module can output */
		double MinValue;

		/** The max value that this module can output */
		double MaxValue;
	};
}

