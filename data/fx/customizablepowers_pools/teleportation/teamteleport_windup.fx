#############################################################
## TeamTeleport_Windup.fx
#############################################################

FxInfo
Lifespan 200

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:TSHeadGlow02.part
		Pmagnet HandR
		Sound teletrans5 80 80 .88
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:TSHeadGlow02.part
		Pmagnet HandL
	End
End

#############################################################

End

