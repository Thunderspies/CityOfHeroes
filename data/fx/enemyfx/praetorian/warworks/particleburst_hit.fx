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
		Sound 	WW_ParticleBurst_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Large_Blue.part
		Part	:Hit_Rings_Blue.part
		Part	:Hit_Shockwave_Small.part
		Lifespan 1
	End
End


Condition
	On 	Cycle
	Time 	3


	Event
		Type	Local
		At	Chest
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Flash_Small.part
		Part	:Hit_Rings_Small.part
		Lifespan 1
	End
End

#############################################################

End