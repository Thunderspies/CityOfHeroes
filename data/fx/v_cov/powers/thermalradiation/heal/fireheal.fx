#########################################################
##	FireAura
FxInfo

LifeSpan	55

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
		#Sound	firesword_loop 50 30 .22
	End
End
###############################################################################

Condition
	On	Time
	Time	15
	
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		Part1	:FireHandz.part
		#Sound	Firehand_loop 100.0 30.0 .55
		LifeSpan	20
	
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		LifeSpan	20
	
	End

	Event
		Type	Local
		At	T_Chest
		Part	:ChestHeal.part
		Sound cauterize 70 70 .95
		LifeSpan	35
	End
	
	Event
		Type	Local
		At	T_Chest
		Part	:HealFire.part
		Sound buildup 70 70 .36
		LifeSpan	30
	End
	
	Event
		Type	Local
		At	T_Chest
		Part	:ChestHealMetal.part
		Part	:ChestHealMetalInvert.part
		Part	:ChestHealGlowsGreenMetal.part
		Part	:ChestHealGlowsGreenMetalInvert.part
		LifeSpan	25
	End

	Event
		Type	Local
		At	T_head
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	T_SpadR
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	T_SpadL
		Part	:HealFire.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	T_UarmR
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	T_UarmL
		Part	:HealFire.part
		LifeSpan	30
	End
	
	Event
		Type	Local
		At	T_Hips
		Part	:HealFirehips.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	T_LarmR
		Part	:HealFire.part
		LifeSpan	30
	End


	Event
		Type	Local
		At	T_LarmL
		Part	:HealFire.part
		LifeSpan	30
	End

End

End				