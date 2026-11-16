#############################################################
## CHILD_BigFeathers_Steady.fx
#############################################################

FxInfo

############################################################

Condition
	On 		Time
	Time		0
	Random		1
	Event
		Ename	BigFeather
		At	Origin
		Type	Start

		Geom	LightFeather_Tintable
		Geom	BentFeather_Tintable

		Bhvr	behaviors\FadeIn.bhvr
		BhvrOverride
			Alpha			255
			FadeInLength		50
			Scale			.55 .55 .55
			SpinJitter		.15 .25 .1
			InitialVelocityJitter	0.05 0.05 0.05
			StartJitter		1.2 2 1.2
			PyrRotateJitter		180 180 180
			Drag			0.01
			Gravity			0.002
			TintGeom		1
		End
		Lifespan 	100
		LifespanJitter	10
	End

	Event
		Ename	SmallFeather
		At	Origin
		Type	Start

		Geom	LightFeather_Tintable
		Geom	BentFeather_Tintable

		Bhvr	behaviors\FadeIn.bhvr
		BhvrOverride
			Alpha			255
			FadeInLength		50

			Scale			.35 .35 .35
			SpinJitter		.15 .25 .1
			InitialVelocityJitter	0.05 0.05 0.05
			StartJitter		1.2 2 1.2
			PyrRotateJitter		180 180 180
			Drag			0.01
			Gravity			0.002
			TintGeom		1
		End
		Lifespan 	100
		LifespanJitter	10
	End
End

#############################################################

End