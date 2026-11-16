#############################################################
## ChainInduction_Attack.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	HandR
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityControl01WhiteLarge.part
		Part	:ElectricityControl02whiteLarge.part
		Part	:ElectricityRing.part
		Sound elecattack1 70 70 0.76
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:ElectricityHandRing2.part
		Part	:ElectricityHandRingThin.part
	End
End

#############################################################

End