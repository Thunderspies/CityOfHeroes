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
		Part1	CustomizeablePowers\IceControl\snowburst.part
		Sound Chillingtouch3 100 100 .6
		LifeSpan	65
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceControl\snowburst.part
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
		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
		LifeSpan	65
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\IceControl\ColdArmMist.part
		LifeSpan	65
	End

	Event
		Ename   LeftHandMist
		Type	Local
		At	WepL
		Part2	CustomizeablePowers\IceControl\FrostHands.part
		LifeSpan	67
	End

	Event
		Ename   RightHandMist
		Type	Local
		At	WepR
		Part2	CustomizeablePowers\IceControl\FrostHands.part
		LifeSpan	67
	End
End


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
		bhvr	CustomizeablePowers\IceControl\UniformScale.bhvr
		BhvrOverride
			TintGeom	1
		End
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
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_01
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo2
		Type	local
		At	Pivot2
		Altpiv	13
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_02
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_03
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo4
		Type	local
		At	Pivot2
		Altpiv	11
		bhvr	CustomizeablePowers\IceControl\IceChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_04
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_05
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_06
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo7
		Type	local
		At	Pivot2
		Altpiv	8
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_07
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_08
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_09
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo10
		Type	local
		At	Pivot2
		Altpiv	5
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_10
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_11
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo12
		Type	local
		At	Pivot2
		Altpiv	3
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_12
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo13
		Type	local
		At	Pivot2
		Altpiv	2
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_13
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_09
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\UniformScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	HotFeet	#HotFeet01
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
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_01
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_02
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End


	Event
		EName	geo3
		Type	local
		At	Pivot1
		Altpiv	3
		bhvr	CustomizeablePowers\IceControl\IceChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_03
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo4
		Type	local
		At	Pivot1
		Altpiv	4
		bhvr	CustomizeablePowers\IceControl\IceChunks2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_04
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo5
		Type	local
		At	Pivot1
		Altpiv	5
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_05
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_06
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo7
		Type	local
		At	Pivot1
		Altpiv	7
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_07
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_04
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo9
		Type	local
		At	Pivot1
		Altpiv	9
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_09
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_10
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo11
		Type	local
		At	Pivot1
		Altpiv	11
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_11
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_12
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

	Event
		EName	geo13
		Type	local
		At	Pivot1
		Altpiv	13
		bhvr	CustomizeablePowers\IceControl\IceChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_13
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
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
		bhvr	CustomizeablePowers\IceControl\IceChunks3.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_IceChunk_01
		Part1	CustomizeablePowers\IceControl\CrystalSnowBurst.part
		Lifespan	10
	End

End


End