#########################################################
##	FireAura
FxInfo

##################################
##Sound
##########################################

Condition
	On	Time
	Time	6
	
	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
	End
End


Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	Origin
		Sound firesword_loop 50 50 .22
	End
End
###############################################################################

Condition
	On	Time
	Time	10
	
	Event
		Type	Local
		At	T_Chest
		Part	:ChestHeal.part
		LifeSpan	25
	End
	
	Event
		Type	Local
		At	T_Chest
		Part	:HealFire.part
		LifeSpan	20
	End
	
	Event
		Type	Local
		At	T_Chest
		Part	:ChestHealMetal.part
		Part	:ChestHealMetalInvert.part
		Part	:ChestHealGlowsGreenMetal.part
		Part	:ChestHealGlowsGreenMetalInvert.part
		LifeSpan	15
	End

	Event
		Type	Local
		At	T_head
		Part	:HealFire.part
		LifeSpan	20
	End

	Event
		Type	Local
		At	T_SpadR
		Part	:HealFire.part
		LifeSpan	20
	End


	Event
		Type	Local
		At	T_SpadL
		Part	:HealFire.part
		LifeSpan	20
	End

	Event
		Type	Local
		At	T_UarmR
		Part	:HealFire.part
		LifeSpan	20
	End


	Event
		Type	Local
		At	T_UarmL
		Part	:HealFire.part
		LifeSpan	20
	End
	
	Event
		Type	Local
		At	T_Hips
		Part	:HealFirehips.part
		LifeSpan	20
	End


	Event
		Type	Local
		At	T_LarmR
		Part	:HealFire.part
		LifeSpan	20
	End


	Event
		Type	Local
		At	T_LarmL
		Part	:HealFire.part
		LifeSpan	20
	End

End

End				