#############################################################
## RockDebrisGoldExplode.fx
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
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			StartJitter		3 0 3
			Scale			0.75 0.75 0.75
			StartColor		255 255 255
			PrimaryTint		100
			SecondaryTint		0
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	1 0.5 1
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End
		Geom	Crystal_FXDebris01A
		Geom	Crystal_FXDebris02A
		Geom	Crystal_FXDebris03A
		Geom	Crystal_FXDebris04A
		Geom	Crystal_FXDebris05A
		Lifespan 75
		LifespanJitter 15
		CThresh 0.00001
	End
End

#############################################################

End