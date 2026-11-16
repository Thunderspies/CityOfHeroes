#############################################################
## ElectricityBoltHitContinuing.fx
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
		At	chest
		Sound	eleccage_loop 70 60 .2
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
	End

	Event
		Type	Local
		At	Chest
		Part	:Continuing_Lightning.part
		Part	:Continuing_Lightning_Glow.part

		Part1	powers\ElectricityControl\ElectricityArchContinuing.part
		Part2	powers\ElectricityControl\ElectricityArchContinuing2.part
		Sound	elecballhit2 50 30 .33

	End
End

#############################################################

End