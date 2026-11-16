
FxInfo
Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	root
		LifeSpan 	15
		Part1	:IceBoltMist.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		4
	RepeatJitter	1

	Event
		Type	Start
		At	root
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part1	:ColdStars.part
		geom	Tintable_FX_IceWall_01
		geom	Tintable_FX_IceWall_02
		geom	Tintable_FX_IceWall_03
		geom	Tintable_FX_IceWall_04
		geom	Tintable_FX_IceWall_05
		geom	Tintable_FX_IceWall_06
		geom	Tintable_FX_IceWall_07
		geom	Tintable_FX_IceWall_08
		geom	Tintable_FX_IceWall_09
		geom	Tintable_FX_IceWall_10
		geom	Tintable_FX_IceWall_11
		geom	Tintable_FX_IceWall_12
	End
End

Condition
	On 	Time
	Time 	0
	Repeat 		2
	RepeatJitter	1

	Event
		Type	Start
		At	root
		LifeSpan 	75
		LifeSpanJitter 	30
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		Part1	:ColdStars.part
		Part2	:IceCrystal.part
	End

	Event
	#	hardwareOnly
		Type	Start
		At	root
		LifeSpan 	150
		LifeSpanJitter 	50
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part1	:ColdStars.part
		geom	Tintable_FX_IceWall_01
		geom	Tintable_FX_IceWall_02
		geom	Tintable_FX_IceWall_03
		geom	Tintable_FX_IceWall_04
		geom	Tintable_FX_IceWall_05
		geom	Tintable_FX_IceWall_06
		geom	Tintable_FX_IceWall_07
		geom	Tintable_FX_IceWall_08
		geom	Tintable_FX_IceWall_09
		geom	Tintable_FX_IceWall_10
		geom	Tintable_FX_IceWall_11
		geom	Tintable_FX_IceWall_12
	End
End

End