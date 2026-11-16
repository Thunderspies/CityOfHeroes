#############################################################
## RockDebrisGoldScatter.fx
#############################################################

FxInfo

LifeSpan 390

#############################################################

Condition
	On 	Time
	Time 	0
	Repeat 2
	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			Scale			1 1 1
			StartJitter		10 0 10
			StartColor		255 240 192
			InitialVelocity		0.0 .5 0.0
			InitialVelocityJitter	0.5 0.2 0.5
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
		Lifespan 300
		LifespanJitter	90
		CThresh 	0.00001
	End
End

#############################################################

End