#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 100

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
		At	Hips
		Sound WW_WarworksDeath 200 200 .8
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/Vanish.bhvr
		Flags 	AdoptParentEntity
		Part	:Hit_Electricity_Shockwave.part
		Part	:Hit_Electricity_Glow.part
		Part	:Hit_Flash_Large_Blue2.part
		Part	:Hit_Electricity_LargeBolts.part
		Part	:Hit_Electricity.part
		Part	:Hit_Electricity_Steam.part
		Part	:Hit_Tendril_Small.part
		Part	:Hit_Glow_Blue.part
		Part	:Hit_Electricity_Sparks.part
		Part	:Hit_Electricity_Shockwave.part
		Part	:Hit_Electricity_Sparks.part
		Part	:Hit_Electricity_Streaks.part
		Lifespan 3
	End

	Event
		Type	Local
		At	Hips
		Part	:Hit_Electricity_Rings_Flat.part
		Part	:Hit_Electricity_Glow_Flat.part
		Part	:Hit_Electricity_Bolts_Flat.part
		Part	:Hit_Flash_Large_Blue_Flat2.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Electricity.part
		Part	:Hit_Electricity_Steam.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Glow_Blue.part
		Part	:Hit_Electricity_Sparks.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Electricity.part
		Part	:Hit_Electricity_Steam.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Glow_Blue.part

		Lifespan 1
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Hips
		BhvrOverride
			StartJitter	1.0 1.0 1.0
		End
		Part	:Hit_Electricity.part
		Part	:Hit_Electricity_Steam.part
		Part	:Hit_Glow_Small.part
		Part	:Hit_Glow_Blue.part

		Lifespan 1
	End
End

#############################################################

End