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
	Regression test for  MAXX_11732
	Skin Morph cannot change topology, but if you call loop on a mnmesh and the vertices 
	are not order this will cause a topology change.
	Fix : make a clone of the mnmesh and call loop on that and then transfer the selection back
*/
	function skinMorph_MAXX_11732 =
	(
		obj = Cylinder smooth:on heightsegs:5 capsegs:1 sides:18 height:41.9411 radius:11.4527 mapcoords:on pos:[39.1992,-52.0927,0] isSelected:on
		convertTo $ PolyMeshObject
			
		max modify mode
		modPanel.addModToSelection (Skin_Morph ()) ui:on
		modPanel.setCurrentObject $.modifiers[#Skin_Morph]
		subobjectLevel = 1
			
		forcecompleteredraw()
			
		$.modifiers[#Skin_Morph].useSoftSelection = on		
		$.modifiers[#Skin_Morph].skinMorphOps.selectVertices $ #{87..88}
		$.modifiers[#Skin_Morph].skinMorphOps.loop ()
		$.modifiers[#Skin_Morph].skinMorphOps.loop ()
		$.modifiers[#Skin_Morph].skinMorphOps.selectVertices $ #{87..88}
		$.modifiers[#Skin_Morph].skinMorphOps.ring ()
		$.modifiers[#Skin_Morph].skinMorphOps.ring ()

		
		forcecompleteredraw()
	),


	Tests =
	#(
		skinMorph_MAXX_11732
	)
)

/*
foo = Obj_Prop_UnitTest()
foo.setup()
foo.skinMorph_MAXX_11732()
*/

run_test_fixture Obj_Prop_UnitTest script:(getThisScriptFilename())