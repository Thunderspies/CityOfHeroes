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
			StartColor		255 255 255
			Scale			0.75 0.75 0.75
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.5
			physSFriction 	 	0.99
			physDFriction 	 	0.9
			TintGeom		1
		End
		Geom	Lava_FXDebris01A
		Geom	Lava_FXDebris02A
		Geom	Lava_FXDebris03A
		Geom	Lava_FXDebris04A
		Geom	Lava_FXDebris05A
		Lifespan	75
		LifespanJitter	15
		CThresh 	0.00001
	End
End

#############################################################

End