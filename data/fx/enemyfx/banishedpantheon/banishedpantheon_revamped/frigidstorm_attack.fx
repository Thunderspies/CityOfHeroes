###########################################################

FxInfo
LifeSpan 200

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidStorm_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidStorm_Attack_01 200 100 .8
	End
End

Condition
	On	Time
	Time	0

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
		Lifespan 65
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
		Lifespan 65
	End
End

###########################################################

End