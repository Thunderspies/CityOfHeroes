#############################################################
## Red_ChargedArmor.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		#Part1	:ElectricitySparkShieldRing.part
		Sound Elecbuildup2 60 60 0.73
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		bhvr	behaviors\soundFade5.bhvr
		Sound elecarmor1_loop 60 60 0.37
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

#############################################################

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	LarmL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	LarmR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	UarmL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	UarmR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	LlegL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	LlegR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.3

	Event
		Type	Local
		At	Hips
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	FootL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	FootR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	WepL
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	WepR
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

Condition
	On 	cycle
	Time 	3
	Chance	0.1

	Event
		Type	Local
		At	Head
		Part	:ElectricityArchContinuingz.part
		Part	:ElectricityArchContinuing2z.part
		Part	:ElectricitySparkBigGlow.part
		Part	:ElectricitySparkStar.part
		Part	:ElectricitySparkRing.part
		lifespan 7
	End
End

#############################################################

End