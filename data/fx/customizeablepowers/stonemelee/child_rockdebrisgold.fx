#############################################################
## RockDebrisGold.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\sparkUP.bhvr
		BhvrOverride
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End
		Part	CustomizeablePowers\StoneMelee\RockDebrisGoldLarge.part
		Lifespan 75
		LifespanJitter 15
		CThresh 	0.00001
	End
End

#############################################################

End