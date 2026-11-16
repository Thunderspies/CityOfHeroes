#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		BhvrOverride
			PyrRotateJitter		90 90 90
			Scale			0.25 0.25 0.25
			PositionOffset		0 0 0
			StartJitter		1 1 1
			InitialVelocity		0.0 0.2 0.0
			InitialVelocityJitter	0.65 0.25 0.65
			Spin			0.0 0.0 0.0
			SpinJitter		0.2 0.2 0.2
			FadeOutLength 		30
			Gravity			0.04
			Drag			0.01
			physics 		1
			physRadius 		0.3
			physGravity 		0.8
			physRestitution 	0.25
			physSFriction 	 	0.9
			physDFriction 	 	0.9
			TintGeom		1
		End
		CThresh 	0.000001
		Geom	Tintable_Spines_Debris
		Lifespan 60
	End
End

#############################################################

End