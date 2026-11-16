#############################################################
## LichenHeal_Attack.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	Infection_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Chest
		#Part	:Infection_Spores.part
		#Part	:Infection_Streaks.part
		Part	:Infection_Dust_Hit.part
		Part	:Infection_Spores_Hit.part
		#Part	:Infection_Spores_Burst_Linger.part
		Part	:Light_Flash_Hit.part
		Part	:Light_Shockwave_Hit.part
		Lifespan 5
	End

#############################################################

End