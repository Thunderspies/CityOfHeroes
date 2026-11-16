#############################################################
## Blood Widow Claws - Activation FX
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local
		At	WepR
		Sound claw1b 60 60 .5
	End
End

#############################################################

End