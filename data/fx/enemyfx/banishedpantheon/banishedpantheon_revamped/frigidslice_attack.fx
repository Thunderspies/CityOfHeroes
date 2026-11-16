
#########################################################################################

FxInfo
Flags	InheritAnimScale

Lifespan	30

#################################################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPB_FrigidSlice_Attack_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_FootL
		Part	:FrigidSlash_Claws_Smoke.part
		Part	:FrigidSlash_Claws_Subtractive.part
		POther	Fore_ToeL
	End

	Event
		EName 	Smoke02
		Type	Local
		At	Fore_FootR
		Part	:FrigidSlash_Claws_Smoke.part
		Part	:FrigidSlash_Claws_Subtractive.part
		POther	Fore_ToeR
	End

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_ToeR
		Part	:FrigidSlash_Claws_Glow_Additive.part
		Part	:FrigidSlash_Claws_Glow.part
		POther	Fore_FootR
	End

	Event
		EName 	Smoke02
		Type	Local
		At	Fore_ToeL
		Part	:FrigidSlash_Claws_Glow_Additive.part
		Part	:FrigidSlash_Claws_Glow.part
		POther	Fore_FootL
	End

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_LLegR
		Part	:FrigidSlash_Claws_Subtractive_Light.part
		POther	Fore_FootR

	End

	Event
		EName 	Smoke02
		Type	Local
		At	Fore_LLegL
		Part	:FrigidSlash_Claws_Subtractive_Light.part
		POther	Fore_FootL

	End
End

#################################################################################

End