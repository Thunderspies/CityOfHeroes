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
		Bhvr	behaviors/WingScale_FAIRY_Huge.bhvr
		BhvrOverride
			PositionOffset	0 .45 -.27
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
		Part	AnimatedCharacterParts/fairySmokeLocal.part
		Part	AnimatedCharacterParts/fairySmoke.part
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
		Part	AnimatedCharacterParts/fairySmokeLocal.part
		Part	AnimatedCharacterParts/fairySmoke.part
		Part	AnimatedCharacterParts/fairySparlies.part
	End
End

#############################################################

End