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
		Sound eleccage_loop 70 70 .2
		bhvr	behaviors\soundFade1.bhvr
		Flags	PowerLoopingSound
		Lifespan 170
	End
End

Condition
	On	Time
	Time	0

#	Event
#		Type	Local
#		At	Chest
#		Part1	CustomizeablePowers\ElectricalBlast\ElectricityGlow.part
#		Part2	CustomizeablePowers\ElectricalBlast\ElectricityGlowStar.part
#		Part3	CustomizeablePowers\ElectricalBlast\ElectricityArch4a.part
#		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchSmalla.part
#		Sound	elecballhit2 50 30 .33
#		Lifespan 21
#	End

	Event
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArchContinuing2.part
	End
End

#############################################################

End