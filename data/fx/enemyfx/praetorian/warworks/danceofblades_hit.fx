#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 60

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0



	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star_Large.part
		Part	:Hit_Glow_Blue.part
		Part	:Hit_Rings_Sharp_Medium.part
		Part	:Hit_Sparks_Blue.part
		Sound WWV_DanceOfBlades_Hit 150.0 150.0 .8
		Lifespan 1
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	BladeAnchor
		Type	Local
		At	Chest
		Lifespan 18
	End
End

Condition
	On	Cycle
	Time	4

	Event
		Type	Local
		At	BladeAnchor
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Sparks_Blue.part
		Lifespan 1
	End
End
#############################################################

End