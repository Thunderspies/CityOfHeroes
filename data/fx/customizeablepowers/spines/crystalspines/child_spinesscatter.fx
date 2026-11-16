#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		BhvrOverride
			Scale			0.375 0.375 0.375
			PyrRotateJitter		90 90 90
			StartJitter		1 1 1
			InitialVelocity		0.0 0.2 0.0
			InitialVelocityJitter	0.65 0.25 0.65
			Spin			0.0 0.0 0.0
			SpinJitter		0.2 0.2 0.2
			FadeOutLength 		30
			Gravity			0.04
			Drag			0.01
			physics 		1
			physGravity 		0.8
			physRestitution 	0.75
			physSFriction 	 	0.75
			physDFriction 	 	0.75
			TintGeom		1
		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
		Geom	MetalSpine_Debris
	End
End

#############################################################

End