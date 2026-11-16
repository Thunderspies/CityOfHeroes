#############################################################
## Hit_Body_Stunner.FX
#############################################################

FxInfo
Lifespan 120

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound Particle_Static_Loop 100 100 .25
	End
End

#############################################################

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	LarmL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	LarmR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	UarmL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	UarmR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	LlegL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	LlegR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	Hips
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	FootL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	FootR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	WepL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	WepR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.2

	Event
		Type	Local
		At	Head
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part

		lifespan 7
	End
End

#############################################################

End