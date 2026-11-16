#############################################################
## ElectricShackles_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################



Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.4 0.0
		End
		Part	:Sonic_Rings.part
		Part	:Sonic_Rings_Solid.part
		Part	:Sonic_Glow.part
	End
End

#############################################################

End


