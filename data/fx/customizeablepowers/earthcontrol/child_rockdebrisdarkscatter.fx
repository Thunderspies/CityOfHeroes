#############################################################
## RockDebrisDarkScatter.fx
#############################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			Scale			0.875 0.875 0.875
			StartColor		255 255 255
			PrimaryTint		33
			SecondaryTint		66
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
		Lifespan	75
		LifespanJitter	15
		CThresh 	0.00001
	End
End

#############################################################

End