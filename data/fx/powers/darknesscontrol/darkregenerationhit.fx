#########################################################
##	Fireweapon
########################################################

FxInfo
LifeSpan	60

########################################################

Input
	Inpname	Hips
End

########################################################



Condition
	On 	Time
	Time 	5

	Event
		ENAME   Tendrills
		Type	Local
		At	chest
		#Part	:DarkDamageHitMist2.part
		Part	:DarkSoulDrainHitTendril2.part
		Part	:DarkSoulDrainHitPuddle2.part

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
		Part	:DarknessRapidTenticles.part
		Part	:DarknessRapidTenticlesGreen.part
		Part	:DarknessRapidCloud.part
		Part	:DarknessRapidCloud2.part
		Part	:SoulDrainSkullGlow.part

		Sound warshadelifedrain2 70 70 .7
		Bhvr	behaviors/Skullprojectile1.bhvr

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

		Part	:DarkHealMist.part
		Part	:DarkSoulDrainHitTendril2.part
		Part	:DarkSoulDrainHitPuddle2.part

		part	:RegenerationRing.part
		LifeSpan	30

	End

	Event
		EName 	Prime
		Type	Destroy

	End

End

End
##################################

