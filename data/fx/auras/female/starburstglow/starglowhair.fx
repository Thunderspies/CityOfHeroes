#############################################################
## StarGlowHead.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## HAIR ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	:HeadOffset1.bhvr
		part	:HeadStarGlow01.part
		part	:HeadStarGlow02.part
		Pmagnet Head
	End
End

#############################################################

End