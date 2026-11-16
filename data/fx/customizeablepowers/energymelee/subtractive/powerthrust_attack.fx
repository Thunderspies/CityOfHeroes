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
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part3	CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchRings2.part
		Part4	CustomizeablePowers\EnergyMelee\Subtractive\LargepowerpunchRingsExpanding.part
		Sound PPenergytrans4 180 180 0.7
	End
End

#############################################################

End