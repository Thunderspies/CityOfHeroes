#############################################################
## PowerThrust_Attack.fx
#############################################################

FxInfo

Lifespan 50

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	WepR
		#Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch1.part
		#Part2	CustomizeablePowers\EnergyMelee\Largepowerpunch2.part
		#Part3	CustomizeablePowers\EnergyMelee\LargepowerpunchGlow.part
		#Part1	CustomizeablePowers\EnergyMelee\Largepowerpunch3.part
		Part3	CustomizeablePowers\EnergyMelee\LargepowerpunchRings2.part
		Part4	CustomizeablePowers\EnergyMelee\LargepowerpunchRingsExpanding.part
		Sound PPenergytrans4 180 180 0.7
	End
End

#############################################################

End