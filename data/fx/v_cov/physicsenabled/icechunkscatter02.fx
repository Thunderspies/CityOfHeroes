
FxInfo

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		LifeSpan 	5
		#Sound	Ice7 100 80 .6
	#	Part1	\POWERS\ColdControl\IceBoltMist.part
	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Icezickle01
		Type	start
		At	Origin
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		CThresh 	0.000001
		Part1	\POWERS\ColdControl\ColdStars.part
		geom	FX_IceWall_01
		geom	FX_IceWall_02
		geom	FX_IceWall_03
		geom	FX_IceWall_04
		geom	FX_IceWall_05
		geom	FX_IceWall_06
		geom	FX_IceWall_07
		geom	FX_IceWall_08
		geom	FX_IceWall_09
		geom	FX_IceWall_10
		geom	FX_IceWall_11
		geom	FX_IceWall_12
	End


End
