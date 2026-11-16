#############################################################
## Male_Shield_BlueSteel.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	:Male_Shield_BlueSteel.bhvr
		Anim	CustomShield_BlueSteel
	End
End

#############################################################

End