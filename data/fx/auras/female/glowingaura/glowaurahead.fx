#############################################################
## GlowAuraHead.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## HEAD ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		part	:HeadAura01.part
		Pmagnet Head
	End
End

#############################################################

End