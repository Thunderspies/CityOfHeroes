#########################################################
##	FireAura
FxInfo

LifeSpan	75

##################################
##Sound
##########################################

Condition
	On	Time
	Time	15
	
	Event
		Type	Local
		At	Origin
		Sound ignite2 50 50 .5
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
		LifeSpan	50
	
	End

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FireHandz.part
		LifeSpan	50
	
	End

	Event
		Type	Local
		At	Chest
		Part	:ChestHeal.part
		Sound cauterize 70 70 .95
		LifeSpan	65
	End
	
	Event
		Type	Local
		At	Chest
		Part	:HealFire.part
		Sound buildup 70 70 .36
		LifeSpan	60
	End
	
	Event
		Type	Local
		At	Chest
		Part	:ChestHealMetal.part
		Part	:ChestHealMetalInvert.part
		Part	:ChestHealGlowsGreenMetal.part
		Part	:ChestHealGlowsGreenMetalInvert.part
		LifeSpan	55
	End

	Event
		Type	Local
		At	head
		Part	:HealFire.part
		LifeSpan	60
	End

	Event
		Type	Local
		At	SpadR
		Part	:HealFire.part
		LifeSpan	60
	End


	Event
		Type	Local
		At	SpadL
		Part	:HealFire.part
		LifeSpan	60
	End

	Event
		Type	Local
		At	UarmR
		Part	:HealFire.part
		LifeSpan	60
	End


	Event
		Type	Local
		At	UarmL
		Part	:HealFire.part
		LifeSpan	60
	End
	
	Event
		Type	Local
		At	Hips
		Part	:HealFirehips.part
		LifeSpan	60
	End


	Event
		Type	Local
		At	LarmR
		Part	:HealFire.part
		LifeSpan	60
	End


	Event
		Type	Local
		At	LarmL
		Part	:HealFire.part
		LifeSpan	60
	End

End

End				