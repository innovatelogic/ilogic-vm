--##################################################
-- Modifiers Skin Morph Unit Test
-- Author: Peter Watje
-- Created:	March 2013
--##################################################
-- The following macros are testing the changes
-- made to the modifier UI in Kahn. For more
-- information view spec #1159b
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
	Regression test for  MAXX_11745
	Morpher performance issue
	Fix : the morpher was calling ui and sending notfiy messages during modifier which it should not
*/
	function morph_MAXX_11745 =
	(
		Sphere radius:24.6729 smooth:on segs:32 chop:0 slice:off sliceFrom:0 sliceTo:0 mapcoords:on recenter:off pos:[-24.9893,24.561,0] isSelected:on
		Sphere radius:22.5079 smooth:on segs:32 chop:0 slice:off sliceFrom:0 sliceTo:0 mapcoords:on recenter:off pos:[37.1411,-8.16643,0] isSelected:on
		macros.run "Modifier Stack" "Convert_to_Poly"			
		
		Sphere radius:16.1261 smooth:on segs:32 chop:0 slice:off sliceFrom:0 sliceTo:0 mapcoords:on recenter:off pos:[108.698,-36.6727,0] isSelected:on
		macros.run "Modifier Stack" "Convert_to_Mesh"
		Sphere radius:5.29694 smooth:on segs:32 chop:0 slice:off sliceFrom:0 sliceTo:0 mapcoords:on recenter:off pos:[174.969,-27.7785,0] isSelected:on
		macros.run "Modifier Stack" "Convert_to_Mesh"			
		Sphere radius:14.9287 smooth:on segs:32 chop:0 slice:off sliceFrom:0 sliceTo:0 mapcoords:on recenter:off pos:[200.603,-69.7415,0] isSelected:on
		macros.run "Modifier Stack" "Convert_to_Mesh"
		
		max modify mode
		modPanel.addModToSelection (Bend ()) ui:on
		$.modifiers[#Bend].BendAngle = 46.5
		sliderTime = 30f
		set animate on
		$.modifiers[#Bend].BendAngle = -36.5
				
		forcecompleteredraw()
		
		select $Sphere001
		m = Morpher ()
		modPanel.addModToSelection (m) ui:on
		WM3_MC_BuildFromNode $Sphere001.morpher 1 $Sphere003
		WM3_MC_BuildFromNode $Sphere001.morpher 2 $Sphere004
		WM3_MC_BuildFromNode $Sphere001.morpher 3 $Sphere005

		WM3_MC_SetValue $Sphere001.morpher 1 100.0
		WM3_MC_SetValue $Sphere001.morpher 2 100.0
		WM3_MC_SetValue $Sphere001.morpher 3 100.0

		select $Sphere002
		modPanel.addModToSelection (m) ui:on
		
		forcecompleteredraw()
		for i = 1 to 30 do
		(
			sliderTime = i
		)
		
	),


	Tests =
	#(
		morph_MAXX_11745
	)
)

/*
foo = Obj_Prop_UnitTest()
foo.setup()
foo.morph_MAXX_11745()
*/

run_test_fixture Obj_Prop_UnitTest script:(getThisScriptFilename())