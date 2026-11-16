#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	BladeAnchor
		Type	Local
		At	Chest
		Sound WWV_BladeFlurry_Hit 150.0 150.0 .8
		Lifespan 45
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star.part
		Part	:Hit_Sparks_Blue.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	5

	Event
		Type	Local
		At	BladeAnchor
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star.part
		Part	:Hit_Sparks_Blue.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	70

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Blue.part
		Part	:Hit_Star.part
		Part	:Hit_Glow_Blue.part
		Part	:Hit_Rings_Sharp_Large.part
		Part	:Hit_Sparks_Large_Blue.part
		Part	:Hit_ELectricity_Streaks.part
		Lifespan 1
	End
End
#############################################################

End