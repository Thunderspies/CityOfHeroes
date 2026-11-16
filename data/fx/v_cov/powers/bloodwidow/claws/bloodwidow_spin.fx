#############################################################
## Blood Widow "Spin" FX
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ClawSpikeR
		Type	local
		At	WepR
		Bhvr	V_COV\Powers\BloodWidow\Claws\WidowSpikes_FadeUp.bhvr
		Geom	Arachnos_Blade_R
		LifeSpan 60
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	WepR
		Part1	Powers\Claws\ClawStreak.part
		Sound Clawspin 100 100 1.0
		Lifespan 23
	End
End

Condition
	On 	Time
	Time 	34

	Event
		EName	Streakburst
		Type	local	#start
		At	WepR
		part1	Powers\Claws\ClawEmbers.part
		part2	Powers\Claws\ClawEmbersBroad.part
		Lifespan 16
	End
End

Condition
	On 	Time
	Time 	35

	Event
		EName	Ring
		Type	start
		At	root
		Part5	Powers\Claws\ClawsRing.part
		Lifespan 15
	End
End

#############################################################

End