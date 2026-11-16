#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound WW_ParticleLance_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Yellow.part
		Part	:Hit_Flash_Large.part
		Part	:Hit_Glow.part
		Part	:Hit_Shockwave.part
		Part	:Explosion_Glow_Lingering.part
		Part	:Hit_Sparks_Large.part
		Part	:Hit_Rings_Large.part
		Part	:Hit_Rings_Smoke.part
		Part	:Hit_Tendril.part

		Lifespan 5
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Rings_Smoke.part
		Lifespan 10
	End
End


#Condition
#	On 	Cycle
#	Time 	3
#
#
#	Event
#		Type	Local
#		At	Chest
#		BhvrOverride
#			StartJitter	1.0 1.0 1.0
#		End
#		Part	:Hit_Flash_Small.part
#		Part	:Hit_Rings_Small.part
#		Lifespan 1
#	End
#End

#############################################################

End