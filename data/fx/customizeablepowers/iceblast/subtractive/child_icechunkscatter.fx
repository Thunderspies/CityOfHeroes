#############################################################
## IceChunkScatter.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part1	:IceBoltMist.part
		LifeSpan 5
	End
End

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			Alpha			150
			TintGeom		1
		End
		Part1	:ColdStars.part
		geom	tintable_Dark_FX_IceWall_01
		geom	tintable_Dark_FX_IceWall_02
		geom	tintable_Dark_FX_IceWall_03
		geom	tintable_Dark_FX_IceWall_04
		geom	tintable_Dark_FX_IceWall_05
		geom	tintable_Dark_FX_IceWall_06
		geom	tintable_Dark_FX_IceWall_07
		geom	tintable_Dark_FX_IceWall_08
		geom	tintable_Dark_FX_IceWall_09
		geom	tintable_Dark_FX_IceWall_10
		geom	tintable_Dark_FX_IceWall_11
		geom	tintable_Dark_FX_IceWall_12
		CThresh 	0.000001
		LifeSpan 	150
		LifeSpanJitter 	50
	End
End

#############################################################

End