#########################################################
##	Fireweapon
########################################################
FxInfo

LifeSpan	60

########################################################



Condition
	On 	Time
	Time 	5

	Event
		ENAME   Tendrills
		Type	Local 
		At	chest
		#Part	:DarkDamageHitMist2.part
		Part	CustomizeablePowers\DarkArmor\DarkSoulDrainHitTendril2.part
		Part	CustomizeablePowers\DarkArmor\DarkSoulDrainHitPuddle2.part
	
		LifeSpan	30
	End

End

Condition
	On	Time
	time	46
	
	Event
		EName	Prime
		Type	start
		At	Chest
		Part	CustomizeablePowers\DarkArmor\DarknessRapidTenticles.part
		Part	CustomizeablePowers\DarkArmor\DarknessRapidTenticlesGreen.part
		Part	CustomizeablePowers\DarkArmor\DarknessRapidCloud.part
		Part	CustomizeablePowers\DarkArmor\DarknessRapidCloud2.part
		Part	CustomizeablePowers\DarkArmor\SoulDrainSkullGlow.part

		Sound warshadelifedrain2 70 70 .7
		Bhvr	CustomizeablePowers\DarkArmor\Skullprojectile1.bhvr
		
		Magnet	T_Chest
		LookAt  T_Chest
		LifeSpan	30
	End

End

Condition
	On 	PrimeHit

	Event
		Type	start
		At	T_Chest

		Part	CustomizeablePowers\DarkArmor\DarkHealMist.part
		Part	CustomizeablePowers\DarkArmor\DarkSoulDrainHitTendril2.part
		Part	CustomizeablePowers\DarkArmor\DarkSoulDrainHitPuddle2.part

		part	CustomizeablePowers\DarkArmor\RegenerationRing.part
		LifeSpan	30
			
	End
	
	Event
		EName 	Prime
		Type	Destroy

	End

End

End
##################################

