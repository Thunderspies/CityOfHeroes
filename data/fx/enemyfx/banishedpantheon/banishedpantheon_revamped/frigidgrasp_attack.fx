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
		Sound 	BPR_FrigidGrasp_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	BP_FrigidGrasp_Attack_01 200 100 .8
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
		Part	:Ravager_FrigidSlash_Claws_SmokeL.part
		Part	:Ravager_FrigidSlash_Claws_SubtractiveL.part
		POther	HandL
		Lifespan 70
	End


	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part	:Ravager_FrigidSlash_Claws_SmokeR.part
		Part	:Ravager_FrigidSlash_Claws_SubtractiveR.part
		POther	HandR
		Lifespan 70
	End
End

###########################################################

End
