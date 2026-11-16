#############################################################
## GlowAuraFists.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## FISTS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmR
		part	:BodyAura01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	HandR
		part	:BodyAura01.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:BodyAura01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandL
		part	:BodyAura01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	HandR
		part	:BodyAura01.part
		part	:BodyAura01.part
	End

	Event
		Type	Local
		At	HandL
		part	:BodyAura01.part
		part	:BodyAura01.part
	End
End

## HEAD ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part	:HeadAura01.part
		Pmagnet Head
	End
End

#############################################################

End