#############################################################
## Red_ElectricityControlHeavy.fx
#############################################################

FxInfo

Lifespan 40

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part	:Red_ElectricityControl01Large.part
		Part	:Red_ElectricityControl02Large.part
		Part	:Red_ElectricityControl01WhiteLarge.part
		Part	:Red_ElectricityControl02whiteLarge.part
		Part	:Red_ElectricityControl01.part
		Part	:Red_ElectricityControl02.part
		Part	:Red_ElectricityInnerGlow.part
		Sound elecattack1 70 70 0.76
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part	:Red_ElectricityControl01Large.part
		Part	:Red_ElectricityControl02Large.part
		Part	:Red_ElectricityControl01WhiteLarge.part
		Part	:Red_ElectricityControl02whiteLarge.part
		Part	:Red_ElectricityOuterGlow.part
		Part	:Red_ElectricityArch.part
		Part	:Red_ElectricityRing.part
		Part	:Red_ElectricityArchInvert.part
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