#########################################################
##	jack
##

FxInfo

Input  
	InpName	WepR
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	9

	Event
		EName 	Sword
		Type	Local 
		At	WepR
		Geom	GEO_WepR_Sword_1
		Sound swordshing 70 70 .4
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

	Event
		EName 	Node1
		Type	Local 
		At	WepR
		Bhvr	:HandleOffset1.bhvr
		Sound valkyriesword2_loop 90 90 .39
	End

	Event
		EName 	Node2
		Type	Local 
		At	WepR
		POther	Node1
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
	End
End

End		