#############################################################
## Red_ChargedArmor.fx
#############################################################

FxInfo

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
		At	Hips
		Part1	Powers\ElectricityControl\Red_ElectricitySparkShieldRing.part
		Sound Elecbuildup2 60 60 .73
	End
End

Condition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		Sound elecarmor1_loop 60 60 .37
		bhvr	behaviors\soundFade5.bhvr
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
		Part	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
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
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingz.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2z.part
		Part1	Powers\ElectricityControl\Red_ElectricityArchContinuingzWhite.part
		Part2	Powers\ElectricityControl\Red_ElectricityArchContinuing2zWhite.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkBigGlow.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkStar.part
		Part	Powers\ElectricityControl\Red_ElectricitySparkRing.part
		lifespan 7
	End
End

#############################################################

End