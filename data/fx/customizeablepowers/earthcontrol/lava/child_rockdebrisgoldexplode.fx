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
			Scale			1 1 1
			StartColor		255 240 192
			PrimaryTint		100
			SecondaryTint		0
			InitialVelocity		0.0 0.25 0.0
			InitialVelocityJitter	1 0.625 1
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
		End
		Geom	Earth_FXDebris01A
		Geom	Earth_FXDebris02A
		Geom	Earth_FXDebris03A
		Geom	Earth_FXDebris04A
		Geom	Earth_FXDebris05A
		Lifespan 75
		LifespanJitter 15
		CThresh 	0.00001
	End
End

#############################################################

End