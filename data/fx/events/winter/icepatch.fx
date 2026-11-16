#########################################################
##	cot_waterfall_med.fx
##
FxInfo

###########################################################
######################## Audio #################################
###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pri
		Type	start
		At	Root
		
		Sound IcePatch_loop 80 80 .77

	End


End

###########################################################
######################## Fx ###################################
###########################################################

Condition
	On	Time
	Time	0

	Event
		EName	splat
		Type	start
		At	Root
		Splat	Splat_ElectricRing.tga	
		Bhvr	:IcePatchRings.bhvr
		LifeSpan	8
		
	End

End

Condition
	On	Time
	Time	17
	

	Event
		EName	Pri
		Type	start
		At	Root
		
#		Part4	:IcePatchRing2.part
		
		Splat	Splat_ElectricRing.tga	
		Bhvr	:IcePatchRings.bhvr
		LifeSpan	1

	End

End

###########################################################

Condition
	On	Time
	Time	20
	

	Event
		EName	Pri
		Type	start
		At	Root
				
		Splat	IceSlick.tga	
		Bhvr	:IcePatchSplat.bhvr

	End



End

Condition
	On	cycle
	Time	25
	Chance	.9

	Event
		EName	Pri
		Type	start
		At	Root
	
		Bhvr	:IcePatchRingsSmall.bhvr
		Splat	Splat_ElectricRing.tga	

		LifeSpan	15
	End

End

#################################################################3

Condition
	On	Time
	Time	7
	

	Event
		EName	Pri
		Type	start
		At	Root
		
		Part1	:IcePatchSparklies.part
		part2	:IcePatchInsideMist.part
		

	End


End


Condition
	On	Time
	Time	11
	

	Event
		EName	Pri
		Type	start
		At	Root
		
		part1	:IcePatchMist.part
		part3	:IcePatchMist2.part
		

	End


End


#######################################################
##IceChunks
#######################################################

Condition
	On	Time
	Time	15

	
	Event
		EName	geo
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_01
		
	End

	Event
		EName	geo2
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks4.bhvr
		Geom	IceChunk_02
		
	End


	Event
		EName	geo3
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_03
		
	End

	Event
		EName	geo4
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_04
		
	End

	Event
		EName	geo5
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_05
		
	End


	Event
		EName	geo6
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_06
		
	End

	Event
		EName	geo7
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_07
		
	End


	Event
		EName	geo8
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_08
		
	End


	Event
		EName	geo9
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_09
		
	End

	Event
		EName	geo10
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_10
		
	End

	Event
		EName	geo11
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_11
		
	End


	Event
		EName	geo12
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_12
		
	End

	Event
		EName	geo13
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_13
		
	End
End

#######################################################
##IceChunks set two
#######################################################

Condition
	On	Time
	Time	22

	
	Event
		EName	geo
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_01
		
	End

	Event
		EName	geo2
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks4.bhvr
		Geom	IceChunk_02
		
	End


	Event
		EName	geo3
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_03
		
	End

	Event
		EName	geo4
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_04
		
	End

	Event
		EName	geo5
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_05
		
	End


	Event
		EName	geo6
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_06
		
	End

	Event
		EName	geo7
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_07
		
	End


	Event
		EName	geo8
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_08
		
	End


	Event
		EName	geo9
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_09
		
	End

	Event
		EName	geo10
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_10
		
	End

	Event
		EName	geo11
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_11
		
	End


	Event
		EName	geo12
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_12
		
	End

	Event
		EName	geo13
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_13
		
	End
End


#######################################################
##IceChunks set three
#######################################################

Condition
	On	Time
	Time	30

	
	Event
		EName	geo
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_01
		
	End

	Event
		EName	geo2
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks4.bhvr
		Geom	IceChunk_02
		
	End


	Event
		EName	geo3
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_03
		
	End

	Event
		EName	geo4
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_04
		
	End

	Event
		EName	geo5
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_05
		
	End


	Event
		EName	geo6
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_06
		
	End

	Event
		EName	geo7
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_07
		
	End


	Event
		EName	geo8
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_08
		
	End


	Event
		EName	geo9
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_09
		
	End

	Event
		EName	geo10
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_10
		
	End

	Event
		EName	geo11
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks3.bhvr
		Geom	IceChunk_11
		
	End


	Event
		EName	geo12
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks.bhvr
		Geom	IceChunk_12
		
	End

	Event
		EName	geo13
		Type	start
		At	Root
		bhvr	behaviors/Winter_IcePatchChunks2.bhvr
		Geom	IceChunk_13
		
	End
End

###################################################################################

Condition
	On	Time
	Time	35


Event
		EName	Pri
		Type	start
		At	Root
				
		Part5	:IcePatchCubes.part
	End


End



End			