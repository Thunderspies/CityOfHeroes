#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	TeslaOffset
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		0.0 0.0 1.00
			StartJitter		0.0 0.0 0.50
			InitialVelocity		0.0 0.0 0.0375
			InitialVelocityJitter	0.0 0.0 0.0125
			PyrRotate		-90 0 0
			Scale			0.125 0.125 0.125
		End
	End

	Event
		EName	TeslaOffsetLeft
		Type	Local
		At	TeslaOffset
		BhvrOverride
			PositionOffset		1.0 0.0 0.0
			StartJitter		0.0 1.0 0.0
			InitialVelocityJitter	0.0 0.125 0.0
		End
	End

	Event
		EName	TeslaOffsetRight
		Type	Local
		At	TeslaOffset
		BhvrOverride
			PositionOffset		-1.0 0.0 0.0
			StartJitter		0.0 1.0 0.0
			InitialVelocityJitter	0.0 0.125 0.0
		End
		Part	:TechSword_LightningCore01.part
		Part	:TechSword_LightningGlow01.part
		pOther	TeslaOffsetLeft
	End
End

#########################################################

End