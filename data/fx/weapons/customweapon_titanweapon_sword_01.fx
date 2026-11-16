#############################################################
## CustomWeapon_Broadsword.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		SetState LEFTHAND
		#Sound swordshing 100 100 .7
	End
End

#############################################################

End