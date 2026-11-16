#############################################################
## N_Fire_Rune.fx
#############################################################

FxInfo

#############################################################


Condition
	On	Time
	Time	0

	Event
		Ename	RuneAnchor
		Type	StartPositOnly #Local
		At	Root
		BhvrOverride
			PositionOffset	0 0.2 0
		End
		Sound	Sewer_RuneFireEmblem_Loop 50 50 .15
	End
End


Condition
	On	Time
	Time	0

	Event
		Ename	Rune
		Type	Local
		At	RuneAnchor
		BhvrOverride
			Spin	0 0.003 0
		End
		part	:N_FireRune.part
		part	:N_FireRune_OuterGlow.part
	End

End




#############################################################

End