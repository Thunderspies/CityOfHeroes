#########################################################
## Electric Aura - Lighting Field
########################################################

FxInfo

########################################################

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	Origin
		bhvr	Behaviors\soundFade4.bhvr
		Sound elecArmor3_loop 70 70 0.34
		Flags	PowerLoopingSound
		Lifespan 350
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	WepR
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Sound lightningfield2 80 80 0.6
		LifeSpan 30
	End

	Event
		Type	local
		At	WepL
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		LifeSpan 30
	End
End

Condition
	On 	Time
	Time 	24

	Event
		Type	local
		At	HandL
		Part	:ElectricitySparkBurst.part
		LifeSpan 20
	End

	Event
		Type	local
		At	HandR
		Part	:ElectricitySparkBurst.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	root
		Part	:ChainLightningRing.part
		Part	:ChainLightningThinRing.part
	End
End

Condition
	On 	Time
	Time 	40

	Event
		Type	local
		At	root
		Part	:ChainLightningArchs.part
		Part	:ChainLightningArchs1.part
		Part	:ChainLightningArchs2.part
		Part	:ChainLightningArchs3.part
	End
End

Condition
	On 	cycle
	Time 	40

	Event
		Type	local
		At	root
		Part	:ChainLightningArchsSpinner.part
		Part	:ChainLightningRing.part
		Part	:ChainLightningThinRing.part
		LifeSpan 40
	End
End

#############################################################

End