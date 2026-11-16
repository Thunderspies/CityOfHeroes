#############################################################
## Child_IceChunkScatterSmallNonJagged.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		BhvrOverride
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.25 0.25 0.25
			Scale			0.15 0.15 0.15
			TintGeom		1
		End
		geom	tintable_FX_IceWall_02
		geom	tintable_FX_IceWall_03
		geom	tintable_FX_IceWall_07
		geom	tintable_FX_IceWall_06
		geom	tintable_FX_IceWall_10
		Part1	:ColdStars.part
		CThresh 	0.000001
		LifeSpan 	150
		LifeSpanJitter 	50
	End
End

#############################################################

End