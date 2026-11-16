
FxInfo

Condition
	On 	Time
	Time 	0

	Event
		Type	Start 
		At	Origin
		LifeSpan 	5
		#Sound	Ice7 100 80 .6
#		Part1	\POWERS\ColdControl\IceBoltMist.part
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

		BhvrOverride
			
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	0.25 0.25 0.25

			Scale		0.15 0.15 0.15
		End

		Bhvr	V_COV\PhysicsEnabled\IceChunkSmall.bhvr
		CThresh 	0.000001
		
		Part1	POWERS\ColdControl\ColdStars.part

		geom	FX_IceWall_02
		geom	FX_IceWall_03
		geom	FX_IceWall_07
		geom	FX_IceWall_06
		geom	FX_IceWall_10

	End

End
