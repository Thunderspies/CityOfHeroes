#############################################################
## FireHair.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## HAIR ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:FireFlame01.part
		part	:FireEmbers01.part
		part	:FireCrawl01.part
		part	:FireHairBase01.part
		POther	Head
	End
End

#############################################################

End