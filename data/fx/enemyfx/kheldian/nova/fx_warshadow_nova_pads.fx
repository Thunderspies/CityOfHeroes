#############################################################
## FX_Warshadow_Nova_Pads.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	SholderPad_L
		Type	Local
		At	SpadL
		Anim	FX_Warshadow_Nova_Pads
	End

	Event
		EName	SholderPad_R
		Type	Local
		At	SpadR
		Anim	FX_Warshadow_Nova_Pads
	End
End

#############################################################

End