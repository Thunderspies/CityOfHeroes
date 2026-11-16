#############################################################
## TendrilHair.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## HAIR ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Neck
		part	:HairTendrils01.part
		POther	Hair
	End

	Event
		Type	Local
		At	Eyes
		part	:HairTendrils01.part
		part	:BodyTendrilsTrailSmall01.part
		POther	Head
	End
End

#############################################################

End