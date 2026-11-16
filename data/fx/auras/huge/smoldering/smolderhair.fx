#############################################################
## SmolderHair.fx
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
		part	:BodyStreaks01.part
		part	:HairStreaks01.part
		part	:BodySmoke01.part
		POther	Hair
	End

	Event
		Type	Local
		At	Eyes
		part	:BodyStreaks01.part
		part	:HairStreaks01.part
		part	:BodySmoke01.part
		POther	Hair
	End
End

#############################################################

End