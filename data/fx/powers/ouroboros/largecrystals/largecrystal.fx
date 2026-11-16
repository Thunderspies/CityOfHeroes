#########################################################
## Ouroboros Portal
#########################################################

FxInfo

## CREATION #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Portal
		Type	Local
		At	Root
		Part	:CrystalFire_FireCore01.part
		part3	:CrystalFire_Flames01.part
		part3	:CrystalFire_Flames02.part
		Part	:CrystalFire_LargeGlow01.part
	End

	Event
		EName	Portal
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0.0 4.0 0.0
		End
		Part	:CrystalFire_Rays01.part
		Part	:CrystalFire_Sparkles01.part
	End

End

#########################################################

End