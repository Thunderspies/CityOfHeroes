#############################################################
## Red_ElectricityBoltHitContinuing.fx
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
		Sound eleccage_loop 70 70 .2
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 390
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part1	POWERS\ElectricityControl\Red_ElectricityGlow.part
		Part2	POWERS\ElectricityControl\Red_ElectricityGlowStar.part
		Part3	POWERS\ElectricityControl\Red_ElectricityArch4a.part
		Part4	POWERS\ElectricityControl\Red_ElectricityArchSmalla.part
		Sound elecballhit2 50 50 .33
		Lifespan 21
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part1	POWERS\ElectricityControl\Red_ElectricityArchContinuing.part
		Part2	POWERS\ElectricityControl\Red_ElectricityArchContinuing2.part
		Part1	POWERS\ElectricityControl\Red_ElectricityArchContinuing.part
		Part2	POWERS\ElectricityControl\Red_ElectricityArchContinuing2.part
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Chest
		Part1	POWERS\ElectricityControl\Red_ElectricityArchContinuing.part
		Part2	POWERS\ElectricityControl\Red_ElectricityArchContinuing2.part
		Part1	POWERS\ElectricityControl\Red_ElectricityArchContinuing.part
		Part2	POWERS\ElectricityControl\Red_ElectricityArchContinuing2.part
	End
End

#############################################################

End
