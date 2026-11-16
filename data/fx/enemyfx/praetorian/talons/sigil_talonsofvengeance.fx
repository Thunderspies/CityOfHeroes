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
		Part	:Sigil_Glow.part
		Part	:Sigil_Highlight.part

	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.5


	Event
		EName	Sigil_DreamDoctor
		Type	Local
		At	Sigil_DreamDoctor
		Part	:Sigil_Main.part
		Lifespan 	3
		LifespanJitter	2
	End
End

#############################################################

End