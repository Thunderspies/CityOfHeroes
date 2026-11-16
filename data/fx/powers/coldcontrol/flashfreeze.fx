#############################################################
## FX System Name
#############################################################

FxInfo

Lifespan	200

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		SOund	Chillingtouch3 100 30 .8
	End
End

Condition
	On	Time
	Time	10

	Event
		EName	Pri
		Type	start
		At	T_Root
		SOund	Coldblast2 100 30 .7
	End
End

Condition
	On	Time
	Time	0

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:snowburst.part
		Sound Chillingtouch3 100 100 .6
		LifeSpan	65
	End
		
	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:snowburst.part
		LifeSpan	65
	End
End

Condition
	On	Time
	Time	5
		
	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part1	:ColdArmMist.part
		LifeSpan	65		
	End

	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	:ColdArmMist.part
		LifeSpan	65
	End

	Event	
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part2	:FrostHands.part
		LifeSpan	67
	End
		
	Event	
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part2	:FrostHands.part
		LifeSpan	67
	End
End

#############################################################

#Condition
#	On	Time
#	Time	50
#	
#	Event
#		EName	Pri
#		Type	start
#		At	T_Root
#		
#		Part1	:FlashFreezeInsideMist2.part
#		part2	:FlashFreezeInsideMist.part
#		part1	:FlashFreezeMist.part
#		part3	:FlashFreezeMist2.part
#		
#		Lifespan	20
#	End
#End

#############################################################

#Condition
#	On	Cycle
#	Time	2
#
#	Event
#		EName	Pri
#		Type	start
#		At	splat
#		Bhvr	:FlashFreezeSplats.bhvr
#		Splat	mist2.tga
#		Lifespan	10
#	End
#End
#
#Condition
#	On	Cycle
#	Time	2
#
#	Event
#		EName	Pri
#		Type	start
#		At	splat
#		Bhvr	:FlashFreezeSplats.bhvr
#		Splat	Glow_03_funkycloud.tga
#		Lifespan	10
#	End
#End
#
#Condition
#	On	Cycle
#	Time	2
#
#	Event
#		EName	Pri
#		Type	start
#		At	splat
#		Bhvr	:FlashFreezeSplats.bhvr
#		Splat	mist2.tga
#		Lifespan	10
#	End
#End
#
#Condition
#	On	Cycle
#	Time	2
#
#	Event
#		EName	Pri
#		Type	start
#		At	splat
#		
#		Bhvr	:FlashFreezeSplats.bhvr
#		Splat	Glow_03_funkycloud.tga
#		Lifespan	10
#	End
#End
#
#Condition
#	On	Cycle
#	Time	2
#
#	Event
#		EName	Pri
#		Type	start
#		At	splat
#		
#		Bhvr	:FlashFreezeSplats.bhvr
#		Splat	mist2.tga
#		Lifespan	10
#	End
#End
#
#Condition
#	On	Cycle
#	Time	2
#
#	Event
#		EName	Pri
#		Type	start
#		At	splat
#		Bhvr	:FlashFreezeSplats.bhvr
#		Splat	Glow_03_funkycloud.tga
#		Lifespan	10
#	End
#End
#
#Condition
#	On	Time
#	Time	50
#	
#	Event
#		EName	splat
#		Type	start
#		At	T_Root
#		Splat	Splat_ElectricRing.tga	
#		Bhvr	:FlashFreezeRing.bhvr
#		LifeSpan	8
#	End
#End
#
#Condition
#	On	Time
#	Time	54
#
#	Event
#		EName	Pri
#		Type	start
#		At	T_Root
#		Part5	:FlashFreezeCubes.part
#		Lifespan	20
#	End
#End

#######################################################
##IceChunks
#######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot2
		Type	Local
		At	T_Root
		bhvr	behaviors/UniformScale.bhvr
		Geom	HotFeet01
		Lifespan	170
	End
End

Condition
	On	Time
	Time	53

	Event
		EName	geo
		Type	local
		At	Pivot2
		Altpiv	14
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_01
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo2
		Type	local
		At	Pivot2
		Altpiv	13
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_02
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	52
	Event
		EName	geo3
		Type	local
		At	Pivot2
		Altpiv	12
		bhvr	behaviors/IceChunks2.bhvr
		Geom	IceChunk_03
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo4
		Type	local
		At	Pivot2
		Altpiv	11
		bhvr	behaviors/IceChunks2.bhvr
		Geom	IceChunk_04
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	57

	Event
		EName	geo5
		Type	local
		At	Pivot2
		Altpiv	10
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_05
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	56

	Event
		EName	geo6
		Type	local
		At	Pivot2
		Altpiv	9
		bhvr	behaviors/IceChunks2.bhvr
		Geom	IceChunk_06
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo7
		Type	local
		At	Pivot2
		Altpiv	8
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_07
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	55
	Event
		EName	geo8
		Type	local
		At	Pivot2
		Altpiv	7
		bhvr	behaviors/IceChunks2.bhvr
		Geom	IceChunk_08
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	56
	Event
		EName	geo9
		Type	local
		At	Pivot2
		Altpiv	6
		bhvr	behaviors/IceChunks3.bhvr
		Geom	IceChunk_09
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo10
		Type	local
		At	Pivot2
		Altpiv	5
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_10
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	54

	Event
		EName	geo11
		Type	local
		At	Pivot2
		Altpiv	4
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_11
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo12
		Type	local
		At	Pivot2
		Altpiv	3
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_12
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo13
		Type	local
		At	Pivot2
		Altpiv	2
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_13
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	57
	
	Event
		EName	geo14
		Type	local
		At	Pivot2
		Altpiv	1
		bhvr	behaviors/IceChunks2.bhvr
		Geom	IceChunk_09
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End


#######################################################
##IceChunks2
#######################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Pivot1
		Type	start
		At	T_Root
		bhvr	behaviors/UniformScale.bhvr
		Geom	HotFeet	#HotFeet01
		#Part1	:CrystalSnowBurst.part
		Lifespan	170
	End
End

Condition
	On	Time
	Time	53

	Event
		EName	geo
		Type	local
		At	Pivot1
		Altpiv	1
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_01
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	55
	Event
		EName	geo2
		Type	local
		At	Pivot1
		Altpiv	2
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_02
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End


	Event
		EName	geo3
		Type	local
		At	Pivot1
		Altpiv	3
		bhvr	behaviors/IceChunks2.bhvr
		Geom	IceChunk_03
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo4
		Type	local
		At	Pivot1
		Altpiv	4
		bhvr	behaviors/IceChunks2.bhvr
		Geom	IceChunk_04
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo5
		Type	local
		At	Pivot1
		Altpiv	5
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_05
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	57

	Event
		EName	geo6
		Type	local
		At	Pivot1
		Altpiv	6
		bhvr	behaviors/IceChunks3.bhvr
		Geom	IceChunk_06
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo7
		Type	local
		At	Pivot1
		Altpiv	7
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_07
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	53

	Event
		EName	geo8
		Type	local
		At	Pivot1
		Altpiv	8
		bhvr	behaviors/IceChunks3.bhvr
		Geom	IceChunk_04
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo9
		Type	local
		At	Pivot1
		Altpiv	9
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_09
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	54

	Event
		EName	geo10
		Type	local
		At	Pivot1
		Altpiv	10
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_10
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo11
		Type	local
		At	Pivot1
		Altpiv	11
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_11
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	55
	Event
		EName	geo12
		Type	local
		At	Pivot1
		Altpiv	12
		bhvr	behaviors/IceChunks.bhvr
		Geom	IceChunk_12
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo13
		Type	local
		At	Pivot1
		Altpiv	13
		bhvr	behaviors/IceChunks3.bhvr
		Geom	IceChunk_13
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End
End

Condition
	On	Time
	Time	56

	Event
		EName	geo14
		Type	local
		At	Pivot1
		Altpiv	14
		bhvr	behaviors/IceChunks3.bhvr
		Geom	IceChunk_01
		Part1	:CrystalSnowBurst.part
		Lifespan	10
	End

End


End			