#############################################################
## Ninja_katana_left.fx
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
		Sound swordshing 70 70 .6
	End
End

#############################################################

End