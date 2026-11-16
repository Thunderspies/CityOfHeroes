#############################################################
## RockDebrisGold.fx
#############################################################

FxInfo

LifeSpan 400

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	origin
		Bhvr	V_COV\PhysicsEnabled\sparkUP.bhvr
		BhvrOverride
			StartColor		255 240 192
			Scale			1 1 1
			PositionOffset		0 1 0
			StartJitter		0 2 0
			SpinJitter		0.5 0.5 0.5
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
		LifespanJitter 90
		CThresh 	0.00001
	End
End

#############################################################

End