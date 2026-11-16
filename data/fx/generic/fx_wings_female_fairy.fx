#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

## WINGS ###########################################################

Condition
	On 	Time
	Time 	1

	Event
		Ename	WingAnim
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_FAIRY_Female.bhvr
		BhvrOverride
			PositionOffset	0 .15 .1
		End
		Anim	FX_wings_FAIRY
		Part	AnimatedCharacterParts/fairySparliesLocal.part
	End
End

## WING FX ###########################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	UarmL
		Part	AnimatedCharacterParts/fairySmokeLocal2.part
		Part	AnimatedCharacterParts/fairySmoke2.part
		Part	AnimatedCharacterParts/fairySparlies.part
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	WingAnim
		AnimPiv	UarmR
		Part	AnimatedCharacterParts/fairySmokeLocal2.part
		Part	AnimatedCharacterParts/fairySmoke2.part
		Part	AnimatedCharacterParts/fairySparlies.part
	End
End

#############################################################

End