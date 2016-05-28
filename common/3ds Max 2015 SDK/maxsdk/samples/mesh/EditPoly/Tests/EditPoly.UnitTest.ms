--##################################################
-- Modifiers Edit Poly Unit Test
-- Author: Peter Watje
-- Created:	April 2013
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
	Regression test for  MAXX_10215
	Max script index issue
	Fix : the code as thinking the index was 1 based, set it to zero base
*/
	function editPoly_MAXX_10215 =
	(
		
		Sphere radius:28.0464 smooth:on segs:32 chop:0 slice:off sliceFrom:0 sliceTo:0 mapcoords:on recenter:off transform:(matrix3 [1,0,0] [0,0,1] [0,-1,0] [-60.5568,1.08518e-006,-24.826]) isSelected:on
		max modify mode
		modPanel.addModToSelection (Uvwmap ()) ui:on
		$.modifiers[#UVW_Map].mapChannel = 4
		max create mode
		Sphere radius:28.8749 smooth:on segs:32 chop:0 slice:off sliceFrom:0 sliceTo:0 mapcoords:on recenter:off transform:(matrix3 [1,0,0] [0,0,1] [0,-1,0] [42.4594,1.34887e-006,-30.8585]) isSelected:on
		select #($Sphere002, $Sphere001)
		macros.run "Modifier Stack" "Convert_to_Poly"
		select $Sphere002
		modPanel.addModToSelection (Edit_Poly ()) ui:on
		$.modifiers[#Edit_Poly].Attach $Sphere001 editPolyNode:$			
		
		
	),
	
	function editPoly_MAXX_12834_01 =
	(		
		Plane length:329.895 width:539.251 transform:(matrix3 [1,0,0] [0,0,1] [0,-1,0] [-327.807,-2.96242e-006,67.7722]) isSelected:on lengthsegs:2 widthsegs:2
		max modify mode
		macros.run "Modifier Stack" "Convert_to_Poly"
		subobjectLevel = 4
		$.EditablePoly.SetSelection #Face #{4}
		$.EditablePoly.SetSelection #Face #{4}
		$.preserveUVs = on
		$.EditablePoly.SetSelection #Face #{4}
		$.outlineAmount = -34.8927
		$.EditablePoly.Outline ()
		$.EditablePoly.SetSelection #Face #{1}
		$.insetAmount = 51.9074
		$.EditablePoly.buttonOp #inset
		subobjectLevel = 0		
		
		epsilon = 0.00001
		
		tmesh = snapshotAsMesh selection[1]
		
		meshVal = gettVert tmesh 1 
		correctVal = [0,0,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 2 
		correctVal = [0.5,0,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 3 
		correctVal = [1,0,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 4 
		correctVal = [0,0.5,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 5 
		correctVal = [0.564706,0.605769,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 6 
		correctVal = [0.935294,0.605769,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 7 
		correctVal = [0,1,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 8 
		correctVal = [0.564706,0.894231,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 9 
		correctVal = [0.935294,0.894231,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 10 
		correctVal = [0.0962583,0.359654,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 11 
		correctVal = [0.0962583,0.157345,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 12 
		correctVal = [0.417304,0.140601,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 13 
		correctVal = [0.465434,0.428801,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		
	),
	function editPoly_MAXX_12834_02 =
	(		
		max create mode
		Plane length:527.85 width:717.757 transform:(matrix3 [1,0,0] [0,0,1] [0,-1,0] [-170.234,-3.91671e-006,89.6038]) isSelected:on
		max modify mode
		modPanel.addModToSelection (Edit_Poly ()) ui:on
		subobjectLevel = 4
		$.modifiers[#Edit_Poly].SetSelection #Face #{}
		$.modifiers[#Edit_Poly].Select #Face #{4}
		$.modifiers[#Edit_Poly].preserveUVs = on
		$.modifiers[#Edit_Poly].SetOperation #Outline
		$.modifiers[#Edit_Poly].outlineAmount = -68.785
		$.modifiers[#Edit_Poly].Commit ()
		$.modifiers[#Edit_Poly].SetSelection #Face #{}
		$.modifiers[#Edit_Poly].Select #Face #{1}
		$.modifiers[#Edit_Poly].SetOperation #Inset
		$.modifiers[#Edit_Poly].insetAmount = 76.2617
		$.modifiers[#Edit_Poly].Commit ()


		epsilon = 0.00001
		
		tmesh = snapshotAsMesh selection[1]
		
		meshVal = gettVert tmesh 1 
		correctVal = [0,0,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 2 
		correctVal = [0.5,0,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 3 
		correctVal = [1,0,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 4 
		correctVal = [0,0.5,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 5 
		correctVal = [0.595833,0.630312,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 6 
		correctVal = [0.904167,0.630312,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 7 
		correctVal = [0,1,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 8 
		correctVal = [0.595833,0.869688,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 9 
		correctVal = [0.904167,0.869688,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 10 
		correctVal = [0.10625,0.376904,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 11 
		correctVal = [0.10625,0.144476,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 12 
		correctVal = [0.411473,0.131349,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		meshVal = gettVert tmesh 13 
		correctVal = [0.474261,0.45739,0] 
		l = Length(meshVal-correctVal) 
		assert_true (l < epsilon) 
		
		
	),

	Tests =
	#(
		editPoly_MAXX_10215,
		editPoly_MAXX_12834_01,
		editPoly_MAXX_12834_02
	)
		
	
)

/*
foo = Obj_Prop_UnitTest()
foo.setup()
foo.editPoly_MAXX_12834_01()
foo.editPoly_MAXX_12834_02()
*/

run_test_fixture Obj_Prop_UnitTest script:(getThisScriptFilename())



