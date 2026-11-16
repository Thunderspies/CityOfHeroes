#############################################################
## Inferno.fx
#############################################################

FxInfo

Lifespan 60

#############################################################


Condition
	On 		Time
	Time 		0
	Repeat		3
	RepeatJitter	2

	Event
		Type	start
		At	Origin
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0.0 0.5 0.0
			Scale			1.2 1.2 1.2
			InitialVelocity		0.0 0.6 0.0
			InitialVelocityJitter	0.2 0.2 0.2
			StartColor		255 240 192
			PyrRotateJitter		180 180 180
			Gravity			0.04
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
		Part	:Dust_RockBreak.part
		Lifespan 500
		LifespanJitter	90
	#	CThresh 	0.00001
	End
End
#############################################################

End