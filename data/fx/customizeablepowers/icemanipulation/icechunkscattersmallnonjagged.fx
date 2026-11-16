#############################################################
## IceChunkScatterSmallNonJagged.fx
#############################################################

FxInfo
Lifespan 10

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Icezickle01
		Type	start
		At	Origin
		LifeSpan 	150
		LifeSpanJitter 	50

		BhvrOverride
			TintGeom		1
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.25 0.25 0.25

			Scale		0.15 0.15 0.15
		End

		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		CThresh 	0.000001

		Part1	POWERS\ColdControl\ColdStars.part

		geom	Tintable_FX_IceWall_02
		geom	Tintable_FX_IceWall_03
		geom	Tintable_FX_IceWall_07
		geom	Tintable_FX_IceWall_06
		geom	Tintable_FX_IceWall_10

	End

End

#############################################################

End
