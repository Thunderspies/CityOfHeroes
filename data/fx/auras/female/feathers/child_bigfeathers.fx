#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

#Lifespan 500

############################################################

Condition

	On 		Cycle
	Time		10
	Repeat		1
	RepeatJitter	3
	Chance		0.35

	Event
		Ename	BigFeather
		At	Origin
		Type	Start

		Geom	LightFeather_Tintable
		Geom	BentFeather_Tintable



		Bhvr	behaviors\Braz\FeatherPluck.bhvr
		BhvrOverride
			Alpha			255
			Scale			.8 .8 .8
			SpinJitter		.15 .15 .15
			InitialVelocity		.0 0.1 .0
			InitialVelocityJitter	0.2 0.12 0.2
			StartJitter		1.2 2 1.2
			PyrRotateJitter		180 180 180
			Drag			0.01
			Gravity			0.02
			TintGeom		1
		End

		Lifespan	150
		LifespanJitter	50
	End
End

#############################################################

End