#############################################################
## Heal_AOE.fx
#############################################################

FxInfo
LifeSpan	75

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:ChestHeal.part
		LifeSpan 55
	End

	Event
		Type	Local
		At	Chest
		Part	:HealFire.part
		LifeSpan 50
	End

	Event
		Type	Local
		At	Chest
		Part	:ChestHealMetal.part
		Part	:ChestHealMetalInvert.part
		Part	:ChestHealMetal.part
		Part	:ChestHealMetalInvert.part
		LifeSpan 45
	End

	Event
		Type	Local
		At	head
		Part	:HealFire.part
		LifeSpan 50
	End

	Event
		Type	Local
		At	SpadR
		Part	:HealFire.part
		LifeSpan 50
	End


	Event
		Type	Local
		At	SpadL
		Part	:HealFire.part
		LifeSpan 50
	End

	Event
		Type	Local
		At	UarmR
		Part	:HealFire.part
		LifeSpan 50
	End


	Event
		Type	Local
		At	UarmL
		Part	:HealFire.part
		LifeSpan 50
	End

	Event
		Type	Local
		At	Hips
		Part	:HealFirehips.part
		LifeSpan 50
	End


	Event
		Type	Local
		At	LarmR
		Part	:HealFire.part
		LifeSpan 50
	End


	Event
		Type	Local
		At	LarmL
		Part	:HealFire.part
		LifeSpan 50
	End
End

#############################################################

End