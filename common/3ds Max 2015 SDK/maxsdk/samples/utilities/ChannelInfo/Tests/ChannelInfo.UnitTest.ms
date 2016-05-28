--##################################################
-- Modifiers Skin Unit Test
-- Author: Peter Watje
-- Created:	March 2013
--##################################################
--##################################################
fileIn "MxsUnit/MxsUnit.ms"

struct Obj_Prop_UnitTest
(
private


	--##################################################
	-- Helper functions
	--##################################################


	function validateProperties experimental expected =
	(
		if (experimental.count != expected.count) then
		(
			assert_equal expected.count experimental.count message:"Incorrect number of properties"
		)
		else
		(
			for i in 1 to experimental.count do
			(
				assert_equal expected[i] experimental[i] message:("Incorrect Value for " + testprops[i])
			)
		)
	),

	function changeValue valueName pvalue properties =
	(
		for i in 1 to properties.count do
		(
			if testprops[i] == valueName do
			(
				properties[i] = pvalue
				return true
			)
		)
		local str = stringstream ""
		format "Value(%): %, not found in property array" valueName pvalue to:str
		assert_true false message:(str as string)
	),

	--##################################################
	-- Unit tests
	--##################################################
public
	function setup =
	(
	),

	function teardown =
	(
		
	),
/*
	Regression test for  MAXX-11450 Crash with Channel Info Utility when clearing channels on Patch
	Patches dont support alpha and illum channels
	Fix : dont show those channels
*/
	function channelInfo_MAXX_11450 =
	(
		
		Teapot radius:34.6551 smooth:on segs:4 body:on handle:on spout:on lid:on mapcoords:on pos:[-5.22664,4.12463,0] isSelected:on
		macros.run "Modifier Stack" "Convert_to_Patch"
		macros.run "Tools" "Channel_Info"	
		channelInfo.ClearChannel $ 0
		channelInfo.ClearChannel $ 1
		channelInfo.ClearChannel $ 0
				
	),


	Tests =
	#(
		channelInfo_MAXX_11450
	)
)

/*
foo = Obj_Prop_UnitTest()
foo.setup()
foo.channelInfo_MAXX_11450()
*/

run_test_fixture Obj_Prop_UnitTest script:(getThisScriptFilename())