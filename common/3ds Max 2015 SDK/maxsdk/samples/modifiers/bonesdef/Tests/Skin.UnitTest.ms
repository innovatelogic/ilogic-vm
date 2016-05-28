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
	Regression test for  MAXX_11350
	Max script index issue
	Fix : the code as thinking the index was 1 based, set it to zero base
*/
	function skin_MAXX_11350 =
	(
		
		Cylinder smooth:on heightsegs:5 capsegs:1 sides:18 height:57.0311 radius:11.2255 mapcoords:on pos:[-1.18015,0.751877,0] isSelected:on
		Dummy transform:(matrix3 [1,0,0] [0,0,1] [0,-1,0] [-0.879765,0,1.46628]) isSelected:on
		Dummy transform:(matrix3 [1,0,0] [0,0,1] [0,-1,0] [-0.879765,-1.2434e-006,28.4457]) isSelected:on
		Dummy transform:(matrix3 [1,0,0] [0,0,1] [0,-1,0] [-0.293255,-2.3458e-006,53.6657]) isSelected:on

		select $Cylinder001
		max modify mode
		modPanel.addModToSelection (Skin ()) ui:on
		skinOps.addBone $.modifiers[#Skin] $Dummy001 1
		skinOps.addBone $.modifiers[#Skin] $Dummy002 1
		skinOps.addBone $.modifiers[#Skin] $Dummy003 1
		
		forceCompleteRedraw()

		iret = false
			
		skinOps.rigidVertex $.modifiers[#Skin] 1 false 
		iret = skinOps.isRigidVertex $.modifiers[#Skin] 1 
		assert_true (iret == 0)
			
		skinOps.rigidVertex $.modifiers[#Skin] 1 true 
		iret = skinOps.isRigidVertex $.modifiers[#Skin] 1 
		assert_true (iret == 1)

		skinOps.rigidVertex $.modifiers[#Skin] 2 false 
		iret = skinOps.isRigidVertex $.modifiers[#Skin] 2 
		assert_true (iret == 0)

		skinOps.rigidVertex $.modifiers[#Skin] 2 true 
		iret = skinOps.isRigidVertex $.modifiers[#Skin] 2 
		assert_true (iret == 1)
	
		
		skinOps.rigidHandle $.modifiers[#Skin] 1 false 
		iret = skinOps.isRigidHandle $.modifiers[#Skin] 1 
		assert_true (iret == 0)

		skinOps.rigidHandle $.modifiers[#Skin] 1 true 
		iret = skinOps.isRigidHandle $.modifiers[#Skin] 1 
		assert_true (iret == 1)

		skinOps.rigidHandle $.modifiers[#Skin] 2 false 
		iret = skinOps.isRigidHandle $.modifiers[#Skin] 2 
		assert_true (iret == 0)

		skinOps.rigidHandle $.modifiers[#Skin] 2 true 
		iret = skinOps.isRigidHandle $.modifiers[#Skin] 2 
		assert_true (iret == 1)
			
			
			
		skinOps.UnNormalizeVertex $.modifiers[#Skin] 1 false 
		iret = skinOps.isUnNormalizeVertex $.modifiers[#Skin] 1 
		assert_true (iret == 0)

		skinOps.UnNormalizeVertex $.modifiers[#Skin] 1 true 
		iret = skinOps.isUnNormalizeVertex $.modifiers[#Skin] 1 
		assert_true (iret == 1)

		skinOps.UnNormalizeVertex $.modifiers[#Skin] 2 false 
		iret = skinOps.isUnNormalizeVertex $.modifiers[#Skin] 2 
		assert_true (iret == 0)

		skinOps.UnNormalizeVertex $.modifiers[#Skin] 2 true 
		iret = skinOps.isUnNormalizeVertex $.modifiers[#Skin] 2 			
		assert_true (iret == 1)
			
		
		
	),


	Tests =
	#(
		skin_MAXX_11350
	)
)

/*
foo = Obj_Prop_UnitTest()
foo.setup()
foo.skin_MAXX_11350()
*/

run_test_fixture Obj_Prop_UnitTest script:(getThisScriptFilename())