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
		EName	Sigil_Tsoo
		Type	Local
		At	Origin
		Bhvr	:Sigil_Offset.bhvr
		Part	:Sigil_Glow.part
		Part	:Sigil_Main.part

	End
End

Condition
	On	Cycle
	Time	50


	Event
		At	Sigil_Tsoo
		Type	Local
		Part	:Sigil_Main2.part
		Lifespan 	1
	End
End

#############################################################

End