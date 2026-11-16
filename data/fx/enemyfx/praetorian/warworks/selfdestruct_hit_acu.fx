#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 70

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	StartPositOnly
		At	Chest
		Part	:Hit_Electricity_Sparks.part
		Part	:Explosion_Fire_Rings_Small2.part
		Part	:Hit_Flash_Small2.part

		Part	:Hit_Electricity_Small.part
		Part	:Hit_Glow_Blue.part
		Lifespan 1
	End
End

Condition
	On	Cycle
	Time	4

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Electricity_Small.part
		Part	:Hit_Glow_Blue.part
		Lifespan 2
	End
End

Condition
	On	Cycle
	Time	6

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Electricity_Small.part
		Part	:Hit_Electricity_Steam_Small.part
		Lifespan 2
	End
End

#############################################################

End