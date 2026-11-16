#############################################################
## RecallFriend_Attack.fx
#############################################################

FxInfo
Lifespan 40

#############################################################

Input
	InpName	Hips
End

#############################################################


#########################################################
#####################    Cast    ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:TSHeadGlow02.part
		Pmagnet HandR
		SOund	teletrans5 80 30 .88

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:TSHeadGlow02.part
		Pmagnet HandL
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:THandGlow1.part
		Pmagnet HandL
	End
End

#############################################################

End

