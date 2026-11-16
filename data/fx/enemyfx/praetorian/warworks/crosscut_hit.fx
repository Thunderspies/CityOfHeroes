#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 40

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
		At	Root
		Sound WWV_CrossCut_Hit 150.0 150.0 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	12

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star.part
		Part	:Hit_Rings_Sharp.part
		Part	:Hit_Sparks_Blue.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	26

	Event
		Type	Local
		At	Root
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash_Small_Blue.part
		Part	:Hit_Star_Large.part
		Part	:Hit_Glow_Blue.part
		Part	:Hit_Rings_Sharp_Medium.part
		Part	:Hit_Sparks_Blue.part
		Lifespan 1
	End
End

#############################################################

End