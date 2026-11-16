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
		Sound eleccage_loop 70 70 .2
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
		Part1	powers\ElectricityControl\ElectricityGlow.part
		Part2	powers\ElectricityControl\ElectricityGlowStar.part
		Part3	powers\ElectricityControl\ElectricityArch4a.part
		Part4	powers\ElectricityControl\ElectricityArchSmalla.part
		Sound elecballhit2 50 50 .33
		Lifespan 21
	End

	Event
		Type	Local
		At	Chest
		Part1	powers\ElectricityControl\ElectricityArchContinuing.part
		Part2	powers\ElectricityControl\ElectricityArchContinuing2.part
	End
End

#############################################################

End