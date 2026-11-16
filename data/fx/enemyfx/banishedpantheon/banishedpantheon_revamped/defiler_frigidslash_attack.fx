
#########################################################################################

FxInfo
Flags	InheritAnimScale

Lifespan	60

#################################################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidSlash_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidSlash_Attack_01 200 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Smoke01
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_SmokeL.part
		Part	:Defiler_FrigidSlash_Claws_SubtractiveL.part
		POther	HandL
		Lifespan 30
	End


	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_Smoke.part
		Part	:Defiler_FrigidSlash_Claws_Subtractive.part
		POther	HandR
		Lifespan 30
	End

#	Event
#		EName 	Smoke02
#		Type	Local
#		At	Fore_ToeL
#		Part	:FrigidSlash_Claws_Glow_Additive.part
#		Part	:FrigidSlash_Claws_Glow.part
#		POther	Fore_FootL
#	End

End

#################################################################################

End