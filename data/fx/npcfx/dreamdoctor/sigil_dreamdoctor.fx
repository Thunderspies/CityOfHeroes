#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo
Flags	DontSuppress

#############################################################

Condition
	On 		Time
	Time	 	0

	Event
		EName	Sigil_DreamDoctor
		Type	Start
		At	Origin
		Bhvr	:Sigil_Offset.bhvr
		Part	:Sigil_Main.part
		Part	:Sigil_Glow.part
		Part	:Sigil_Highlight.part

	End
End

#############################################################

End