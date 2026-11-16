#############################################################
## IceChunkLargeScatter.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	:IceBoltMist.part
		LifeSpan 5
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
		Bhvr	V_COV\PhysicsEnabled\IceChunk.bhvr
		BhvrOverride
			TintGeom	1
		End
		Part	:ColdStars.part
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
		CThresh 	0.000001
		CDestroy 	1
	End
End

#############################################################

End