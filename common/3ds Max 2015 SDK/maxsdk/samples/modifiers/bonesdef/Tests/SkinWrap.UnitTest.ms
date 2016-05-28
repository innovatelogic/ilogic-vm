--##################################################
-- Modifiers Skin Wrap Unit Test
-- Author: Qinming Ren
-- Created:	April 2013
--##################################################
--##################################################
fileIn "MxsUnit/MxsUnit.ms"

struct Skin_Wrap_UnitTest
(
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
	Regression test for  MAXX_11852
	Max script index issue
*/
	function skinwarp_MAXX_11852 =
	(
		
		obj=Teapot radius:40 smooth:on segs:4 body:on handle:on spout:on lid:on mapcoords:on pos:[0,0,0] isSelected:on
			
		rm = RayMeshGridIntersect ()  
		rm.Initialize 100 
		rm.addNode obj 	 
		rm.buildGrid ()
		theGSMesh = snapshotasmesh obj
		forceCompleteRedraw()
		
		hits=0

		vpos = getVert theGSMesh 2
		hits= rm.intersectSphere  vpos 30
		assert_true (hits == 296)
		
		vpos1 = getVert theGSMesh 2
		vpos2 = getVert theGSMesh 128
		hits = rm.intersectRay vpos1 (vpos2-vpos1) false
		assert_true (hits == 1)
		
		hits = rm.intersectRay vpos1 (vpos2-vpos1) true
		assert_true (hits == 8)
		
		hits = rm.intersectSegment vpos1 vpos2 false
		assert_true (hits == 1)
		
		hits = rm.intersectSegment vpos1 vpos2 true
		assert_true (hits == 8)
		
		vpos1 = getVert theGSMesh 387
		vpos2 = getVert theGSMesh 165
		hits = rm.intersectBox vpos1 vpos2
		assert_true (hits == 235)
	),


	Tests =
	#(
		skinwarp_MAXX_11852
	)
)

/*
foo = Obj_Prop_UnitTest()
foo.setup()
foo.skinwarp_MAXX_11852()
*/

run_test_fixture Skin_Wrap_UnitTest script:(getThisScriptFilename())