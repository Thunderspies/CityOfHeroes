#########################################################
##	PhysicsRing
##
FxInfo

LifeSpan 120

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Root

		BhvrOverride
			PositionOffset		0.0 3.0 0.0
			physics 		1
			physRadius 		0.1
			physGravity 		1.5
			physRestitution 	0.1
			physSFriction 	 	.9
			physDFriction 	 	.9
		End
		Lifespan	90
		LifespanJitter 	20

		CEvent 	:VolcanicGasAttack.fx
		CThresh 	.001
		CDestroy 	0
	End

End

Condition
	On 	Time
	Time 	300

	Event
		EName	All
		Type	Destroy

	End


End


End
