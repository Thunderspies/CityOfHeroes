
FxInfo

Condition
	On 	Time
	Time 	0

	Event
		Type	Start 
		At	Origin
		LifeSpan 	75
		LifeSpanJitter 	30
		Bhvr	V_COV\PhysicsEnabled\IceCubes.bhvr
		BhvrOverride
			#InitialVelocityJitter	0.2 0.25 0.2
			PositionOffset		0 1 0
			Gravity			.04
			physGravity		1
			physRestitution 	0.6
			physSFriction 	 	0.9
			physDFriction 	 	0.9
		End
		Part2	:SaltCrystal.part
	End

End
