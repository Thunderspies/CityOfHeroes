#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	JuggleRig
		Type	Local
		At	Mystic
		Anim	FX_JuggleBalls_Female
	End
End

## BALLS ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Ball1
		Type	Local
		At	JuggleRig
		AnimPiv	Neck
		BhvrOverride
			StartColor		255 0 0
		End
		Part1	POWERS\DarknessControl\FearSkullrays2.part
	#	Part2	POWERS\ElectricityControl\ElectricityGlowStarSmall02.part
	#	Part3	POWERS\ElectricityControl\BodyArcsSmall03.part
	#	Part4	POWERS\ElectricityControl\GlowRingSmall2.part
		Geom	GEO_GenericBall_01
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Ball2
		Type	Local
		At	JuggleRig
		AnimPiv	Waist
		BhvrOverride
			StartColor	0 255 0
		End
		Part1	POWERS\DarknessControl\FearSkullrays2.part
	#	Part2	POWERS\ElectricityControl\ElectricityGlowStarSmall02.part
	#	Part3	POWERS\ElectricityControl\BodyArcsSmall03.part
	#	Part4	POWERS\ElectricityControl\GlowRingSmall2.part
		Geom	GEO_GenericBall_01
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Ball3
		Type	Local
		At	JuggleRig
		AnimPiv	Chest
		BhvrOverride
			StartColor	0 0 255
		End
		Part1	POWERS\DarknessControl\FearSkullrays2.part
	#	Part2	POWERS\ElectricityControl\ElectricityGlowStarSmall02.part
	#	Part3	POWERS\ElectricityControl\BodyArcsSmall03.part
	#	Part4	POWERS\ElectricityControl\GlowRingSmall2.part
		Geom	GEO_GenericBall_01
	End
End


#############################################################

End