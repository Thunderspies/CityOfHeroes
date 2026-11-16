#############################################################
## ElectricityBoltHitContinuing.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	chest
		Sound eleccage_loop 70 70 0.2
		bhvr	behaviors\soundFade1.bhvr
		Flags	PowerLoopingSound
		Lifespan 170
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
	End
End

#############################################################

End