#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Flash
		Type	Local
		At	C_Neck
		Part	:Idle_Welding_Core.part
		Part	:Idle_Welding_Sparks.part
		Sound Clockwork_Weld_Loop 50 50 .35
		bhvr	behaviors/sound/SoundIn01Out01.bhvr
	
	End
End

#############################################################


#############################################################

End